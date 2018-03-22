#pragma once

#include <string>
#include <map>
#include <vector>


class CConfig
{
	std::map< std::string, std::string > m_params_m;

	std::string m_file;

private:
	CConfig();

public:

	static const std::string NOT_FOUND;

public:

	static CConfig *instance();

	std::string get( const std::string& key );

	std::string file(){ return m_file; };
	bool file( const std::string val );

	bool verify( const std::vector< std::string > keys_v, std::vector< std::string >& not_found_keys_v );

	bool assign_defaults( const std::vector< std::string >& not_found_keys_v );
};
