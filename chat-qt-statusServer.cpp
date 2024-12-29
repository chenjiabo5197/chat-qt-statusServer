// chat-qt-statusServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "const.h"
#include "ConfigMgr.h"
#include <thread>
#include "StatusServiceImpl.h"

void RunServer()
{
    auto& gCfgMgr = ConfigMgr::Inst();
    std::string server_address = gCfgMgr["StatusServer"]["Host"] + ":" + gCfgMgr["StatusServer"]["Port"];
    StatusServiceImpl service;

    grpc::ServerBuilder builder;
    // 监听端口和添加服务
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    // 构建并启动grpc服务器
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // 创建Boost.Asio的io_context
    boost::asio::io_context io_context;
    // 创建signal_set 用于捕获SIGINT
    // SIGINT是中断信号，通常由用户按下Ctrl+C产生，用于请求程序中断。
    // SIGTERM是终止信号，用于请求程序正常终止。
    // 通过将这些信号添加到signal_set中，程序可以在这些信号发生时得到通知
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

    // 设置异步等待SIGINT信号
    signals.async_wait([&server, &io_context](const boost::system::error_code error, int signal_number) {
        if (!error)
        {
            std::cout << "shut down server" << std::endl;
            // 关闭服务器
            server->Shutdown();
            // 停止io_context
            io_context.stop();
        }
    });

    // 在单独的线程中运行io_context
    std::thread([&io_context]() {
        io_context.run();
    }).detach();

    // 等待服务器关闭
    server->Wait();
}

int main(int argc, char** argv)
{
    try
    {
        RunServer();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error=" << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}

