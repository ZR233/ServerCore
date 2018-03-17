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

namespace servercore {

	connection::connection(boost::asio::io_context& io_context,
		boost::shared_ptr<connection_handler > handler,
		connection_service& conn_service,
		server_tasks& server_task):
		strand_(io_context),
		socket_(io_context),
		handler_(handler),
		conn_service_(conn_service),
		io_context_(io_context),
		server_task_(server_task)
	{
		handler_->registConnection(*this);
		handler_->registServerTasks(server_task);
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
		if (stop_flag_)
			return;
		buf_head_ = handler_->getBufHead();
		auto self = shared_from_this();
		socket_.async_read_some(boost::asio::buffer(buf_head_),
			/*strand_.wrap(*/[&,self](const boost::system::error_code& e,
				std::size_t bytes_transferred) {
			if (!e &&!stop_flag_)
			{
				handler_->readHeadHandler(buf_head_);
				readBody();
			}
			else
			conn_service_.leave(shared_from_this());
		})/*)*/;
	}

	void connection::readBody()
	{
		if (stop_flag_)
			return;
		buf_body_ = handler_->getBufBody();
		auto self = shared_from_this();
		socket_.async_read_some(boost::asio::buffer(buf_body_),
			/*strand_.wrap(*/[&, self](const boost::system::error_code& e,
				std::size_t bytes_transferred) {
			if (!e && !stop_flag_)
			{
				handler_->readBodyHandler(buf_body_);
				readHead();
			}else
				conn_service_.leave(shared_from_this());
		})/*)*/;
	}
	void connection::doWrite()
	{
		if (stop_flag_)
			return;
		const auto self(shared_from_this());
		socket_.async_write_some(boost::asio::buffer(send_que_.front()),
			/*strand_.wrap(*/[&,self](boost::system::error_code ec, std::size_t /*length*/)
		{

			if (!ec && !stop_flag_)
			{
				send_que_.pop_front();
				if (!send_que_.empty())
				{
					doWrite();
				}
			}
			else
				conn_service_.leave(shared_from_this());
		})/*)*/;
	}
	//∑¢ÀÕ–≈œ¢
	void connection::send(std::vector<uint8_t>& buf)
	{
		std::lock_guard<std::mutex> mu(send_mu_);
		const auto write_in_progress = !send_que_.empty();
		send_que_.push_back(buf);
		if (!write_in_progress)
		{
			doWrite();
		}
	}
} // namespace servercore
