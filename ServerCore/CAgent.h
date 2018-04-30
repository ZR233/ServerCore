#pragma once
#include "IAgentHandlers.h"
namespace servercore {
	class IAgentPool;

	class CAgent
	{
	public:
		CAgent(IAgentPool& agent_pool, IAgentHandlers& agent_handlers);
		~CAgent();

		void start();
		void stop();

	};
}//namespace servercore 
