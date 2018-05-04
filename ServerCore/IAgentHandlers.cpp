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
	std::shared_ptr<IAgentHandlers> IAgentHandlers::getNewInstance()
	{
		std::shared_ptr<IAgentHandlers> hand(new IAgentHandlers());
		BOOST_LOG_SEV(wlg::get(), warning) << L"未重写IAgentHandlers子类的newHandler方法";
		return hand;
	}
	void IAgentHandlers::dealRecvData(std::vector<uint8_t> data_buf)
	{
		std::string str;
		str.assign(data_buf.begin(), data_buf.end());
		BOOST_LOG_SEV(lg::get(), debug) << "收到字节数:" << std::to_string(data_buf.size())
			<<"\n内容:"<< str;

	}
	void IAgentHandlers::connectFail()
	{

	}
	void IAgentHandlers::setDefaultIo(boost::asio::io_context& io)
	{
		default_io_ptr_ = &io;
	}

	void IAgentHandlers::setThisAgentPtr(CAgent * this_agent)
	{
		this_agent_ptr_ = this_agent;
	}

}//namespace servercore