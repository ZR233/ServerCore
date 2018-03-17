#pragma once
#include "IClientBase.hpp"
#include <set>
#include <memory>
#include <deque>
#include <mutex>


class ClientRoom
{
public:
	typedef std::shared_ptr<IClientBase> IClient_ptr;
	ClientRoom(size_t room_size):
	room_size_(room_size)
	{};
	~ClientRoom() {};
	bool join(IClient_ptr participant)
	{
		std::lock_guard<std::mutex> mu(IClients_mu_);
		if (IClients_.size() < room_size_)
		{
			IClients_.insert(participant);
			return true;
		}
		else
		{
			return false;
		}
		
	}
	void leave(IClient_ptr participant)
	{
		std::lock_guard<std::mutex> mu(IClients_mu_);
		IClients_.erase(participant);
	}

	void send(std::vector<uint8_t>& buf)
	{
		char_vector_que.push_back(buf);
		while (char_vector_que.size() > max_recent_msgs)
			char_vector_que.pop_front();

		for (auto IClients : IClients_)
			IClients->sendToClient(buf);
	}
protected:
	std::mutex IClients_mu_;
	std::set<IClient_ptr> IClients_;
	enum { max_recent_msgs = 100 };
	std::deque<std::vector<uint8_t>> char_vector_que;
	size_t room_size_;
};
