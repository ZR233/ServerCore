#pragma once
#include <memory>



class IHandlerBase
{
public:
	IHandlerBase() = default;
	virtual ~IHandlerBase() = default;
	//std::shared_ptr<子类>
	virtual std::shared_ptr<IHandlerBase> getNewInstance() = 0;
};

