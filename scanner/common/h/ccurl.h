#pragma once

#include <string>

class CCUrl
{
public:
	CCUrl(){}

	static void get( const std::string& url, const std::string& content );
};
