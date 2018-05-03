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
		virtual std::shared_ptr<CAgent> createAgent(
			IAgentHandlers& agent_handlers,
			boost::asio::io_context& io,
			CTaskList& task_list
		) = 0;
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
}


