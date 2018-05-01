#pragma once
#include <memory>
#include <vector>
#include <boost\asio.hpp>
namespace servercore {

	class CAgent;
	class IAgentHandlers
	{
	public:
		IAgentHandlers();
		virtual ~IAgentHandlers();
		//需返回指向自身的智能指针
		virtual std::shared_ptr<IAgentHandlers> getNewInstance() = 0;
		virtual void dealRecvData(std::vector<uint8_t> data_buf);
		virtual void connectFail(CAgent* this_agent);

		void setDefaultIo(boost::asio::io_context& io);
	private:
		boost::asio::io_context* default_io_ptr_;
	};
}//namespace servercore
