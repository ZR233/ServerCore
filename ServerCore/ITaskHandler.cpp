#include "stdafx.h"
#include "ITaskHandler.h"
#include "CLog.h"
#include <boost\locale.hpp>

namespace servercore {
	void ITaskHandler::registerAgents(IAgentModule & agents)
	{
		agents_ = &agents;
	}
	void ITaskHandler::registerServer(IServerModule & server)
	{
		server_ = &server;
	}
	ITaskHandler::ITaskHandler()
	{
	}


	ITaskHandler::~ITaskHandler()
	{
		BOOST_LOG_SEV(wlg::get(), debug) << L"ITaskHandler析构";
	}
	void ITaskHandler::taskHandler(CTask task)
	{
		BOOST_LOG_SEV(wlg::get(), debug) << L"-----------\n任务类型：\n" << task.getType();
		BOOST_LOG_SEV(wlg::get(), debug) << L"-----------";
	}
	std::shared_ptr<ITaskHandler> ITaskHandler::getNewInstance()
	{
		std::shared_ptr<ITaskHandler> hand(new ITaskHandler());
		BOOST_LOG_SEV(wlg::get(), warning) << L"未重写ITaskHandler子类的getNewInstance方法";
		return hand;
	}
}