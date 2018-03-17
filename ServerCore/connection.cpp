//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "stdafx.h"
#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "IHandler.hpp"

namespace servercore {

	connection::connection(boost::asio::io_context& io_context,
		boost::shared_ptr<IHandler > handler,
		connection_service& conn_service)
		: strand_(io_context),
		socket_(io_context),
		handler_(handler),
		conn_service_(conn_service)
	{
	}

	boost::asio::ip::tcp::socket& connection::socket()
	{
		return socket_;
	}

	void connection::start()
	{
		auto self = shared_from_this();
		stop_flag_ = !conn_service_.jion(self);
		readHead();
	}

	void connection::readHead()
	{
		buf_head_ = handler_->getBufHead();
		auto self = shared_from_this();
		auto stop_flag = stop_flag_;
		socket_.async_read_some(boost::asio::buffer(buf_head_),
			strand_.wrap([&,self](const boost::system::error_code& e,
				std::size_t bytes_transferred) {
			if (!e &&!stop_flag_)
			{
				handler_->readHeadHandler(buf_head_);
				readBody();
			}
			else
			conn_service_.leave(shared_from_this());
		}));
	}

	void connection::readBody()
	{
		buf_body_ = handler_->getBufBody();
		auto self = shared_from_this();
		auto stop_flag = stop_flag_;
		socket_.async_read_some(boost::asio::buffer(buf_body_),
			strand_.wrap([&, self](const boost::system::error_code& e,
				std::size_t bytes_transferred) {
			if (!e && !stop_flag_)
			{
				handler_->readBodyHandler(buf_body_);
				readHead();
			}else
				conn_service_.leave(shared_from_this());
		}));
	}
} // namespace servercore
