#pragma once
#include <vector>
#include <string>
#include <map>
#include <boost\shared_ptr.hpp>
namespace servercore
{
	class connection;
	class server_tasks;
	class IConnectionHandler
	{
	public:
		IConnectionHandler(size_t head_len, size_t body_len);
		virtual ~IConnectionHandler();
		//待实现函数-------------------------------------------------

		//消息头处理
		virtual void readHeadHandler(std::vector<uint8_t> buf);
		//消息体处理
		virtual void readBodyHandler(std::vector<uint8_t> buf) = 0;
		//服务器任务处理
		virtual void serverTaskHandler(std::tuple<std::vector<std::string>, int> task) = 0;
		//返回 new 自身 
		//boost::shared_ptr<子类>
		virtual boost::shared_ptr<IConnectionHandler> newHandler() = 0;
		//
		virtual void kickClientHandler() {};
		//待实现函数-------------------------------------------------


		std::vector<uint8_t> getBufHead()
		{
			return buf_head_;
		}
		std::vector<uint8_t> getBufBody()
		{
			return buf_body_;
		}
		void setAttribute(std::string key, std::string value)
		{
			attribute_[key] = value;
		}
		std::string getAttribute(std::string key)
		{
			return attribute_[key];
		}
		void registConnection(connection& conn)
		{
			connection_ = &conn;
		}
		void registServerTasks(server_tasks& server_ta)
		{
			server_tasks_ = &server_ta;
		}
	protected:
		void addServerTask(std::vector<std::string> parameters, int task_type);
		/*std::tuple<std::vector<std::string>, int> popServerTask()
		{
		auto task = server_tasks_->popTask();
		return task;
		}*/
		std::vector<uint8_t> buf_head_;
		std::vector<uint8_t> buf_body_;
		std::map<std::string, std::string> attribute_;
		connection* connection_ = nullptr;
		server_tasks* server_tasks_ = nullptr;
	};
}

