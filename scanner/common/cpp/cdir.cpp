#include "cdir.h"
#include "cutil.h"

#include "dirent.h"

#include <string>
#include <vector>
#include <iostream> // std::cout

//https://stackoverflow.com/questions/983376/recursive-folder-scanning-in-cf


std::string CDir::join( std::string name, std::string filename )
{
	return string_sprintf("%s/%s", name.c_str(), filename.c_str() );
}


bool CDir::validate_ext( std::string& name, const std::vector< std::string >& extensions_v )
{
	bool res = false;

	res = std::find( extensions_v.begin(), extensions_v.end(), "*" ) != extensions_v.end();

	if( res ) // no filter
	{
	}
	else
	{
		size_t idx = name.rfind('.');
		if( idx != std::string::npos )
		{
			std::string extension = CUtil::to_lower( name.substr( idx+1 ) );

			res = std::find( extensions_v.begin(), extensions_v.end(), extension ) != extensions_v.end();
		}
	}

	return res;
}

bool CDir::validate_size( std::string& name, long max_bytes )
{
	bool res = max_bytes == -1;

	if( !res )
	{
		long long size_ = CUtil::filesize( name.c_str() );
		res = size_ <= max_bytes;
	}

	return res;
}

void CDir::search_files( const std::string &path, std::vector< std::string >& files_v, const std::vector< std::string >& extensions_v, long max_bytes ) 
{
	std::cout << "Dir : " << path << std::endl;

    if (auto dir = opendir(path.c_str())) 
	{
        while( auto f = readdir( dir ) ) 
		{
            if( !f->d_name || f->d_name[ 0 ] == '.' ) 
				continue;

            if( f->d_type == DT_DIR ) 
			{
				std::string subdir = string_sprintf("%s/%s", path.c_str(), f->d_name );
                search_files( subdir, files_v, extensions_v, max_bytes );
			}

            if( f->d_type == DT_REG )
			{
				std::string name = std::string( f->d_name );

				bool res = validate_ext( name, extensions_v );
				if( !res )
				{
					std::cout << "File : " << name << " : Skip (Extension)" << std::endl;
					continue;
				}

				std::string filename = string_sprintf("%s/%s", path.c_str(), f->d_name );

				res = validate_size( filename, max_bytes );
				if( !res )
				{
					std::cout << "File : " << name << " : Skip (Size)" << std::to_string(  CUtil::filesize( filename.c_str() ) ) <<  std::endl;
					continue;
				}

				std::cout << "Analyzed : " << name << std::endl;

				//std::cout << filename.c_str() << std::endl;
				//std::cout << std::to_string(  CUtil::filesize( filename.c_str() ) ).c_str() << std::endl;
				
				//std::string hash_s = COpenssl::md5( filename.c_str(), true ); //strip whitespaces
				//std::cout << "hash : " << hash_s << std::endl;
				
				files_v.push_back( filename );
			}
        }
        closedir(dir);
    }
}
