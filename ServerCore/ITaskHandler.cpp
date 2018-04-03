#include "stdafx.h"
#include "ITaskHandler.h"
#include "CLog.h"
#include <boost\locale.hpp>
namespace servercore {
	ITaskHandler::ITaskHandler()
	{
	}


	ITaskHandler::~ITaskHandler()
	{
		BOOST_LOG_SEV(wlg::get(), debug) << L"ITaskHandler析构";
	}
	void ITaskHandler::taskHandler(std::tuple<std::vector<std::string>, int> task)
	{
		BOOST_LOG_SEV(wlg::get(), debug) << L"-----------\n任务类型：" << std::to_wstring(std::get<1>(task));
		for (auto string : std::get<0>(task))
		{
			std::wstring ws = boost::locale::conv::to_utf<wchar_t>(string, "GBK");
			BOOST_LOG_SEV(wlg::get(), debug) << ws;
		}
		BOOST_LOG_SEV(wlg::get(), debug) << L"-----------";
	}
}