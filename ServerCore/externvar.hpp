#pragma once
#include "../../ZRUtils/ZRUtils/Utils.hpp"
#include <boost/thread/mutex.hpp>
#include <string>
namespace servercore
{
	using string = std::string;
	class externvar
	{
		
		externvar() {}
		~externvar() {}
	public:
		static string log_location;
		static int log_filter_lv;
		static int log_save_lv;
	};

	string externvar::log_location = "";
	int externvar::log_filter_lv = 0;
	int externvar::log_save_lv = 0;
}
