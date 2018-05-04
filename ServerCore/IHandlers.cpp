#include "stdafx.h"
#include "IHandlers.h"
#include "CLog.h"
namespace servercore {
	IHandlers::IHandlers()
	{
	}
	IHandlers::~IHandlers()
	{
	}
	//std::shared_ptr<IHandlerBase> IHandlers::getNewInstance()
	//{
	//	std::shared_ptr<IHandlerBase> hand(new IHandlers());
	//	BOOST_LOG_SEV(wlg::get(), warning) << L"未重写IHandlers子类的getNewInstance方法";
	//	return hand;
	//}
}