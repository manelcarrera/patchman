#include "csqlite.h"
#include "cutil.h"
#include "copenssl.h"
#include "ccurl.h"
#include "clogic.h"
#include "cdir.h"
#include "inc.h"
#include "clogs.h"
#include "cconfig.h"
#include "cparse.h"

#include "dirent.h"

#include <string>	// to_string
#include <iostream> // std::cout
#include <chrono>

#ifdef LINUX
	#include <unistd.h>
#endif
#ifdef WINDOWS
	#include <windows.h>
#endif
void my_sleep( int sleepMs )
{
	#ifdef LINUX
		usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
	#endif
	#ifdef WINDOWS
		Sleep(sleepMs);
	#endif
}

#ifdef LINUX
	#include "cdaemon.h"
#endif


void ERROR( const char* msg )
{
	std::cerr << "ERROR: " << msg << std::endl;
	Logs::instance()->error( msg );
	std::cout << "Press a key to finish..." << std::endl;
	getchar();
	exit( 1 );
}

int main(int argc, char *argv[])
{
	#ifdef LINUX
		cdaemon::skeleton();
	#endif

	// (00) : Config
	
	std::cout << "config :" << dummy::config_file << std::endl;

	CConfig::instance()->file( dummy::config_file );
	std::vector< std::string > not_found_keys_v;
	bool res0 = CConfig::instance()->verify( {	param::datapath, 
												param::db, 
												param::logs, 
												param::hashes, 
												param::paths,
												param::url,
												param::ext,
												param::maxbytes }, not_found_keys_v );
	if( !res0 )
	{
		res0 = CConfig::instance()->assign_defaults( not_found_keys_v );
		if( !res0 )
			ERROR( "Config" );
	}

	std::string logs_file = CDir::join( CConfig::instance()->get( param::datapath ), CConfig::instance()->get( param::logs ) );
	std::cout << "logs :" << logs_file << std::endl;
	Logs::instance()->file( logs_file );
	Logs::instance()->debug("Starting scanner...");

	bool res;

	// (0) Clean DB
	Logs::instance()->debug("Cleaning DB...");
	std::string db_file = CDir::join( CConfig::instance()->get( param::datapath ), CConfig::instance()->get( param::db ) );
	std::cout << "db :" << db_file << std::endl;
	CSQLite bd( db_file );
	res0 = bd.open();
	if( !res0 )
		ERROR( "DB Open" );

	bool res1 = bd.clean( eTable::All );
	if( !res1 )
		ERROR( "DB Clean" );
	
	// (1) Get hashes from URL and insert them into the DB
	Logs::instance()->debug("Getting hashes from URL...");
	std::string content;
	CCUrl::get( CConfig::instance()->get( param::url ), content );

	std::vector< std::string > hashes_v;
	logic::parse_hash_file( content, hashes_v );

	res = bd.add_hashes( hashes_v );

	res = bd.clean( eTable::Tasks );
	std::string paths_file = CDir::join( CConfig::instance()->get( param::datapath ), CConfig::instance()->get( param::paths ) );
	res = bd.add_task(paths_file,"nothing"); // Just to test
	bd.show( eTable::Tasks );

	while( 1 )
	{
		std::vector< TTask > tasks_v;
		bd.tasks( tasks_v );

		for( int i = 0; i < tasks_v.size(); i++)
		{
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

			TTask& task = tasks_v[ i ];
			logic::scan( bd, hashes_v, task );

			bd.update_task( task.id, true );

			std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
			std::string time_s = string_sprintf("Elapsed time: %.2fs.", std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count() / 1000.0 );
			std::cout << time_s << std::endl;
			Logs::instance()->debug( time_s.c_str() );
		}

		my_sleep( 1*1000 ); //ms
		break; //FIXME: remove to make it a deamon
	}

	// (4) Show tables content
	Logs::instance()->debug("Showing DB tables...");
	std::vector< eTable > tables_v = { Files, Hashes, Paths, Detections, Tasks };
	for( int i=0; i< tables_v.size(); i++ )
		bd.show( (eTable)i );

	std::cout << "Press a key to finish..." << std::endl;
	getchar();
}