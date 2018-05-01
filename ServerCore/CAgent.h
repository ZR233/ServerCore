#pragma once
#include "IAgentHandlers.h"
#include <boost\asio.hpp>
#include "CTaskList.h"
#include <deque>
#include <mutex>

namespace servercore {
	class CAgentPool;

	class CAgent:
		public std::enable_shared_from_this<CAgent>
	{
	public:
		CAgent(
			uint64_t agent_id,
			CAgentPool* agent_pool, 
			IAgentHandlers& agent_handlers, 
			boost::asio::io_context& io,
			CTaskList& task_list
		);
		~CAgent();
		void connect(std::string ip, std::string port);
		void send(std::vector<uint8_t> data_buf);
		void disconnect();
		void destroy();
	private:
		void do_write();
		boost::asio::io_context& io_;
		boost::asio::io_context::strand strand_;
		std::shared_ptr<IAgentHandlers> agent_handlers_ptr_;
		CTaskList& task_list_;
		boost::asio::ip::tcp::socket socket_;
		std::deque<std::vector<uint8_t>> send_que_;
		std::mutex send_mu;
		boost::asio::deadline_timer send_per_timer_;
		boost::asio::deadline_timer activate_timer_;
		boost::asio::deadline_timer destroy_deadline_timer_;
		uint64_t agent_id_;
		bool stop_flag_;
		CAgentPool* agentpool_;
	};
}//namespace servercore 
