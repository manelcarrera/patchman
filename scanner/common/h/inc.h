#pragma once

#include <string>

#include "cutil.h"

namespace dummy
{
	static const std::string hash1 = "6620f3a6f01d8d7ebfe59cf11e5e49bf";
	static const std::string hash2 = "db67713529131f035c29fba40dea1c2a";

	

	static const std::string ext1 = "txt";

	#ifdef __linux__ 
		static const std::string base_path = "/media/sf_shared/patchman"; //FIXME
	#elif _WIN32
		static const std::string base_path = "D:/shared/patchman"; //FIXME
	#endif
	static const std::string data_path = string_sprintf("%s/data",base_path.c_str());

	static const std::string dir1 = string_sprintf("%s/test/files",data_path.c_str());
	static const std::string db_file = string_sprintf("%s/db/MyDb.db",data_path.c_str());
	static const std::string log_file = string_sprintf("%s/patchman.log",data_path.c_str());
	static const std::string config_file = string_sprintf("%s/patchman.cfg",data_path.c_str());
	static const std::string hashes_file = string_sprintf("%s/hashes",data_path.c_str());
	static const std::string paths_file = string_sprintf("%s/paths",data_path.c_str());

	static const std::string url = "https://example.com/";

	static long max_bytes = 1000*1000;
	static long max_bytes_no_limit = -1;
}

namespace param
{
	static const std::string datapath = "DATAPATH";
	static const std::string db = "DB";
	static const std::string logs = "LOGS";
	static const std::string hashes = "HASHES";
	static const std::string paths = "PATHS";
	static const std::string url = "URL";
	static const std::string ext = "EXT";
	static const std::string maxbytes = "MAXBYTES";
}
