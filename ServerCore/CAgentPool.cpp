#include "stdafx.h"
#include "CAgentPool.h"
#include <boost\date_time.hpp>
namespace servercore {
	CAgentPool::CAgentPool(io_context_pool& io_pool, CPlatform& plat):
		io_pool_(io_pool),
		plat_(plat)
	{
	}
	CAgentPool::~CAgentPool()
	{
	}
	CAgent* CAgentPool::createAgent(
			const char* name,
			IAgentHandlers& agent_handlers
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
			io_pool_.get_io_context(),
			plat_));
		agent_map_[name] = agent;
		return &(*agent);
	}

	CAgent* CAgentPool::useAgent(const char* name)
	{
		auto agent_ptr_iter = agent_map_.find(name);
		if (agent_ptr_iter != agent_map_.end())
		{
			return &(*((*agent_ptr_iter).second));
		}
		else
		{
			std::string err_str = "没有名为[";
			err_str += name;
			err_str += "]的Agent";
			BOOST_THROW_EXCEPTION(zrutils::exception(err_str));
		}
	}
}