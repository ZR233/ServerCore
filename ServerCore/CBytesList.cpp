#include "stdafx.h"
#include "CBytesList.h"


namespace servercore {
	CBytesList::CBytesList()
	{
	}


	CBytesList::~CBytesList()
	{
	}
	void CBytesList::addBytes(std::vector<uint8_t> bytes)
	{
		boost::unique_lock<boost::mutex> mu(task_mu_);
		bytes_.push_back(bytes);
		m_cond_.notify_one();
	}

	std::vector<uint8_t> CBytesList::popBytes()
	{
		boost::unique_lock<boost::mutex> mu(task_mu_);
		if (bytes_.empty())
		{
			//如果队列中没有任务，则等待互斥锁 
			m_cond_.wait(mu);//
		}
		auto bytes = bytes_.front();
		bytes_.pop_front();
		return bytes;
	}
}//namespace servercore