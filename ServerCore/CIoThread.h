#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost\shared_ptr.hpp>
#include "ITaskHandler.h"
#include <deque>
#include <tuple>
#include <vector>
namespace servercore {
	class CIoThread
	{
	public:
		CIoThread(boost::shared_ptr< ITaskHandler> taskHandler);
		~CIoThread();
		void run();
		void join();
		boost::asio::io_context* getIo()
		{
			return &io_;
		}
		void stop()
		{
			io_.stop();
		}
		void addTask(std::vector<std::string>& parameters, int task_type);
	private:
		void  taskDealer();
		boost::asio::io_context io_;
		boost::shared_ptr<boost::thread> io_thread_ptr;
		std::deque<std::tuple<std::vector<std::string>, int>> tasks_;
		boost::asio::deadline_timer task_timer_;
		boost::shared_ptr< ITaskHandler> taskHandler_;
	};

}