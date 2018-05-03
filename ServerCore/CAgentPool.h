#pragma once
#include "CAgent.h"
#include <map>
#include "IModules.h"


namespace servercore {
	class CAgentPool:
		public IAgentModule
	{
	public:
		CAgentPool();
		~CAgentPool();
		std::shared_ptr<CAgent> createAgent(
			IAgentHandlers& agent_handlers,
			boost::asio::io_context& io,
			CTaskList& task_list
			)override;
		friend class CAgent;
	private:
		uint64_t client_count_;
		//agent_id, agent_ptr
		std::map<uint64_t, std::shared_ptr<CAgent>> agent_map_;
	};

}