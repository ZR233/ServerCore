#pragma once
#include "ITaskHandler.h"
#include "IServerHandler.h"
#include "../ZRUtils/ZRUtils/Utils.hpp"
#include "IAgentHandlers.h"



namespace servercore {
	class IHandlers:
		virtual public ITaskHandler,
		virtual public IServerHandler,
		virtual public IAgentHandlers
	{
	public:
		IHandlers();
		virtual ~IHandlers();
	};
}


