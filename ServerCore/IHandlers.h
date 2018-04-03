#pragma once
#include "ITaskHandler.h"
#include "IConnectionHandler.h"
#include "connection.h"


namespace servercore {
	class IHandlers :
		public ITaskHandler,
		public IConnectionHandler
	{
	public:
		IHandlers();
		virtual ~IHandlers();

		//返回 new 自身 
		//boost::shared_ptr<子类>
		virtual boost::shared_ptr<IHandlers> newHandler();
	};
}


