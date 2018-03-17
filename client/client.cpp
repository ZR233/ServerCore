// client.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/asio.hpp>
#include <iostream>
#include <deque>
#include <vector>
using namespace boost::asio;
class Client
{
	typedef ip::tcp::socket socket_type;
	typedef std::shared_ptr<socket_type> sock_ptr;
public:
	Client() :m_ep(ip::address::from_string("127.0.0.1"), 8801),
		sock(new socket_type(m_io))
	{
		
	}
	void run()
	{
		start();
		m_io.run();
	}
	std::deque<std::vector<uint8_t>> bufs_;
private:
	void start()
	{
		sock->async_connect(m_ep, std::bind(&Client::connect_handler, this, std::placeholders::_1, sock));
	}
	void connect_handler(const boost::system::error_code& ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		std::cout << "连接成功:" << sock->remote_endpoint().address() << std::endl;
		do_write();
	}
	void do_write()
	{
		buf = bufs_.front();
		
		bufs_.pop_front();
		
		sock->async_write_some(buffer(buf),
			std::bind(&Client::write_handler, this, std::placeholders::_1));
	}
	void write_handler(const boost::system::error_code& ec)
	{
		if (ec || bufs_.size() ==0)
		{
			return;
		}
		do_write();
	}

private:
	io_service m_io;
	ip::tcp::endpoint m_ep;
	enum { max_length = 1024 };
	std::vector<uint8_t> buf;
	sock_ptr sock;
};
int main()
{
	try
	{
		std::cout << "client start." << std::endl;
		Client cl_1;
		Client cl_2;
		Client cl_3;
		Client cl_4;
		uint32_t a = 2;
		a = htonl(a);
		std::vector<uint8_t> buf(4, 0);
		memcpy_s(&buf[0], 4, &a, 4);
		uint8_t word = '1';
		for (auto i =0;i<5;++i)
		{
			std::vector<uint8_t> buf2;
			buf2.assign(buf.begin(), buf.end());
			buf2.push_back('1');
			buf2.push_back(++word);
			cl_1.bufs_.push_back(buf2);
			buf2[4] = '2';
			cl_2.bufs_.push_back(buf2);
			buf2[4] = '3';
			cl_3.bufs_.push_back(buf2);
			buf2[4] = '4';
			cl_4.bufs_.push_back(buf2);
		}
		
		cl_1.run();
		cl_2.run();
		cl_3.run();
		cl_4.run();
	
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	getchar();
	return 0;
}

