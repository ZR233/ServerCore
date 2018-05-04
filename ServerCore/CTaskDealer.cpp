#include "stdafx.h"
#include "CTaskDealer.h"


namespace servercore {
	CTaskDealer::CTaskDealer(CTaskList & task_list, ITaskHandler & task_handler) :
		task_list_(task_list),
		task_handler_ptr_(task_handler.getNewInstance())
	{
	}

	void CTaskDealer::run()
	{
		deal_thread_ = std::move(boost::thread([this] {
			while (true)
			{
				boost::this_thread::interruption_point();
				task_handler_ptr_->taskHandler(task_list_.popTask());
			}
		}));
	}
	void CTaskDealer::join()
	{
		deal_thread_.join();
	}
	void CTaskDealer::registerAgents(IAgentModule& agents)
	{
		task_handler_ptr_->registerAgents(agents);
	}
	void CTaskDealer::registerServer(IServerModule& server)
	{
		task_handler_ptr_->registerServer(server);
	}

	CTaskDealer::~CTaskDealer()
	{
		deal_thread_.interrupt();
	}
}

