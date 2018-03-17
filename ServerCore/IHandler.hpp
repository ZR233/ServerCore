#pragma once
#include "boost/asio.hpp"
#include "connection.hpp"
#include <iostream>

//class IHandler:
//	public servercore::IHandler
//{
//public:
//	IHandler(){};
//	virtual ~IHandler() {};
//	virtual void doReadBody() final
//	{
//		connection_ptr_->readBody();
//	}
//	virtual void doReadHead() final
//	{
//		connection_ptr_->readHead();
//	}
//	virtual void addToConnection(servercore::connection *connection_ptr)
//	{
//		connection_ptr_ = connection_ptr;
//	};
//private:
//	servercore::connection *connection_ptr_;
//};
