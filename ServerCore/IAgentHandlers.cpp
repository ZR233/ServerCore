#include "stdafx.h"
#include "IAgentHandlers.h"
#include "CLog.h"
#include <string>

namespace servercore {
	IAgentHandlers::IAgentHandlers()
	{
	}


	IAgentHandlers::~IAgentHandlers()
	{
	}
	void IAgentHandlers::dealRecvData(std::vector<uint8_t> data_buf)
	{
		std::string str;
		str.assign(data_buf.begin(), data_buf.end());
		BOOST_LOG_SEV(lg::get(), debug) << "收到字节数:" << std::to_string(data_buf.size())
			<<"\n内容:"<< str;

	}
	void IAgentHandlers::connectFail(CAgent* this_agent)
	{

	}
	void IAgentHandlers::setDefaultIo(boost::asio::io_context& io)
	{
		default_io_ptr_ = &io;
	}

}//namespace servercore