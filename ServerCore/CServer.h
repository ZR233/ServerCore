#pragma once

// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//



#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.h"
#include "io_context_pool.hpp"
#include "externvar.hpp"
#include "IHandlers.h"
#include "CLog.h"
#include "IModules.h"
#include "CTaskList.h"



namespace servercore {
	class CPlatform;
	// The top-level class of the server.
	class CServer:
		private boost::noncopyable,
		public IServerModule
	{
	public:
		// Construct the server to listen on the specified TCP address and port, and
		// serve up files from the given directory.
		explicit CServer(
			std::string address, 
			std::string port,
			io_context_pool& io_context_pool,
			CPlatform& plat,
			IServerHandler& handler);

		// Run the server's io_context loop.
		void run();

		void join();
		void setMaxLink(size_t max);
		void setRestrictByAttribute(std::string key, std::string value, size_t num);
		void kickByAttribute(std::string key, std::string value);

	private:

		// Initiate an asynchronous accept operation.
		void start_accept();

		// Handle completion of an asynchronous accept operation.
		void handle_accept(const boost::system::error_code& e);

		// Handle a request to stop the server.
		void handle_stop();

		IServerHandler& handler_;

		// The pool of io_context objects used to perform asynchronous operations.
		io_context_pool& io_context_pool_;

		// The io_context used to perform asynchronous operations.
		boost::asio::io_context io_context_;

		// The signal_set is used to register for process termination notifications.
		boost::asio::signal_set signals_;

		// Acceptor used to listen for incoming connections.
		boost::asio::ip::tcp::acceptor acceptor_;

		// The next connection to be accepted.
		connection_ptr new_connection_;

		connection_service connection_service_;

		CPlatform& plat_;

	};//class server

} // namespace servercore
