#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ServerCore/Server.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: �ڴ�������Դ���
			Server s;
			auto a = s.add(1, 2);
			Assert::AreEqual(a, 3);
		}

	};
}