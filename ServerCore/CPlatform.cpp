#include "stdafx.h"
#include "CPlatform.h"
#include "CAgentPool.h"
#include "CServer.h"
#include "externvar.hpp"

namespace servercore {
	CPlatform::CPlatform()
	{
		externvar ini;
	}


	CPlatform::~CPlatform()
	{
	}
	std::shared_ptr<IAgentModule> CPlatform::registerAgent(const char name[])
	{
		auto temp = std::make_shared<CAgentPool>(CAgentPool());
		agent_set_[name] = temp;
		return temp;
	}
	std::shared_ptr<IServerModule> CPlatform::registerServer(
		const char name[],
		const char address[],
		const char port[],
		int thread,
		std::shared_ptr<CTaskList> task_list,
		IHandlers& handler
	)
	{
		std::shared_ptr<IServerModule> temp(new CServer(
			address,
			port,
			thread,
			*task_list,
			handler));
		server_set_[name] = temp;
		return temp;
	}
	std::shared_ptr<CTaskList> CPlatform::registerTaskList(const char * name)
	{
		std::shared_ptr<CTaskList> temp(new CTaskList());
		tasklist_set_[name] = temp;
		return temp;
	}
}