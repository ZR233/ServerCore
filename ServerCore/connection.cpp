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
#include "connection.h"
#include <vector>
#include <boost/bind.hpp>

namespace servercore {

	void connection_service::kickByAttribute(std::string key, std::string value)
	{
		std::lock_guard<std::mutex> mu(set_mu_);
		auto iter = connection_ptrs_.begin();
		for (; iter != connection_ptrs_.end();)
		{
			if ((*iter)->getAttribute(key) == value)
			{
				(*iter)->stop();
				iter = connection_ptrs_.erase(iter);
			}
			else
				++iter;
		}
	}
	bool connection_service::jion(boost::shared_ptr< connection> connection_ptr)
	{
		std::lock_guard<std::mutex> mu(set_mu_);
		/*for (auto restrict_attribute : restrict_attributes_)
		{
		size_t num = 0;
		for (auto connection_ptr : connection_ptrs_)
		{
		std::string key, value;
		size_t num_max;
		std::tie(key, value, num_max) = restrict_attribute;
		if (connection_ptr->getAttribute(key) == value)
		{
		++num;
		if (num >= num_max)
		{
		connection_ptr->stop();
		return false;
		}
		}
		}
		}*/

		if (connection_ptrs_.size() < max_link_)
		{
			connection_ptrs_.insert(connection_ptr);
			return true;
		}
		else {
			connection_ptr->stop();
			return false;
		}
	}
	void connection_service::leave(boost::shared_ptr< connection> connection_ptr)
	{
		std::lock_guard<std::mutex> mu(set_mu_);
		connection_ptrs_.erase(connection_ptr);
	}
	connection::connection(boost::asio::io_context& io_context,
		boost::shared_ptr<IConnectionHandler> handler,
		connection_service& conn_service,
		server_tasks& server_task):
		strand_(io_context),
		socket_(io_context),
		handler_(handler),
		conn_service_(conn_service),
		io_context_(io_context),
		server_task_(server_task),
		stop_flag_(false)
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
	//发送信息
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
	void connection::stop()
	{
		handler_->kickClientHandler();
		stop_flag_ = true;
		socket_.close();
	}
	void connection::addServerTask(std::vector<std::string> parameters, int task_type)
	{
		server_task_.addTask(parameters, task_type);
	}
} // namespace servercore
