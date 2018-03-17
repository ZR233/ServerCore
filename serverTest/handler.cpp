#include "stdafx.h"
#include "handler.h"


handler::handler():
	IHandler(4,1)
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
	buf_body_.assign(len, 0);
}

void handler::readBodyHandler(std::vector<uint8_t> buf)
{
	std::string str;
	str.assign(buf.begin(), buf.end());
	std::lock_guard<std::mutex> mu(cout_mu);
	std::cout << str << std::endl;
}
