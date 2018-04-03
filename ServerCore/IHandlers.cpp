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
	boost::shared_ptr<IHandlers> IHandlers::newHandler()
	{
		boost::shared_ptr<IHandlers> hand(new IHandlers());
		BOOST_LOG_SEV(wlg::get(), warning) << L"未重写IHandlers子类的newHandler方法";
		return hand;
	}
}