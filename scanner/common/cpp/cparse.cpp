#include "cparse.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <fstream> //ifstream

namespace parse
{
	bool lines( const std::string& file_, std::vector< std::string >& lines_v )
	{
		std::string line;

		std::ifstream infile( file_.c_str() );
		while( std::getline( infile, line ) )
		{
			if( line.empty() )
				continue;

			if( line[ 0 ] == '#' )
				continue;

			lines_v.push_back( line );
		}
		return true;
	}

	bool params( std::string& file_, std::map< std::string, std::string >& params_m )
	{
		bool res = true;

		std::string line;

		std::ifstream infile( file_.c_str() );

		while( std::getline( infile, line ) )
		{
			if( line.empty() )
				continue;

			if( line[ 0 ] == '#' )
				continue;

			int pos = line.find('=');
			if( pos == std::string::npos )
				continue;

			std::string key = line.substr( 0, pos ); // ( ini, lenght)
			std::string value = line.substr( pos+1 ); // from pos to the end

			params_m[ key ] = value;
		}
		return res;
	}
}