// serverTest.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\ServerCore/server.h"
#include "handler.h"
#include "boost/thread.hpp"

int main()
{
	handler han;
	servercore::server s("127.0.0.1", "8801","", 4,
	                     han);
	s.setMaxLink(5);
	s.setRestrictByAttribute("appid", "1", 0);
	boost::thread t1([&]{
		boost::this_thread::sleep(boost::posix_time::seconds(10));
		s.kickByAttribute("appid", "2");
		std::cout << "踢人" << std::endl;
	});
	s.run();

    return 0;
}

