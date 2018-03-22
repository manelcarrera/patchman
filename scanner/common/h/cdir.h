#pragma once

#include <string>
#include <vector>

class CDir
{
public:
	static void search_files( const std::string &path, std::vector< std::string >& files_v, const std::vector< std::string >& extensions_v={"*"}, long max_bytes = -1 ); //FXME: static const
	static bool validate_ext( std::string& name, const std::vector< std::string >& extensions_v );
	static bool validate_size( std::string& name, long max_bytes );
	static std::string join( std::string name, std::string filename );
};
