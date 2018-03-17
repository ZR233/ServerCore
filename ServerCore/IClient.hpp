#pragma once
#include "IClientBase.hpp"
#include "ClientRoom.hpp"
#include "boost/asio.hpp"
#include "IHandleQueue.hpp"
#include "boost/thread/thread.hpp"

class IClient:
	public virtual IClientBase,
	public virtual std::enable_shared_from_this<IClient>
{
public:
	typedef boost::asio::ip::tcp::socket boostSocket;
	IClient(boostSocket socket,
		ClientRoom& room,
		IHandleQueue &handle_que
	) :
		socket_(std::move(socket)),
		room_(room),
		handle_que_(handle_que)
	{};
	virtual ~IClient()
	{
		read_thread_.interrupt();
	};
	virtual void sendToServer(std::vector<uint8_t>& buf) = 0;
	virtual void sendToClient(std::vector<uint8_t>& buf) = 0;
	virtual void start()
	{
		const auto self(shared_from_this());
		room_.join(self);
		read_thread_ = std::move(boost::thread(boost::bind(&IClient::do_read,this)));

	}
protected:
	void do_read()
	{
		while (true)
		{
			std::vector<uint8_t> msg_len(4, 0);
			socket_.read_some(boost::asio::buffer(msg_len));
		}
	}
	boostSocket socket_;
	ClientRoom &room_;
	IHandleQueue &handle_que_;

	//收到的消息缓存区-----

	std::mutex rec_que_mu_;
	std::deque<std::vector<uint8_t>> rcv_que_;
	boost::thread read_thread_;
};
