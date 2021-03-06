// client.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/asio.hpp>
#include <iostream>
#include <deque>
#include <vector>
#include <boost/thread.hpp>

std::mutex cout_mu;
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
		std::lock_guard<std::mutex> mu(cout_mu);
		std::cout << "连接成功:" << sock->remote_endpoint().address() << std::endl;
		do_write();
		do_read();
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
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		do_write();
	}
	void do_read()
	{
		read_buf_.assign(7, 0);
		sock->async_read_some(buffer(read_buf_),
			std::bind(&Client::read_handler, this, std::placeholders::_1));
	}
	void read_handler(const boost::system::error_code& ec)
	{
		if (ec)
		{
			return;
		}
		std::string str;
		str.assign(read_buf_.begin(), read_buf_.end());
		str = str.c_str();
		if (str.size()> 0)
		{
			std::lock_guard<std::mutex> mu(cout_mu);
			std::cout << str << std::endl;
		}

		//boost::this_thread::sleep(boost::posix_time::seconds(0));
		do_read();
	}
private:
	io_service m_io;
	ip::tcp::endpoint m_ep;
	enum { max_length = 1024 };
	std::vector<uint8_t> buf;
	sock_ptr sock;
	std::vector<uint8_t> read_buf_;
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
		Client cl_5;
		uint32_t b = 3;
		b = htonl(b);
		std::vector<uint8_t> buf3(4, 0);
		memcpy_s(&buf3[0], 4, &b, 4);
		uint8_t word2 = '1';
		for (auto i = 0; i<5; ++i)
		{
			std::vector<uint8_t> buf4;
			buf4.assign(buf3.begin(), buf3.end());
			buf4.push_back('5');
			buf4.push_back('5');
			buf4.push_back(++word2);
			cl_5.bufs_.push_back(buf4);
		}
		boost::thread t1([&]{ cl_1.run(); });
		boost::thread t2([&] { cl_2.run(); });
		boost::thread t3([&] { cl_3.run(); });
		boost::thread t4([&] { cl_4.run(); });
		boost::thread t5([&] { cl_5.run(); });
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	getchar();
	return 0;
}

