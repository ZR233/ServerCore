#include "stdafx.h"
#include "CPlatform.h"
#include "CAgentPool.h"
#include "CServer.h"
#include "externvar.hpp"
#include "CTaskDealer.h"
#include "..\ZRUtils\ZRUtils\Utils.hpp"

namespace servercore {
	CPlatform::CPlatform():
		io_pool_(externvar::thread_num)
	{
		externvar ini;
	}


	CPlatform::~CPlatform()
	{
	}
	IAgentModule* CPlatform::registerAgent(const char name[])
	{
		auto temp(std::make_shared<CAgentPool>(CAgentPool(io_pool_,*this)));
		agent_set_[name] = temp;
		return &(*temp);
	}
	std::shared_ptr<IServerModule> CPlatform::registerServer(
		const char name[],
		const char address[],
		const char port[],
		IServerHandler& handler
	)
	{
		std::shared_ptr<IServerModule> temp(new CServer(
			address,
			port,
			io_pool_,
			*this,
			handler));
		server_set_[name] = temp;
		return temp;
	}



	std::shared_ptr<ITaskDealerModule> CPlatform::registerDealer(
		const char * name, 
		ITaskHandler & task_handlers
	)
	{
		std::shared_ptr<ITaskDealerModule> dealer(new CTaskDealer(
			task_handlers
		));
		task_dealer_set_[name] = dealer;
		return dealer;
	}
	ITaskDealerModule* CPlatform::useTaskDealer(const char * name)
	{
		auto task_dealer_iter = task_dealer_set_.find(name);
		if (task_dealer_iter != task_dealer_set_.end())
		{
			return &(*(task_dealer_iter->second));
		}
		else
		{
			std::string err_str = "没有名为[";
			err_str += name;
			err_str += "]的TaskDealer";
			BOOST_THROW_EXCEPTION(zrutils::exception(err_str));
		}
		
	}
	void CPlatform::run()
	{
		io_pool_.run();
		for (auto dealer: task_dealer_set_)
		{
			dealer.second->run();
		}
	}
	void CPlatform::join()
	{
		io_pool_.join();
	}
}