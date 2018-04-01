#pragma once
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/support/date_time.hpp>
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
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(lg, boost::log::sources::severity_logger_mt< severity_level >)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(wlg, boost::log::sources::wseverity_logger_mt< severity_level >)

//初始化日志
namespace servercore
{
	void logIni();

}
#ifndef LOG
#define LOG boost::log::sources::severity_logger< severity_level >
#endif // !LOG
#ifndef WLOG
#define WLOG boost::log::sources::wseverity_logger< severity_level >
#endif // !LOG