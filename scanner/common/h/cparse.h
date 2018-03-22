#pragma once

#include <string>
#include <vector>
#include <map>

namespace parse
{
	bool lines( const std::string& file_, std::vector< std::string >& lines_v );
	bool params( std::string& file_, std::map< std::string, std::string >& params_m );
}