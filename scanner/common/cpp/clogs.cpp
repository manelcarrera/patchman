#include "clogs.h"

#include <iostream>
#include <fstream>
#include <cstdarg>

#include <stdio.h>
#include <time.h>

#include <string>

#include "cutil.h"

std::string Logs::m_file = "/patchman.log";

static const int MAX_LEN_BUFFER = 4096;//2048;

static const int FILE_MAX_LEN = 10*1000;


Logs::Config::Config()
{
	error			= true;
	info			= true;
	debug			= true;

	max_file_size	= FILE_MAX_LEN;
}  

Logs::Logs(){}

Logs *Logs::instance()
{
	static Logs _instance;
	return &_instance;
}

bool Logs::swap_file()
{
	if( m_count % 10 == 0 )
	{
		long long size =  CUtil::filesize( m_file.c_str() );
		if( size > m_cfg.max_file_size )
		{
			rename( m_file.c_str(), (m_file+".OLD").c_str() );
			m_count = 0;
			return true;
		}
	}
	return false;
}

void Logs::print( const char* cp )
{
	m_count++;

	time_t timer;
    char buffer[128];
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S - ", tm_info);

	swap_file();

	std::ofstream myfile;
	myfile.open( m_file, std::ios_base::app );
	myfile << buffer << cp << std::endl ;
	myfile.close();
}

void Logs::debug( const char* fmt, ... )
{
	if( m_cfg.debug )
	{
		std::lock_guard<std::mutex> lock(m);

		char buf[ MAX_LEN_BUFFER ];     // this should really be sized appropriately
						   // possibly in response to a call to vsnprintf()
		va_list vl;
		va_start( vl, fmt );
		vsnprintf( buf, sizeof( buf ), fmt, vl );
		va_end( vl );

		print( (char*)( std::string("[DEBUG] ") + std::string( buf ) ).c_str() );
	}
}

void Logs::error( const char* fmt, ... )
{
	if( m_cfg.error )
	{
		std::lock_guard<std::mutex> lock(m);

		char buf[ MAX_LEN_BUFFER ];     // this should really be sized appropriately
						   // possibly in response to a call to vsnprintf()
		va_list vl;
		va_start( vl, fmt );
		vsnprintf( buf, sizeof( buf ), fmt, vl );
		va_end( vl );

		//print( buf );
		print( (char*)( std::string("[ERROR] ") + std::string( buf ) ).c_str() );
	}
}

void Logs::info( const char* fmt, ... )
{
	if( m_cfg.info )
	{
		std::lock_guard<std::mutex> lock(m);

		char buf[ MAX_LEN_BUFFER ];     // this should really be sized appropriately
						   // possibly in response to a call to vsnprintf()
		va_list vl;
		va_start( vl, fmt );
		vsnprintf( buf, sizeof( buf ), fmt, vl );
		va_end( vl );

		print( (const char*)( std::string("[INFO ] ") + std::string( buf ) ).c_str() );
	}
}