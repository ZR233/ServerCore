#include "stdafx.h"
#include "CPlatform.h"
#include "CAgentPool.h"
#include "CServer.h"
#include "externvar.hpp"
#include "CTaskDealer.h"


namespace servercore {
	CPlatform::CPlatform():
		io_pool_(externvar::thread_num)
	{
		externvar ini;
	}


	CPlatform::~CPlatform()
	{
	}
	std::shared_ptr<IAgentModule> CPlatform::registerAgent(const char name[])
	{
		auto temp(std::make_shared<CAgentPool>(CAgentPool(io_pool_)));
		agent_set_[name] = temp;
		return temp;
	}
	std::shared_ptr<IServerModule> CPlatform::registerServer(
		const char name[],
		const char address[],
		const char port[],
		std::shared_ptr<CTaskList> task_list,
		IServerHandler& handler
	)
	{
		std::shared_ptr<IServerModule> temp(new CServer(
			address,
			port,
			io_pool_,
			*task_list,
			handler));
		server_set_[name] = temp;
		return temp;
	}
	std::shared_ptr<CTaskList> CPlatform::registerTaskList(const char * name)
	{
		std::shared_ptr<CTaskList> temp(new CTaskList());
		tasklist_set_[name] = temp;
		return temp;
	}


	std::shared_ptr<ITaskDealerModule> CPlatform::registerDealer(
		const char * name, 
		ITaskHandler & task_handlers, 
		std::shared_ptr<CTaskList> task_list)
	{
		std::shared_ptr<ITaskDealerModule> dealer(new CTaskDealer(
			*task_list, task_handlers
		));
		task_dealer_set_[name] = dealer;
		return dealer;
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