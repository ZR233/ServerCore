#pragma once
#include "connection.hpp"
#include "externvar.hpp"
namespace servercore
{
	class IConnectionHandler :
		public connection_handler
	{
	public:
		IConnectionHandler(size_t head_len, size_t body_len) :connection_handler(head_len, body_len) {};
		virtual ~IConnectionHandler() {}
		void readHeadHandler(std::vector<uint8_t> buf) override = 0;
		void readBodyHandler(std::vector<uint8_t> buf) override = 0;
		boost::shared_ptr<connection_handler> newHandler() override = 0;
	};
}

