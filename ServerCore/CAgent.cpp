#include "stdafx.h"
#include "CAgent.h"
#include <boost/asio/spawn.hpp>
#include "CLog.h"
#include <vector>
#include "CAgentPool.h"
#include <iostream>
#include "CPlatform.h"

namespace servercore {
	CAgent::CAgent(
		uint64_t agent_id,
		CAgentPool* agent_pool, 
		IAgentHandlers & agent_handlers, 
		boost::asio::io_context& io,
		CPlatform& plat
	):
		agent_id_(agent_id),
		agent_handlers_ptr_(agent_handlers.getNewInstance()),
		io_(io),
		strand_(io),
		plat_(plat),
		socket_(io),
		send_per_timer_(io),
		activate_timer_(io),
		close_deadline_timer_(io),
		reconnect_timer_(io),
		stop_flag_(false),
		agentpool_(agent_pool),
		destroy_deadline_timer_(io)
	{
		agent_handlers_ptr_->setDefaultIo(io);
		agent_handlers_ptr_->setThisAgentPtr(this);
	}

	CAgent::~CAgent()
	{
		BOOST_LOG_SEV(lg::get(), debug) << "Agent 退出";
	}
	void CAgent::connect(std::string ip, std::string port)
	{
		ip_ = ip;
		port_ = port;
		auto self(shared_from_this());
		//std::cout << self.use_count() << std::endl;
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
				reconnect_timer_.expires_from_now(boost::posix_time::milliseconds(5000));
				reconnect_timer_.async_wait([this](boost::system::error_code ec) {
					if (stop_flag_)
						return;
					if (reconnect_timer_.expires_from_now() <= boost::posix_time::seconds(0))
					{
						connect(ip_, port_);
						reconnect_timer_.expires_at(boost::posix_time::pos_infin);
					}
				});
			}
			auto login_buf = agent_handlers_ptr_->loginDeal();
			sendNow(login_buf);

			//若一定时间无数据，则关闭socket  的模式是否开启
			if (agent_handlers_ptr_->noDataCloseSocket())
				boost::asio::spawn(strand_,
					[this, self](boost::asio::yield_context yield)
				{
					while (socket_.is_open())
					{
						close_deadline_timer_.expires_from_now(boost::posix_time::milliseconds(
							agent_handlers_ptr_->getSocketCloseSeadlineMillsec()
						));
						boost::system::error_code ec;
						close_deadline_timer_.async_wait(yield[ec]);
						if (stop_flag_)
							break;
						if (close_deadline_timer_.expires_from_now() <= boost::posix_time::seconds(0))
						{
							BOOST_LOG_SEV(lg::get(), warning) << "服务器超时没有响应，断开连接";
							disconnect();
						}
					}

				});
			//若超时没有回应，则进行心跳检测
			boost::asio::spawn(strand_,
				[this, self](boost::asio::yield_context yield)
			{
				while (socket_.is_open())
				{
					activate_timer_.expires_from_now(boost::posix_time::milliseconds(
						agent_handlers_ptr_->getActiveTestTimeMillsec()
					));
					boost::system::error_code ec;
					activate_timer_.async_wait(yield[ec]);
					if (stop_flag_)
					{
						break;
					}
					if (activate_timer_.expires_from_now() <= boost::posix_time::seconds(0))
					{
						auto act_buf = agent_handlers_ptr_->activeTest();
						if (!act_buf.empty())
							send(act_buf);
					}
				}
			});

			//定时从send_que读取发送
			boost::asio::spawn(strand_,
				[this, self](boost::asio::yield_context yield)
			{
				while (true)
				{
					if (!socket_.is_open()) {
						connect(ip_, port_);
						break;
					}
					send_per_timer_.expires_from_now(boost::posix_time::milliseconds(20));
					boost::system::error_code ec;
					send_per_timer_.async_wait(yield[ec]);
					if (stop_flag_)
					{
						break;
					}
					if (send_per_timer_.expires_from_now() <= boost::posix_time::seconds(0))
					{
						if (send_que_.empty())
							continue;
						socket_.async_write_some(boost::asio::buffer(send_que_.front()), yield[ec]);
						if (stop_flag_)
							return;
						if (ec)
						{
							socket_.close();
						}

						std::lock_guard<std::mutex> g(send_mu);
						send_que_.pop_front();
					}
				}

			});
			//持续接收数据
			boost::asio::spawn(strand_,
				[this, self](boost::asio::yield_context yield)
			{
				std::vector<uint8_t> head_buf;
				std::vector<uint8_t> data_buf;
				while (socket_.is_open())
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
						{
							break;
						}
						//收到数据则重置心跳检测计时器
						activate_timer_.expires_from_now(
							boost::posix_time::milliseconds(
								agent_handlers_ptr_->getActiveTestTimeMillsec()));
						//收到数据则重置关闭socket计时器
						close_deadline_timer_.expires_from_now(
							boost::posix_time::milliseconds(
								agent_handlers_ptr_->getSocketCloseSeadlineMillsec()));
						uint32_t len;
						memcpy_s(&len, 4, &(head_buf[0]), 4);
						len = ntohl(len);
						data_buf.assign(len, 0);
						//接收数据体
						socket_.async_read_some(boost::asio::buffer(data_buf), yield[ec]);
						if (ec)
						{
							break;
						}
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
	void CAgent::sendNow(std::vector<uint8_t> data_buf)
	{
		std::lock_guard<std::mutex> g(send_mu);
		send_que_.push_front(data_buf);
	}
	void CAgent::addTask(const char * task_dealer_name, CTask task)
	{
		plat_.useTaskDealer(task_dealer_name)->addTask(task);
	}

	void CAgent::disconnect()
	{
		socket_.close();
		activate_timer_.expires_at(boost::posix_time::pos_infin);
		close_deadline_timer_.expires_at(boost::posix_time::pos_infin);
		send_per_timer_.expires_at(boost::posix_time::pos_infin);
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
			send_per_timer_.cancel();
			activate_timer_.cancel();
			destroy_deadline_timer_.cancel();

		});
		disconnect();

		for (auto agent : agentpool_->agent_map_)
		{
			if (agent.second->getAgentID() == agent_id_)
			{
				agentpool_->agent_map_.erase(agent.first);
				break;
			}
		}
	}

}//namespace servercore