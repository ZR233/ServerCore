#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ServerCore/Server.hpp"
#include "../../ZRUtils/ZRUtils/Utils.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: �ڴ�������Դ���
	
		}
		TEST_METHOD(ntoh64)
		{
			// TODO: �ڴ�������Դ���
			uint64_t a = 111;
			Assert::AreEqual(ntohll(a), zrutils::ntoh64(a));
		}
	};
}