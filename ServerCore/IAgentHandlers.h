#pragma once
#include <boost\shared_ptr.hpp>
namespace servercore {
	class IAgentHandlers
	{
	public:
		IAgentHandlers();
		virtual ~IAgentHandlers();
		virtual boost::shared_ptr<IAgentHandlers> getNewInstance() = 0;
	};
}//namespace servercore
