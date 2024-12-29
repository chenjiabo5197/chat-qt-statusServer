#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include "Singleton.h"
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "hiredis.h"
#include <cassert>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// 错误码
enum ErrorCodes
{
	Success = 0,
	Error_Json = 1001,		// json解析错误
	RPCFailed = 1002,		// RPC请求错误
	VerifyExpired = 1003,	// 验证码过期
	VerifyCodeErr = 1004,	// 验证码错误
	UserExist = 1005,		// 用户已存在
	PasswdErr = 1006,		// 密码错误
	EmailNotMatch = 1007,	// 邮箱不匹配
	PasswdUpFailed = 1008,	// 更新密码失败
	PasswdInvalid = 1009,	// 密码无效
};

class Defer {
public:
	// 接受一个lambda表达式或函数指针
	Defer(std::function<void()> func): func_(func){}

	// 析构函数中执行传入的函数
	~Defer()
	{
		func_();
	}

private:
	std::function<void()> func_;
};

#define CODEPREFIX "code_"

// 采用单例后此处不需要了，直接调用ConfigMgr::Inst()
//class ConfigMgr;
//extern ConfigMgr gCfgMgr;
