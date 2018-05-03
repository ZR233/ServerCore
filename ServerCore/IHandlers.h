#pragma once
#include "ITaskHandler.h"
#include "IServerHandler.h"
#include "../ZRUtils/ZRUtils/Utils.hpp"

namespace servercore {
	class IHandlers:
		public ITaskHandler,
		public IServerHandler
	{
	public:
		IHandlers();
		virtual ~IHandlers();
		//返回 new 自身 
		//boost::shared_ptr<子类>
		virtual boost::shared_ptr<IHandlers> newHandler();
	};
}


