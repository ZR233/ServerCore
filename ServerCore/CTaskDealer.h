#pragma once
#include "IModules.h"
#include "ITaskHandler.h"
#include "CTaskList.h"
#include <boost\thread.hpp>
namespace servercore {
	class CTaskDealer :
		public ITaskDealerModule
	{
	private:
		CTaskList & task_list_;
		std::shared_ptr<ITaskHandler> task_handler_ptr_;
		boost::thread deal_thread_;
	public:
		CTaskDealer(CTaskList& task_list, ITaskHandler& task_handler);
		
		void run()override;
		void join()override;
		void registerAgents(IAgentModule& agents)override;
		void registerServer(IServerModule& server)override;

		~CTaskDealer();

	};

		
}


