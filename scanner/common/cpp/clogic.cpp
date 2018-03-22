#include <string>
#include <vector>

#include <iostream> // std::cout


#include "inc.h"
#include "copenssl.h"
#include "cparse.h"
#include "cconfig.h"
#include "cdir.h"
#include "clogs.h"
#include "csqlite.h"

#include "clogic.h"

namespace logic
{
	void scan( CSQLite& bd, std::vector< std::string >& hashes_v, TTask& task )
	{
		// (2) Insert dirs to scan into the DB
		Logs::instance()->debug("Inserting scanned dirs into the DB...");
		//std::string paths_file = CDir::join( CConfig::instance()->get( param::datapath ), CConfig::instance()->get( param::paths ) );
		std::string paths_file = task.dir;
		std::vector< std::string > dirs_v;
		bool res = parse::lines( paths_file, dirs_v );
		res = bd.clean( eTable::Paths );
		res = bd.add_paths( dirs_v );

		// (3) Scans files and insert detections into the DB
		Logs::instance()->debug("Inserting matching files into the DB...");
		std::vector< std::string > files_v;

		// extensions
		//std::vector< std::string > extensions_v = {"*"};
		//std::vector< std::string > extensions_v = {"tlog","pdb"};
		std::vector< std::string > extensions_v = { CConfig::instance()->get( param::ext ) };
		for( int i=0; i< extensions_v.size(); i++ )
			extensions_v[ i ] = CUtil::to_lower( extensions_v[ i ] );

		for( int i=0; i < dirs_v.size(); i++ )
			CDir::search_files( dirs_v[ i ], files_v, extensions_v, std::stol( CConfig::instance()->get( param::maxbytes ) ) ); // dummy::MaxBytesNoLimit

		std::vector< std::string > md5_v;
		logic::calculate_md5( files_v, md5_v );

		std::vector< std::string > detections_v;
		logic::detections( files_v, md5_v, hashes_v, detections_v );

		res = bd.add_detections( files_v );
	}

	void detections(	const std::vector< std::string >& files_v, 
						const std::vector< std::string >& files_hashes_v, 
						const std::vector< std::string >& hashes_v, 
						std::vector< std::string >& detections_v )
	{
		for( int i=0; i< files_hashes_v.size(); i++ )
		{
			const std::string& file_hash = files_hashes_v[ i ];

			for( int j=0; j < hashes_v.size(); j++ )
			{
				const std::string& hash = hashes_v[ j ];

				if( file_hash == hash )
				{
					const std::string& file = files_v[ i ];
					detections_v.push_back( file );
					std::cout << "Detected : " << file << std::endl;
					break;
				}
			}
		}
	}

	void parse_hash_file( const std::string& content, std::vector< std::string >& hashes_v )
	{
		std::string hashes_file = CDir::join( CConfig::instance()->get( param::datapath ), CConfig::instance()->get( param::hashes ) );
		parse::lines( hashes_file, hashes_v );
		for( int i=0; i< hashes_v.size(); i++ )
			std::cout << "Searched hashes : " << hashes_v[ i ] << std::endl;
	}

	void calculate_md5( const std::vector< std::string >& files_v, std::vector< std::string >& md5_v ) 
	{
		for( int i=0; i< files_v.size(); i++ )
		{
			std::string hash_s = COpenssl::md5( files_v[ i ].c_str(), true ); //strip whitespaces
			std::cout << files_v[ i ] << " : hash: " << hash_s << std::endl;
			md5_v.push_back( hash_s );
		}
	}
}
