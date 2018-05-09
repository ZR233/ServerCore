#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <memory>
#include "CTaskList.h"
#include "IModules.h"

namespace servercore {
	class ITaskHandler
	{
	protected:
		IAgentModule* agents_= nullptr;
		IServerModule* server_ = nullptr;
	public:
		//----------需重写---------------

		//用于产生系统新实例
		virtual std::shared_ptr<ITaskHandler> getNewInstance();
		//服务器任务处理
		virtual void taskHandler(CTask task);
		

		//-------------------------------


		void registerAgents(IAgentModule& agents);
		void registerServer(IServerModule& server);
		ITaskHandler();
		virtual ~ITaskHandler();
	};
}
