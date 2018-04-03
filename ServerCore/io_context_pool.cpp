
#include "stdafx.h"
#include "server.h"
#include <stdexcept>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "io_context_pool.hpp"


namespace servercore {

io_context_pool::io_context_pool(std::size_t pool_size, IHandlers &handlers)
  : next_io_context_(0),
	handlers_(handlers)
{
	if (pool_size == 0)
		throw std::runtime_error("io_context_pool size is 0");
	for (std::size_t i = 0; i < pool_size; ++i)
	{
		io_thread_ptr io_context(new CIoThread(handlers_.newHandler()));
		io_threads_.push_back(io_context);
		work_.push_back(boost::asio::make_work_guard(*(io_context->getIo())));
	}
}

void io_context_pool::run()
{
	for(auto var: io_threads_)
		var->run();
	for (auto var : io_threads_)
		var->join();
}

void io_context_pool::stop()
{
	for (auto thread: io_threads_)
	{
		thread->stop();
	}
}

boost::asio::io_context& io_context_pool::get_io_context()
{
  auto& io_thread = *io_threads_[next_io_context_];
  ++next_io_context_;
  if (next_io_context_ == io_threads_.size())
    next_io_context_ = 0;
  return *(io_thread.getIo());
}

} // namespace servercore