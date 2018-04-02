#include "stdafx.h"
#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>


namespace servercore {

	server::server(const std::string& address, const std::string& port,
		const std::string& doc_root, std::size_t thread_pool_size,
		IConnectionHandler& handler):
		thread_pool_size_(thread_pool_size),
		io_context_pool_(thread_pool_size),
		signals_(io_context_pool_.get_io_context()),
		acceptor_(io_context_pool_.get_io_context()),
		new_connection_(),
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
		signals_.async_wait(boost::bind(&server::handle_stop, this));
		std::locale::global(std::locale(""));
		logIni();

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

	void server::run()
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
		boost::thread serverTaskDealThread([&] {
			while (true)
			{
				try
				{
					handler_.serverTaskHandler(server_tasks_.popTask());
				}
				catch (std::exception &e)
				{

					//boost::lock_guard<boost::mutex> mu(externvar::cout_mu);
					//std::cout << e.what()<<std::endl;
				}
			}
		});
		io_context_pool_.run();
		serverTaskDealThread.join();
	}

	void server::setRestrictByAttribute(std::string key, std::string value, size_t num)
	{
		connection_service_.setRestrictByAttribute(key, value, num);
	}

	void server::kickByAttribute(std::string key, std::string value)
	{
		connection_service_.kickByAttribute(key, value);
	}
	void server::setMaxLink(size_t max)
	{
		connection_service_.setMaxLink(max);
	}
	void server::start_accept()
	{
		auto hander = handler_.newHandler();
		//new_connection_.reset(new connection(io_context_, 
		//	hander,connection_service_, server_tasks_));

		//acceptor_.async_accept(new_connection_->socket(),
		//	boost::bind(&server::handle_accept, this,
		//		boost::asio::placeholders::error));

		new_connection_.reset(new connection(
			io_context_pool_.get_io_context(), 
			hander, connection_service_, server_tasks_));
		acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&server::handle_accept, this,
				boost::asio::placeholders::error));
	}

	void server::start_deal_task()
	{

	}

	void server::handle_accept(const boost::system::error_code& e)
	{
		if (!e)
		{
			//std::lock_guard<std::mutex> mu(cout_mu);
			std::cout << "客户端连接" << std::endl;
			new_connection_->start();
		}

		start_accept();
	}

	void server::handle_stop()
	{
		io_context_.stop();
	}

} // namespace servercore

