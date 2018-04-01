#include "stdafx.h"
#include "externvar.hpp"

namespace servercore {
	//namespace externvar {
		string externvar::log_location = "";
		int  externvar::log_filter_lv = 0;
		int externvar::log_save_lv = 0;
		CLog externvar::clog;
	//}//externvar
}//servercore
