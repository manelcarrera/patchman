#include "cconfig.h"

#include "cparse.h"

#include "inc.h"

const std::string CConfig::NOT_FOUND = "NOT_FOUND";

CConfig::CConfig(){}

bool CConfig::file( const std::string val )
{ 
	m_file = val; 
	bool res = parse::params( m_file, m_params_m );
	return res;
}

bool CConfig::verify(  const std::vector< std::string > keys_v, std::vector< std::string >& not_found_keys_v )
{
	bool res = true;
	for( int i=0; i < keys_v.size(); i++ )
	{
		const std::string& key = keys_v[ i ];
		if( m_params_m.find( key ) == m_params_m.end() )
		{
			not_found_keys_v.push_back( key );
			res = false;
		}
	}
	return res;
}

bool CConfig::assign_defaults( const std::vector< std::string >& not_found_keys_v )
{
	bool res = true;
	for( int i=0; i < not_found_keys_v.size(); i++ )
	{
		const std::string& key = not_found_keys_v[ i ];

		if(	key == param::datapath ) m_params_m[ param::datapath ] = dummy::data_path;
		else if( key == param::db ) m_params_m[ param::db ] = dummy::db_file;
		else if( key == param::logs ) m_params_m[ param::logs ] = dummy::log_file;
		else if( key == param::hashes ) m_params_m[ param::hashes ] = dummy::hashes_file;
		else if( key == param::paths ) m_params_m[ param::paths ] = dummy::paths_file;
		else if( key == param::url ) m_params_m[ param::url ] = dummy::url;
		else if( key == param::ext ) m_params_m[ param::ext ] = dummy::ext1;
		else if( key == param::maxbytes ) m_params_m[ param::maxbytes ] = std::to_string( dummy::max_bytes );
		else 
			res = false;
	}
	return res;
}


CConfig *CConfig::instance()
{
	static CConfig _instance;
	return &_instance;
}

std::string CConfig::get( const std::string& key )
{
	if( m_params_m.find( key ) == m_params_m.end() )
		return CConfig::NOT_FOUND;
	return m_params_m[ key ];
}

