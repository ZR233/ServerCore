#include "stdafx.h"
#include "CIoThread.h"

namespace servercore {
	CIoThread::CIoThread()
	{
	}


	CIoThread::~CIoThread()
	{
		io_.stop();
	}
	void CIoThread::run()
	{
		io_thread_ptr = boost::make_shared<boost::thread>(boost::thread([this] {
			io_.run();
		}));
	}
}