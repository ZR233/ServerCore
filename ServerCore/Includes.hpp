#pragma once
#include "../../ZRUtils/ZRUtils/Utils.hpp"
#include <boost/thread/mutex.hpp>

namespace servercore
{
	namespace externvar
	{
		//ȫ�ֿ���̨���������
		extern boost::mutex cout_mu;
	}
}
