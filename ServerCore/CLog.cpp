#include "stdafx.h"
#include "CLog.h"
#include "externvar.hpp"
#include <iostream>
#include <boost/locale/generator.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/support/date_time.hpp>

namespace servercore
{

	namespace logging = boost::log;
	namespace sinks = boost::log::sinks;
	namespace attrs = boost::log::attributes;
	namespace src = boost::log::sources;
	namespace expr = boost::log::expressions;
	namespace keywords = boost::log::keywords;
	using boost::shared_ptr;
	

	void logIni()
	{
		// For now we only create a text output sink:
		typedef sinks::asynchronous_sink< sinks::text_ostream_backend > text_sink;
		shared_ptr< text_sink > pSink(new text_sink());


		//pSink->imbue(std::locale("en_US.UTF-8"));
		{
			// The good thing about sink frontends is that they are provided out-of-box and
			// take away thread-safety burden from the sink backend implementors. Even if you
			// have to call a custom backend method, the frontend gives you a convenient way
			// to do it in a thread safe manner. All you need is to acquire a locking pointer
			// to the backend.
			text_sink::locked_backend_ptr pBackend = pSink->locked_backend();

			// Now, as long as pBackend lives, you may work with the backend without
			// interference of other threads that might be trying to log.

			// Next we add streams to which logging records should be output
			shared_ptr< std::ostream > pStream(&std::clog, boost::null_deleter());
			pBackend->add_stream(pStream);
		}

		pSink->set_formatter(expr::stream
			<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%H:%M:%S]")
			<< "[" << expr::attr< severity_level >("Severity") << "]"
			<< expr::message);
		pSink->set_filter(expr::attr< severity_level >("Severity") >= externvar::log_filter_lv);
		/*logging::add_console_log(std::clog,
		keywords::format = expr::stream
		<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%H:%M:%S]")
		<< "[" << expr::attr< severity_level >("Severity") << "]"
		<< expr::message,
		keywords::filter = expr::attr< severity_level >("Severity") >= externvar::log_filter_lv);*/

		logging::core::get()->add_sink(pSink);
		// One can also use lambda expressions to setup filters and formatters

		if (externvar::log_save_lv == 0)
		{
			logging::add_file_log
			(
				keywords::auto_flush = true,
				keywords::open_mode = std::ios::app,
				keywords::file_name = externvar::log_location + "log/%Y%m%d_debug_%N.log",
				keywords::rotation_size = 10 * 1024 * 1024,/* rotate files every 10 MiB... */
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::filter = expr::attr< severity_level >("Severity") >= debug,
				keywords::format = expr::stream
				<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%H:%M:%S]")
				/*<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
				<< "] [" << expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
				<< "] */
				<< "[" << expr::attr< severity_level >("Severity")
				<< "]" << expr::message
				/*
				keywords::format = expr::format("%1% [%2%] [%3%] <%4%> %5%")
				% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
				% expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
				% expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
				% expr::attr< severity_level >("Severity")
				% expr::message
				*/
			);
		}
		if (externvar::log_save_lv < 2)
		{
			logging::add_file_log
			(
				keywords::auto_flush = true,
				keywords::open_mode = std::ios::app,
				keywords::file_name = externvar::log_location + "log/%Y%m%d_ifo_%N.log",
				keywords::rotation_size = 10 * 1024 * 1024,/* rotate files every 10 MiB... */
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::filter = expr::attr< severity_level >("Severity") >= ifo,
				keywords::format = expr::stream
				<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%H:%M:%S]")
				<< "[" << expr::attr< severity_level >("Severity") << "]"
				<< expr::message
			);
		}
		if (externvar::log_save_lv < 3)
		{
			logging::add_file_log
			(
				keywords::auto_flush = true,
				keywords::open_mode = std::ios::app,
				keywords::file_name = externvar::log_location + "log/%Y%m%d_key_%N.log",
				keywords::rotation_size = 10 * 1024 * 1024,/* rotate files every 10 MiB... */
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::filter = expr::attr< severity_level >("Severity") >= key,
				keywords::format = expr::stream
				<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%H:%M:%S]")
				<< "[" << expr::attr< severity_level >("Severity") << "]"
				<< expr::message
			);
		}
		if (externvar::log_save_lv < 4)
		{
			logging::add_file_log
			(
				keywords::auto_flush = true,
				keywords::open_mode = std::ios::app,
				keywords::file_name = externvar::log_location + "log/%Y%m%d_warning_%N.log",
				keywords::rotation_size = 10 * 1024 * 1024,/* rotate files every 10 MiB... */
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::filter = expr::attr< severity_level >("Severity") >= warning,
				keywords::format = expr::stream
				<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%H:%M:%S]")
				<< expr::message
			);
		}
		if (externvar::log_save_lv < 5)
		{
			logging::add_file_log
			(
				keywords::auto_flush = true,
				keywords::open_mode = std::ios::app,
				keywords::file_name = externvar::log_location + "log/%Y%m%d_error_%N.log",
				keywords::rotation_size = 10 * 1024 * 1024,/* rotate files every 10 MiB... */
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::filter = expr::attr< severity_level >("Severity") >= error,
				keywords::format = expr::stream
				<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "[%H:%M:%S]")
				<< expr::message
			);
		}
		// Also let's add some commonly used attributes, like timestamp and record counter.
		logging::add_common_attributes();
		//logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());

		BOOST_LOG_FUNCTION();

	}

}