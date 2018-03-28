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
			// TODO: 在此输入测试代码
	
		}
		TEST_METHOD(ntoh64)
		{
			// TODO: 在此输入测试代码
			uint64_t a = 111;
			Assert::AreEqual(ntohll(a), zrutils::ntoh64(a));
		}
		TEST_METHOD(hton64)
		{
			// TODO: 在此输入测试代码
			uint64_t a = 11111111111111;
			Assert::AreEqual(htonll(a), zrutils::hton64(a));
		}
	};
}