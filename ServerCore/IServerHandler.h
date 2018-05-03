﻿#pragma once
#include <vector>
#include <string>
#include <map>
#include "CTaskList.h"

namespace servercore
{
	class connection;
	class IServerHandler
	{
	public:
		IServerHandler();
		virtual ~IServerHandler();
		//待实现函数-------------------------------------------------

		//消息头处理
		virtual void readHeadHandler(std::vector<uint8_t> buf);
		//消息体处理
		virtual void readBodyHandler(std::vector<uint8_t> buf);
		////服务器任务处理
		//virtual void serverTaskHandler(std::tuple<std::vector<std::string>, int> task) = 0;


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
		void registServerTasks(CTaskList& server_ta)
		{
			server_tasks_ = &server_ta;
		}
		//设置消息长度数据段的长度
		void setHeadLen(int len)
		{
			head_len_ = len;
		}
		int getHeadLen()
		{
			return head_len_;
		}
	protected:
		void addServerTask(CTask task);
		/*std::tuple<std::vector<std::string>, int> popServerTask()
		{
		auto task = server_tasks_->popTask();
		return task;
		}*/
		std::vector<uint8_t> buf_head_;
		std::vector<uint8_t> buf_body_;
		std::map<std::string, std::string> attribute_;
		connection* connection_ = nullptr;
		CTaskList* server_tasks_ = nullptr;
		int head_len_;
	};
}
