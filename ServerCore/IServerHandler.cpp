#include "stdafx.h"
#include "IServerHandler.h"
#include "CLog.h"
#include "connection.h"
#include <boost\asio.hpp>
#include <boost\locale.hpp>
namespace servercore {
	IServerHandler::IServerHandler():
		head_len_(4)
	{
		buf_head_.assign(head_len_, 0);
		buf_body_.clear();
	};
	IServerHandler::~IServerHandler()
	{
		/*WLOG wlg;*/
		BOOST_LOG_SEV(wlg::get(), debug) << L"Server_Handler析构";
		//BOOST_LOG_SEV(wlg::get(), debug) << L"connection_Handler析构";

	};

	void IServerHandler::readHeadHandler(std::vector<uint8_t> buf)
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
	void IServerHandler::readBodyHandler(std::vector<uint8_t> buf)
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
		uint32_t len = htonl(send_buf_char.size());
		std::vector<uint8_t> send_len(4, 0);
		memcpy_s(&(send_len[0]), 4, &len, 4);
		send_buf_char.insert(send_buf_char.begin(), send_len.begin(), send_len.end());
		connection_->send(send_buf_char);
	}
	std::shared_ptr<IServerHandler> IServerHandler::getNewInstance()
	{
		std::shared_ptr<IServerHandler> hand(new IServerHandler());
		BOOST_LOG_SEV(wlg::get(), warning) << L"未重写IServerHandler子类的getNewInstance方法";
		return hand;
	}
	void IServerHandler::addServerTask(CTask task) 
	{
		server_tasks_->addTask(task);
	}
}