#pragma once

#include <string>
#include <fstream> // filesize
#include <cctype> //tolower

#include <algorithm> //transform


template< typename... Args >
std::string string_sprintf( const char* format, Args... args ) 
{
	int length = std::snprintf( nullptr, 0, format, args... );
	//assert( length >= 0 );

	char* buf = new char[length + 1];
	std::snprintf( buf, length + 1, format, args... );

	std::string str( buf );
	delete[] buf;
	return std::move(str);
}

class CUtil
{
public:
	CUtil(){};

	static std::string to_lower( std::string s );
	static std::ifstream::pos_type filesize( const char* filename );
};

