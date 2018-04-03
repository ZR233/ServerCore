#include "stdafx.h"
#include "IConnectionHandler.h"
#include "CLog.h"
#include "connection.h"
#include <boost\asio.hpp>
#include <boost\locale.hpp>
namespace servercore {
	IConnectionHandler::IConnectionHandler()
	{
		buf_head_.assign(4, 0);
		buf_body_.clear();
	};
	IConnectionHandler::~IConnectionHandler()
	{
		/*WLOG wlg;*/
		BOOST_LOG_SEV(wlg::get(), debug) << L"connection_Handler析构";
		//BOOST_LOG_SEV(wlg::get(), debug) << L"connection_Handler析构";

	};

	void IConnectionHandler::readHeadHandler(std::vector<uint8_t> buf)
	{
		uint32_t len = 0;
		memcpy_s(&len, 4, &(buf[0]), 4);
		len = ntohl(len);
		if (len>5000)
		{
			return;
		}
		buf_body_.assign(len, 0);
	}
	void IConnectionHandler::readBodyHandler(std::vector<uint8_t> buf)
	{
		if (buf.size() == 0)
		{
			return;
		}
		std::string str;
		str.assign(buf.begin(), buf.end());
		std::wstring ws = boost::locale::conv::to_utf<wchar_t>(str, "GBK");
		BOOST_LOG_SEV(wlg::get(), debug) << ws;
		std::string send_buf = "已收到";
		std::vector<uint8_t> send_buf_char;
		send_buf_char.assign(send_buf.begin(), send_buf.end());
		connection_->send(send_buf_char);
	}
	void IConnectionHandler::addServerTask(std::vector<std::string> parameters, int task_type) 
	{
		//server_tasks_->addTask(parameters, task_type);
	}
}