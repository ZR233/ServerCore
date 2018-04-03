#include "stdafx.h"
#include "handler.h"
#include <iostream>
#include <boost\asio.hpp>
handler::handler()
{
}


handler::~handler()
{
}

void handler::readHeadHandler(std::vector<uint8_t> buf)
{
	uint32_t len = 0;
	memcpy_s(&len, 4, &(buf[0]), 4);
	len = ntohl(len);
	if (len>1000)
	{
		return;
	}
	buf_body_.assign(len, 0);
}

void handler::readBodyHandler(std::vector<uint8_t> buf)
{
	if (buf.size() == 0)
	{
		return;
	}
	std::string str;
	str.assign(buf.begin(), buf.end());
	char a = buf[0];
	std::string value;
	value = a;
	setAttribute("appid", value);
	std::lock_guard<std::mutex> mu(cout_mu);
	std::cout << str << std::endl;
	std::string send_buf = "已收到";
	std::vector<uint8_t> send_buf_char;
	send_buf_char.assign(send_buf.begin(), send_buf.end());
	connection_->send(send_buf_char);
}
