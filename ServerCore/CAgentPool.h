#pragma once
#include "CAgent.h"
#include <map>
#include "IModules.h"
#include "io_context_pool.hpp"


namespace servercore {
	class CPlatform;
	class CAgentPool:
		public IAgentModule
	{
	public:
		CAgentPool(io_context_pool& io_pool, CPlatform& plat);
		~CAgentPool();
		CAgent* createAgent(
			const char* name,
			IAgentHandlers& agent_handlers
			)override;
		CAgent* useAgent(const char* name)override;
		friend class CAgent;
	private:
		uint64_t client_count_;
		//agent_name, agent_ptr
		std::map<std::string, std::shared_ptr<CAgent>> agent_map_;
		io_context_pool& io_pool_;
		CPlatform& plat_;
	};
}