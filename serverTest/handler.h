#pragma once
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include "../ServerCore/connection.h"
class handler :
	public servercore::IConnectionHandler,
	public boost::enable_shared_from_this<handler>
{
public:
	handler();
	~handler();
	void readHeadHandler(std::vector<uint8_t> buf) override;
	void readBodyHandler(std::vector<uint8_t> buf) override;
	boost::shared_ptr<IConnectionHandler> newHandler() override
	{
		boost::shared_ptr<handler> hand(new handler());
		return hand;
	}

	void serverTaskHandler(std::tuple<std::vector<std::string>, int> task) override{};
};

