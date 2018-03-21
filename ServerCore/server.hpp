#pragma once

// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_SERVER_HPP
#define HTTP_SERVER3_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "io_context_pool.hpp"


namespace servercore {

	/// The top-level class of the HTTP server.
	class server
		: private boost::noncopyable
	{
	public:
		/// Construct the server to listen on the specified TCP address and port, and
		/// serve up files from the given directory.
		explicit server(const std::string& address, const std::string& port,
			const std::string& doc_root, std::size_t thread_pool_size,
			connection_handler& handler);

		/// Run the server's io_context loop.
		void run();
		void setMaxLink(size_t max);
		void setRestrictByAttribute(std::string key, std::string value, size_t num);
		void kickByAttribute(std::string key, std::string value);

	private:

		/// Initiate an asynchronous accept operation.
		void start_accept();

		void start_deal_task();
		/// Handle completion of an asynchronous accept operation.
		void handle_accept(const boost::system::error_code& e);

		/// Handle a request to stop the server.
		void handle_stop();



		/// The number of threads that will call io_context::run().
		std::size_t thread_pool_size_;

		/// The pool of io_context objects used to perform asynchronous operations.
		io_context_pool io_context_pool_;

		/// The io_context used to perform asynchronous operations.
		boost::asio::io_context io_context_;

		/// The signal_set is used to register for process termination notifications.
		boost::asio::signal_set signals_;

		/// Acceptor used to listen for incoming connections.
		boost::asio::ip::tcp::acceptor acceptor_;

		/// The next connection to be accepted.
		connection_ptr new_connection_;

		connection_handler& handler_;

		connection_service connection_service_;

		server_tasks server_tasks_;


	};

} // namespace servercore


#endif // HTTP_SERVER3_SERVER_HPP
