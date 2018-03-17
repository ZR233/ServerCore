// serverTest.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\ServerCore/server.hpp"
#include "handler.h"

int main()
{
	handler han;
	servercore::server s("127.0.0.1", "8801","", 4,
	                     han);
	s.run();
    return 0;
}

