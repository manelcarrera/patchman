#pragma once

#include <string>
#include <vector>

class COpenssl
{
public:
	//COpenssl(){};

	static std::string md5( const char* filename, bool strip = false );
};
