#include "stdafx.h"
#include "CIoThread.h"

namespace servercore {
	CIoThread::CIoThread(boost::shared_ptr< ITaskHandler> taskHandler):
		task_timer_(io_),
		taskHandler_(taskHandler)
	{
		task_timer_.expires_from_now(boost::posix_time::seconds(1));
		taskDealer();
	}
	void  CIoThread::taskDealer()
	{

		task_timer_.async_wait([this](const boost::system::error_code& ec) {
			if (ec)
				return;
			if (task_timer_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
			{
				for (auto i = 0; i < tasks_.size(); ++i)
				{
					taskHandler_->taskHandler(tasks_.front());
					tasks_.pop_front();
				}
				task_timer_.expires_at(boost::posix_time::pos_infin);
				taskDealer();
			}
		});
	}
	CIoThread::~CIoThread()
	{
		io_.stop();
	}
	void CIoThread::run()
	{
		io_thread_ptr = boost::make_shared<boost::thread>(boost::thread([this] {
			io_.run();
		}));
	}
	void CIoThread::join()
	{
		io_thread_ptr->join();
	}
	void CIoThread::addTask(std::vector<std::string>& parameters, int task_type)
	{
		auto task = std::tie(parameters, task_type);
		tasks_.push_back(task);
		task_timer_.expires_from_now(boost::posix_time::milliseconds(10));
	}
}