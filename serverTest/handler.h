#pragma once
#include "../ServerCore/IHandler.hpp"

class handler :
	public servercore::IHandler
{
public:
	handler();
	~handler();
	void readHeadHandler(std::vector<uint8_t> buf) override;
	void readBodyHandler(std::vector<uint8_t> buf) override;
};

