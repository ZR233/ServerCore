#pragma once
#include "../ServerCore/IHandlers.hpp"

class handler :
	public servercore::IConnectionHandler,
	public boost::enable_shared_from_this<handler>
{
public:
	handler();
	~handler();
	void readHeadHandler(std::vector<uint8_t> buf) override;
	void readBodyHandler(std::vector<uint8_t> buf) override;
	boost::shared_ptr<connection_handler> newHandler() override
	{
		boost::shared_ptr<handler> hand(new handler());
		return hand;
	}
};

