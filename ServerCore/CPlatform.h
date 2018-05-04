#pragma once
#include <map>
#include "IModules.h"
#include "CTaskList.h"
#include <memory>
#include "IHandlers.h"
#include "io_context_pool.hpp"


namespace servercore {
	class CPlatform
	{
	public:
		CPlatform();
		~CPlatform();
		std::shared_ptr<IAgentModule> registerAgent(const char name[]);
		std::shared_ptr<IServerModule> registerServer(
			const char name[],
			const char address[],
			const char port[],
			std::shared_ptr<CTaskList> task_list,
			IServerHandler& handler
			);
		std::shared_ptr<CTaskList> registerTaskList(const char* name);
		std::shared_ptr<ITaskDealerModule> registerDealer(
			const char* name,
			ITaskHandler& task_handlers,
			std::shared_ptr<CTaskList> task_list
		);

		void run();
		void join();
	private:
		std::map<std::string,std::shared_ptr<IAgentModule>> agent_set_;
		std::map<std::string, std::shared_ptr<IServerModule>> server_set_;
		std::map<std::string, std::shared_ptr<CTaskList>> tasklist_set_;
		std::map<std::string, std::shared_ptr<ITaskDealerModule>> task_dealer_set_;


		int thread_num_;

		io_context_pool io_pool_;
	};
}


