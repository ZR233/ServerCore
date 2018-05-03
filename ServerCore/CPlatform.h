#pragma once
#include <map>
#include "IModules.h"
#include "CTaskList.h"
#include <memory>
#include "IHandlers.h"
//#include "io_context_pool.hpp"


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
			int thread,
			std::shared_ptr<CTaskList> task_list,
			IHandlers& handler
			);
		std::shared_ptr<CTaskList> registerTaskList(const char* name);
	private:
		std::map<std::string,std::shared_ptr<IAgentModule>> agent_set_;
		std::map<std::string, std::shared_ptr<IServerModule>> server_set_;
		std::map<std::string, std::shared_ptr<CTaskList>> tasklist_set_;

		int thread_num;
	};
}


