#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost\shared_ptr.hpp>

namespace servercore {
	class CIoThread
	{
	public:
		CIoThread();
		~CIoThread();
		void run();
		boost::asio::io_context* getIo()
		{
			return &io_;
		}
		void stop()
		{
			io_.stop();
		}
	private:
		boost::asio::io_context io_;
		boost::shared_ptr<boost::thread> io_thread_ptr;
	};

}