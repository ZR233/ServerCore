#pragma once
#include "IAgentHandlers.h"
#include <boost\asio.hpp>
#include "CTaskList.h"
#include <deque>
#include <mutex>


namespace servercore {
	class CAgentPool;
	class CPlatform;
	class CAgent:
		public std::enable_shared_from_this<CAgent>
	{
	public:
		CAgent(
			uint64_t agent_id,
			CAgentPool* agent_pool, 
			IAgentHandlers& agent_handlers, 
			boost::asio::io_context& io,
			CPlatform& plat
		);
		~CAgent();
		void connect(std::string ip, std::string port);
		void send(std::vector<uint8_t> data_buf);
		void sendNow(std::vector<uint8_t> data_buf);

		void addTask(const char * task_dealer_name, CTask task);
		void disconnect();
		void destroy();
		void setAgentID(uint64_t id)
		{
			agent_id_ = id;
		}
		uint64_t getAgentID()
		{
			return agent_id_;
		}
	private:
		boost::asio::io_context& io_;
		boost::asio::io_context::strand strand_;
		std::shared_ptr<IAgentHandlers> agent_handlers_ptr_;
		CPlatform& plat_;
		boost::asio::ip::tcp::socket socket_;
		std::deque<std::vector<uint8_t>> send_que_;
		std::mutex send_mu;
		boost::asio::deadline_timer send_per_timer_;
		boost::asio::deadline_timer activate_timer_;
		boost::asio::deadline_timer close_deadline_timer_;
		boost::asio::deadline_timer destroy_deadline_timer_;
		boost::asio::deadline_timer reconnect_timer_;
		uint64_t agent_id_;
		bool stop_flag_;
		CAgentPool* agentpool_;
		std::string ip_;
		std::string port_;
	};
}//namespace servercore 
