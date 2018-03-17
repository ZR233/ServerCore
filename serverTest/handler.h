#pragma once
#include "../ServerCore/IHandler.hpp"

class handler :
	public servercore::IHandler,
	public boost::enable_shared_from_this<handler>
{
public:
	handler();
	~handler();
	void readHeadHandler(std::vector<uint8_t> buf) override;
	void readBodyHandler(std::vector<uint8_t> buf) override;
	boost::shared_ptr<IHandler> newHandler() override
	{
		boost::shared_ptr<IHandler> hand(new handler());
		return hand;
	}
};

