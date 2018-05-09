#pragma once
#include <memory>
#include <vector>
#include <boost\asio.hpp>
#include "IHandlerBase.hpp"
#include "CTaskList.h"

namespace servercore {

	class CAgent;
	class IAgentHandlers
	{
	public:
		IAgentHandlers();
		virtual ~IAgentHandlers();
		//需返回指向自身的智能指针
		virtual std::shared_ptr<IAgentHandlers> getNewInstance();
		virtual void dealRecvData(std::vector<uint8_t> data_buf);
		virtual void connectFail();
		virtual std::vector<uint8_t> activeTest() { return std::vector<uint8_t>(); };
		virtual std::vector<uint8_t> loginDeal() { return std::vector<uint8_t>(); };

		void setDefaultIo(boost::asio::io_context& io);
		void setThisAgentPtr(CAgent* this_agent);

		bool noDataCloseSocket() { return no_data_close_socket_; };
		int getActiveTestTimeMillsec() { return active_test_time_millsec_; };
		int getSocketCloseSeadlineMillsec() { return socket_close_deadline_millsec_; };
	protected:
		boost::asio::io_context* default_io_ptr_;
		CAgent* this_agent_ptr_;
		int active_test_time_millsec_;
		int socket_close_deadline_millsec_;
		bool no_data_close_socket_;
	};
}//namespace servercore
