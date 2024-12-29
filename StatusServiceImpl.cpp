#include "StatusServiceImpl.h"
#include "const.h"
#include "ConfigMgr.h"

std::string generate_unique_string()
{
	// 创建uuid对象
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	// 将uuid转化为字符串
	std::string unique_string = to_string(uuid);
	return unique_string;
}

StatusServiceImpl::StatusServiceImpl()
{
	auto gCfgMgr = ConfigMgr::Inst();
	ChatServer server;
	server.port = gCfgMgr["ChatServer1"]["Port"];
	server.host = gCfgMgr["ChatServer1"]["Host"];
	server.conn_count = 0;
	server.name = gCfgMgr["ChatServer1"]["Name"];
	_servers[server.name] = server;

	server.port = gCfgMgr["ChatServer2"]["Port"];
	server.host = gCfgMgr["ChatServer2"]["Host"];
	server.conn_count = 0;
	server.name = gCfgMgr["ChatServer2"]["Name"];
	_servers[server.name] = server;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
	std::string prefix("chen status server has received=");
	const auto& server = getChatServer();
	reply->set_host(server.host);
	reply->set_port(server.port);
	reply->set_error(ErrorCodes::Success);
	reply->set_token(generate_unique_string());
	insertToken(request->uid(), reply->token());
	return Status::OK;
}

Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request, LoginRsp* reply)
{
	return Status();
}

void StatusServiceImpl::insertToken(int uid, std::string token)
{
	std::lock_guard<std::mutex> lock(_token_mtx);
	_tokens[uid] = token;
}

ChatServer StatusServiceImpl::getChatServer()
{
	std::lock_guard<std::mutex> lock(_server_mtx);
	auto minServer = _servers.begin()->second;
	// 使用for循环找到当前连接最少的chatserver
	for (const auto& server: _servers)
	{
		if (server.second.conn_count < minServer.conn_count)
		{
			minServer = server.second;
		}
	}
	return minServer;
}
