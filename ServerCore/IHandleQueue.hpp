#pragma once
#include <vector>
#include <deque>
#include <mutex>

struct Task
{
	int handle_type;
	std::vector<std::string> task;
};

class IHandleQueue
{
public:
	IHandleQueue()
	{};
	virtual ~IHandleQueue() {};
	virtual void addTask(Task task)
	{
		std::lock_guard<std::mutex> mu(task_que_mu_);
		task_que_.push_back(task);
	}
	virtual void handle() = 0;

protected:
	std::mutex task_que_mu_;
	std::deque<Task> task_que_;
};
