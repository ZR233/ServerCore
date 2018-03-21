#pragma once
#include "../../ZRUtils/ZRUtils/Utils.hpp"
#include <boost/thread/mutex.hpp>

namespace servercore
{
	namespace externvar
	{
		//全局控制台输出互斥量
		extern boost::mutex cout_mu;
	}
}
