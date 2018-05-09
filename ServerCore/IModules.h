#pragma once
#include <memory>
#include <boost\asio.hpp>
#include "CAgent.h"


namespace servercore {
	class IAgentModule
	{
	public:
		IAgentModule();
		virtual ~IAgentModule();
		virtual CAgent* createAgent(
			const char* name,
			IAgentHandlers& agent_handlers
		) = 0;
		virtual CAgent* useAgent(const char* name) = 0;
	};

	class IServerModule
	{
	public:
		IServerModule();
		~IServerModule();
		virtual void run()=0;

		virtual void join() = 0;
		virtual void setMaxLink(size_t max) = 0;
		virtual void setRestrictByAttribute(std::string key, std::string value, size_t num) = 0;
		virtual void kickByAttribute(std::string key, std::string value) = 0;
	};

	class CTask;
	class ITaskHandler;
	class ITaskDealerModule
	{
	public:
		ITaskDealerModule() = default;
		virtual ~ITaskDealerModule() = default;
		virtual void run()=0;
		virtual void join() = 0;
		virtual void addTask(CTask task) = 0;
		virtual void registerAgents(IAgentModule* agents) = 0;
		virtual void registerServer(IServerModule& server) = 0;
	};
}


