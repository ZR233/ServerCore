
#include "stdafx.h"
#include "CServer.h"
#include <stdexcept>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "io_context_pool.hpp"


namespace servercore {

io_context_pool::io_context_pool(std::size_t pool_size): 
	next_io_context_(0),
	pool_size_(pool_size)
{
	if (pool_size == 0)
		throw std::runtime_error("io_context_pool size is 0");
	for (std::size_t i = 0; i < pool_size; ++i)
	{
		io_context_ptr io_context(new boost::asio::io_context());
		work_.push_back(boost::asio::make_work_guard(*io_context));
		io_contexts_.push_back(io_context);
	}
}

void io_context_pool::run()
{
	for (std::size_t i = 0; i < pool_size_; ++i)
	{
		thread_ptr thread(new boost::thread([this,i]() {
			io_contexts_[i]->run();
		}));
		threads_.push_back(thread);
	}
}

void io_context_pool::stop()
{
	for (auto io_context: io_contexts_)
	{
		io_context->stop();
	}
}

void io_context_pool::join()
{
	for (auto var : threads_)
		var->join();
}

boost::asio::io_context& io_context_pool::get_io_context()
{
  auto io_ = io_contexts_[next_io_context_];
  ++next_io_context_;
  if (next_io_context_ == io_contexts_.size())
    next_io_context_ = 0;
  return *io_;
}

} // namespace servercore