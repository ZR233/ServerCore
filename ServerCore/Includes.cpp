#include "stdafx.h"
#include "Includes.hpp"

namespace servercore
{
	namespace externvar
	{
		//全局控制台输出互斥量
		boost::mutex cout_mu;
	}
}