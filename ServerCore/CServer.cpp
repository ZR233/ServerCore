#include "stdafx.h"
#include "CServer.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>


namespace servercore {
	CServer::CServer(
		std::string address,
		std::string port,
		io_context_pool& io_context_pool,
		CTaskList& task_list,
		IServerHandler& handler
	) :
		io_context_pool_(io_context_pool),
		signals_(io_context_pool_.get_io_context()),
		acceptor_(io_context_pool_.get_io_context()),
		new_connection_(),
		server_tasks_(task_list),
		handler_(handler)
	{
		// Register to handle the signals that indicate when the server should exit.
		// It is safe to register for the same signal multiple times in a program,
		// provided all registration for the specified signal is made through Asio.
		signals_.add(SIGINT);
		signals_.add(SIGTERM);
#if defined(SIGQUIT)
		signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
		signals_.async_wait(boost::bind(&CServer::handle_stop, this));


		// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		boost::asio::ip::tcp::resolver resolver(io_context_);
		boost::asio::ip::tcp::endpoint endpoint =
			*resolver.resolve(address, port).begin();
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();

		start_accept();
	}

	void CServer::run()
	{

		//// Create a pool of threads to run all of the io_contexts.
		//std::vector<boost::shared_ptr<boost::thread> > threads;
		//for (std::size_t i = 0; i < thread_pool_size_; ++i)
		//{
		//	boost::shared_ptr<boost::thread> thread(new boost::thread(
		//		boost::bind(&boost::asio::io_context::run, &io_context_)));
		//	threads.push_back(thread);
		//}

		//// Wait for all threads in the pool to exit.
		//for (std::size_t i = 0; i < threads.size(); ++i)
		//	threads[i]->join();

		//服务器任务处理
		//boost::thread serverTaskDealThread([&] {
		//	while (true)
		//	{
		//		try
		//		{
		//			handler_.serverTaskHandler(server_tasks_.popTask());
		//		}
		//		catch (std::exception &e)
		//		{

		//			//boost::lock_guard<boost::mutex> mu(externvar::cout_mu);
		//			//std::cout << e.what()<<std::endl;
		//		}
		//	}
		//});
		//io_context_pool_.registHandler(handler_);
		io_context_pool_.run();
		//serverTaskDealThread.join();
	}
	void CServer::join()
	{
		io_context_pool_.join();
	}

	void CServer::setRestrictByAttribute(std::string key, std::string value, size_t num)
	{
		connection_service_.setRestrictByAttribute(key, value, num);
	}

	void CServer::kickByAttribute(std::string key, std::string value)
	{
		connection_service_.kickByAttribute(key, value);
	}
	void CServer::setMaxLink(size_t max)
	{
		connection_service_.setMaxLink(max);
	}
	void CServer::start_accept()
	{
		auto connection_hander = handler_.getNewInstance();

		new_connection_.reset(new connection(
			io_context_pool_.get_io_context(), 
			connection_hander, connection_service_, server_tasks_));
		acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&CServer::handle_accept, this,
				boost::asio::placeholders::error));
	}

	void CServer::handle_accept(const boost::system::error_code& e)
	{
		if (!e)
		{
			new_connection_->start();
		}

		start_accept();
	}

	void CServer::handle_stop()
	{
		io_context_.stop();
	}

} // namespace servercore

