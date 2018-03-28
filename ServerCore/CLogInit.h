#pragma once
#include <deque>
#include <sstream>
#include <mutex>
#include <boost\thread.hpp>
#include <boost\lockfree\queue.hpp>
// Here we define our application severity levels.
enum severity_level
{
	debug,
	ifo,
	key,
	warning,
	error
};

// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
	static const char* const str[] =
	{
		"debug",
		"ifo",
		"key",
		"warning",
		"error"
	};
	if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast< int >(lvl);
	return strm;
}

//初始化日志
namespace servercore
{
	class CLogInit
	{
	public:
		CLogInit();
		~CLogInit();
	};
}
