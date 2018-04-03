#pragma once
#include <tuple>
#include <string>
#include <vector>

namespace servercore {
	class ITaskHandler
	{
	public:
		ITaskHandler();
		virtual ~ITaskHandler();
		//服务器任务处理
		virtual void taskHandler(std::tuple<std::vector<std::string>, int> task);
	};
}
