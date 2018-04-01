#pragma once

//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_CONNECTION_HPP
#define HTTP_SERVER3_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <set>
#include <deque>
#include <boost/thread/condition_variable.hpp>
#include "CLog.h"

namespace servercore {

	class connection;
	class connection;
	class server_tasks
	{
	public:
		void addTask(std::vector<std::string> parameters, int task_type)
		{
			boost::unique_lock<boost::mutex> mu(task_mu_);
			auto task = std::tie(parameters, task_type);
			tasks_.push_back(task);
			m_cond_.notify_one();
		}
		std::tuple<std::vector<std::string>, int> popTask()
		{
			boost::unique_lock<boost::mutex> mu(task_mu_);
			if (tasks_.empty())
			{
				//如果队列中没有任务，则等待互斥锁 
				m_cond_.wait(mu);//
			}
			auto task = tasks_.front();
			tasks_.pop_front();
			return task;
		}
	private:
		boost::condition_variable_any m_cond_;//条件变量
		boost::mutex task_mu_;
		std::deque<std::tuple<std::vector<std::string>, int>> tasks_;
	};
	class connection_handler
	{
	public:
		//待实现函数-------------------------------------------------

		//消息头处理
		virtual void readHeadHandler(std::vector<uint8_t> buf) = 0;
		//消息体处理
		virtual void readBodyHandler(std::vector<uint8_t> buf) = 0;
		//服务器任务处理
		virtual void serverTaskHandler(std::tuple<std::vector<std::string>, int> task) = 0;
		//返回 new 自身 
		//boost::shared_ptr<子类>
		virtual boost::shared_ptr<connection_handler> newHandler() = 0;
		//
		virtual void kickClientHandler() {};
		//待实现函数-------------------------------------------------

		connection_handler(size_t head_len, size_t body_len)
		{
			buf_head_.assign(head_len, 0);
			buf_body_.assign(body_len, 0);
		};
		virtual ~connection_handler()
		{
			//WLOG wlg;
			//BOOST_LOG_SEV(wlg, debug) << L"connection_Handler析构";


			BOOST_LOG_SEV(wlg::get(), debug) << L"connection_Handler析构";

		};
		std::vector<uint8_t> getBufHead()
		{
			return buf_head_;
		}
		std::vector<uint8_t> getBufBody()
		{
			return buf_body_;
		}
		void setAttribute(std::string key, std::string value)
		{
			attribute_[key] = value;
		}
		std::string getAttribute(std::string key)
		{
			return attribute_[key];
		}
		void registConnection(connection& conn)
		{
			connection_ = &conn;
		}
		void registServerTasks(server_tasks& server_ta)
		{
			server_tasks_ = &server_ta;
		}
	protected:
		void addServerTask(std::vector<std::string> parameters, int task_type)
		{
			server_tasks_->addTask(parameters,task_type);
		}
		/*std::tuple<std::vector<std::string>, int> popServerTask()
		{
			auto task = server_tasks_->popTask();
			return task;
		}*/
		std::vector<uint8_t> buf_head_;
		std::vector<uint8_t> buf_body_;
		std::map<std::string, std::string> attribute_;
		connection* connection_=nullptr;
		server_tasks* server_tasks_ = nullptr;
	};
	class connection_service
	{
	public:
		connection_service() :max_link_(100000) {};
		~connection_service() {};
		bool jion(boost::shared_ptr< connection> connection_ptr);
		
		void leave(boost::shared_ptr< connection> connection_ptr);
		void kickByAttribute(std::string key, std::string value);
		
		void setRestrictByAttribute(std::string key, std::string value, size_t num)
		{
			auto restrict = std::tie(key, value, num);
			restrict_attributes_.push_back(restrict);
		}
		void setMaxLink(size_t max)
		{
			max_link_ = max;
		}
	private:
		std::mutex set_mu_;
		std::set<boost::shared_ptr< connection>> connection_ptrs_;
		//限制登陆条件 key value numbers
		std::vector<std::tuple<std::string, std::string, size_t>> restrict_attributes_;
		//最大连接数
		size_t max_link_;
	};
	/// Represents a single connection from a client.
	class connection:
		public boost::enable_shared_from_this<connection>
	{
	public:
		/// Construct a connection with the given io_context.
		explicit connection(boost::asio::io_context& io_context,
			boost::shared_ptr<connection_handler > handler,
			connection_service& conn_service,
			server_tasks& server_task);
		~connection()
		{
			//WLOG wlg;
			//BOOST_LOG_SEV(wlg, debug) << L"客户端析构";
			//LOG lg;
			//BOOST_LOG_SEV(lg, debug) << "客户端析构";
			BOOST_LOG_SEV(wlg::get(), warning) << L"客户端析构";
			BOOST_LOG_SEV(wlg::get(), debug) << L"客户端析构";
		}
		virtual void stop();

		virtual std::string getAttribute(std::string key)
		{
			return handler_->getAttribute(key);
		}
		virtual void send(std::vector<uint8_t>& buf);
		void start();
		boost::asio::ip::tcp::socket& socket();
	protected:
		//停止标识
		bool stop_flag_;

		/// Get the socket associated with the connection.
		

		/// Start the first asynchronous operation for the connection.
		
		void readHead();
		void readBody();
		void doWrite();	
		virtual void addServerTask(std::vector<std::string> parameters, int task_type)
		{
			server_task_.addTask(parameters, task_type);
		}
		boost::asio::io_context::strand* getStrand()
		{
			return &strand_;
		}
		virtual boost::asio::io_context* getIo()
		{
			return &io_context_;
		}
	private:
		/// Strand to ensure the connection's handlers are not called concurrently.
		boost::asio::io_context::strand strand_;
		boost::asio::io_context &io_context_;
		/// Socket for the connection.
		boost::asio::ip::tcp::socket socket_;

		/// Buffer for incoming data.
		std::vector<uint8_t> buffer_;

		std::vector<uint8_t> buf_head_;
		std::vector<uint8_t> buf_body_;
		boost::shared_ptr<connection_handler > handler_;
		connection_service& conn_service_;
		server_tasks& server_task_;
		std::mutex send_mu_;
		std::deque<std::vector<uint8_t>> send_que_;
	};

	typedef boost::shared_ptr<connection> connection_ptr;


} // namespace http

#endif // HTTP_SERVER3_CONNECTION_HPP
