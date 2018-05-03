#include "stdafx.h"
#include "CAgentPool.h"
#include <boost\date_time.hpp>
namespace servercore {
	CAgentPool::CAgentPool()
	{
	}


	CAgentPool::~CAgentPool()
	{
	}
	std::shared_ptr<CAgent> CAgentPool::createAgent(
			IAgentHandlers& agent_handlers,
			boost::asio::io_context& io,
			CTaskList& task_list
		)
	{
		auto pt = boost::posix_time::second_clock::local_time();
		tm time = boost::posix_time::to_tm(pt);
		if (client_count_ == 999)
		{
			client_count_ = 0;
		}
		++client_count_;
		auto id = client_count_;
		id += 1000 * time.tm_sec;
		id += 1000 * 100 * time.tm_min;
		id += 1000 * 100 * 100 * (time.tm_hour + 1);
		std::shared_ptr<CAgent> agent(new CAgent(
			id,
			this,
			agent_handlers,
			io,
			task_list));
		agent_map_[id] = agent;
		return agent;
	}
}