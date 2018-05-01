#pragma once
#include <vector>
#include <deque>
#include <tuple>
#include <boost\thread.hpp>
#include <map>
#include <string>

namespace servercore {

	//thread safe
	class CBytesList
	{
	public:
		CBytesList();
		~CBytesList();

		void addBytes(std::vector<uint8_t> bytes);
		//取出一条任务，如果没有任务则阻塞
		std::vector<uint8_t> popBytes();
	private:
		boost::condition_variable_any m_cond_;//条件变量
		boost::mutex task_mu_;
		std::deque<std::vector<uint8_t>> bytes_;
	};
}

