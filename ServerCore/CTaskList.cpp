#include "stdafx.h"
#include "CTaskList.h"
#include "../ZRUtils/ZRUtils/Utils.hpp"



namespace servercore {

	std::string CTask::getValue(std::string key)
	{
		auto i = task_.find(key);
		if (i != task_.end())
		{
			return i->second;
		}
		else
		{
			BOOST_THROW_EXCEPTION(zrutils::exception("任务没有该关键字"));
		}
	}


	CTaskList::CTaskList()
	{
	}


	CTaskList::~CTaskList()
	{
	}

	void CTaskList::addTask(std::vector<std::string> parameters, int task_type)
	{
		boost::unique_lock<boost::mutex> mu(task_mu_);
		auto task = std::tie(parameters, task_type);
		tasks_.push_back(task);
		m_cond_.notify_one();
	}

	std::tuple<std::vector<std::string>, int> CTaskList::popTask()
	{
		boost::unique_lock<boost::mutex> mu(task_mu_);
		if (tasks_.empty())
		{
			//如果队列中没有任务，则等待互斥锁 
			m_cond_.wait(mu);//
		}
		auto task = tasks_.front();
		tasks_.pop_front();
		return task;
	}



}//namespace servercore 