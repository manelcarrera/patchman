#pragma once

#include <string>
#include <map>

#include <mutex>

class Logs
{
	static std::string m_file;

	int m_count;

	class Config
	{
	public:
		bool error;
		bool info;
		bool debug;

		double max_file_size;

		Config();// : error( false ), info( false ), debug( false ){}  
	};

public:

	Config m_cfg;

	static Logs *instance();

	static const std::string DEFAULT_IT_FOLDER;
	static const std::string DEFAULT_FOLDER;
	static const std::string DEFAULT_FILE;


	std::string file(){ return m_file; };
	void file( std::string val ){ m_file = val; };

	bool swap_file();

private:
	void print( const char* cp );

public:
	
	Logs();

	void debug( const char* fmt, ... );
	void error( const char* fmt, ... );
	void info( const char* fmt, ... );

	mutable std::mutex m;
};
