#pragma once
#include "CAgent.h"
#include <map>
#include "IModules.h"
#include "io_context_pool.hpp"


namespace servercore {
	class CAgentPool:
		public IAgentModule
	{
	public:
		CAgentPool(io_context_pool& io_pool);
		~CAgentPool();
		std::shared_ptr<CAgent> createAgent(
			IAgentHandlers& agent_handlers,
			std::shared_ptr<CTaskList> task_list
			)override;
		friend class CAgent;
	private:
		uint64_t client_count_;
		//agent_id, agent_ptr
		std::map<uint64_t, std::shared_ptr<CAgent>> agent_map_;
		io_context_pool& io_pool_;
	};
}