#include "stdafx.h"
#include "CAgent.h"
#include <boost/asio/spawn.hpp>
#include "CLog.h"
#include <vector>
#include "CAgentPool.h"

namespace servercore {
	CAgent::CAgent(
		uint64_t agent_id,
		CAgentPool* agent_pool, 
		IAgentHandlers & agent_handlers, 
		boost::asio::io_context& io,
		CTaskList& task_list
	):
		agent_id_(agent_id),
		agent_handlers_ptr_(agent_handlers.getNewInstance()),
		io_(io),
		strand_(io),
		task_list_(task_list),
		socket_(io),
		send_per_timer_(io),
		activate_timer_(io),
		stop_flag_(false),
		agentpool_(agent_pool),
		destroy_deadline_timer_(io)
	{
		agent_handlers_ptr_->setDefaultIo(io);
		agent_handlers_ptr_->setThisAgentPtr(this);
	}

	CAgent::~CAgent()
	{
	}
	void CAgent::connect(std::string ip, std::string port)
	{
		auto self(shared_from_this());
		boost::asio::ip::tcp::resolver resolver(io_);
		auto endpoints = resolver.resolve(ip, port);
		//连接到服务器
		boost::asio::async_connect(socket_, endpoints, 
			[this, self](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ed) {
			if (stop_flag_)
				return;
			if (ec)
			{
				socket_.close();
				BOOST_LOG_SEV(lg::get(), debug) << "连接服务器失败\nIP:"<< ed.address()<<"PORT:"
					<<std::to_string(ed.port());
				agent_handlers_ptr_->connectFail();
			}

			//发送
			do_write();
			//定时从send_que读取发送
			send_per_timer_.async_wait([this,self](const boost::system::error_code &ec) {
				if (ec || stop_flag_)
					return;
				if (send_per_timer_.expires_from_now() <= boost::posix_time::seconds(0))
				{
					do_write();
					send_per_timer_.expires_from_now(boost::posix_time::milliseconds(20));
				}
			});

			boost::asio::spawn(strand_,
				[this, self](boost::asio::yield_context yield)
			{
				/*while (socket_.is_open())
				{
					boost::system::error_code ignored_ec;
					activate_timer_.async_wait(yield[ignored_ec]);
					if (activate_timer_.expires_from_now() <= boost::posix_time::seconds(0))
						socket_.close();
				}*/
				std::vector<uint8_t> head_buf;
				std::vector<uint8_t> data_buf;
				while (true)
				{
					try
					{
						if (stop_flag_)
							return;
						//读数据头得到数据总长度
						head_buf.assign(4, 0);
						boost::system::error_code ec;
						socket_.async_read_some(boost::asio::buffer(head_buf), yield[ec]);
						if (ec)
							return;
						uint32_t len;
						memcpy_s(&len, 4, &(head_buf[0]), 4);
						len = ntohl(len);
						data_buf.assign(len, 0);
						//接收数据体
						socket_.async_read_some(boost::asio::buffer(data_buf), yield[ec]);
						if (ec)
							return;
						data_buf.insert(data_buf.begin(), head_buf.begin(), head_buf.end());
						agent_handlers_ptr_->dealRecvData(data_buf);
					}
					catch (const std::exception& e)
					{
						BOOST_LOG_SEV(lg::get(), debug) << e.what();
						socket_.close();
					}
				}//接受循环
			});//接收swapn
		});//连接
	}

	void CAgent::send(std::vector<uint8_t> data_buf)
	{
		std::lock_guard<std::mutex> g(send_mu);
		send_que_.push_back(data_buf);
	}

	void CAgent::disconnect()
	{

	}

	void CAgent::destroy()
	{
		auto self(shared_from_this());
		destroy_deadline_timer_.expires_from_now(boost::posix_time::milliseconds(2000));
		destroy_deadline_timer_.async_wait([this, self](const boost::system::error_code &ec) {
			if (ec)
				return;
			stop_flag_ = true;
			socket_.close();
		});
		disconnect();
		agentpool_->agent_map_.erase(agentpool_->agent_map_.find(agent_id_));
	}

	void CAgent::do_write()
	{
		if (stop_flag_)
			return;
		std::lock_guard<std::mutex> g(send_mu);
		if (send_que_.empty())
			return;
		auto self(shared_from_this());
		boost::asio::async_write(socket_,
			boost::asio::buffer(send_que_.front()),
			[this,self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (stop_flag_)
				return;
			if (!ec)
			{
				std::lock_guard<std::mutex> g(send_mu);
				send_que_.pop_front();
				if (!send_que_.empty())
				{
					g.~lock_guard();
					do_write();
				}
			}
			else
			{
				socket_.close();
			}
		});
	}

}//namespace servercore