﻿#pragma once
#include <vector>
#include <deque>
#include <tuple>
#include <boost\thread.hpp>
#include <map>
#include <string>



namespace servercore {
	class CTask {
	private:
		std::map<std::string, std::string> task_;
	public:
		CTask(std::string task_type) {
			task_["type"] = task_type;
		};
		~CTask() {};
		void addParam(std::string key, std::string value)
		{
			task_[key] = value;
		}
		std::string getType()
		{
			return task_["type"];
		}
		std::string getValue(std::string key);
	};



	class CTaskList
	{
	public:
		CTaskList();
		~CTaskList();
		void addTask(CTask task);
		//取出一条任务，如果没有任务则阻塞
		CTask popTask();
	private:
		boost::condition_variable_any m_cond_;//条件变量
		boost::mutex task_mu_;
		std::deque<CTask> tasks_;
	};
}//namespace servercore
