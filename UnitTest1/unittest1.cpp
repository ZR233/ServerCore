#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ServerCore/CTaskList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestTask)
		{
			// TODO: 在此输入测试代码
			auto task = servercore::CTask("A");
			task.addParam("名字", "周睿");
			std::string str = task.getValue("名字1");
			std::string str3 = task.getType();
			std::string str2 = "A";
			Assert::AreEqual(str3, str2);
		}

	};
}