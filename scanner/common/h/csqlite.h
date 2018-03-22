#pragma once

class sqlite3;

#include <string>
#include <vector>

enum eTable{ Files, Hashes, Paths, Detections, Tasks, All };

const std::string m_sTable[]={ "Files", "Hashes", "Paths", "Detections", "Tasks" };

struct TTask{ int id; std::string dir ; std::string cronotime; };

class CSQLite
{
	sqlite3* m_db;
	std::string m_file;

public:
	CSQLite( std::string file );
	~CSQLite();
	
	int demo();

	bool open();
	void close();

	bool add_files( const std::vector< std::string >& files_v, const std::vector< std::string >& hashes_v );
	bool add_file( const std::string& file_, const std::string& hash );
	bool add_hashes( const std::vector< std::string >& hashes_v );
	bool add_hash( const std::string& hash );
	bool add_paths( const std::vector< std::string >& paths_v );

	bool clean( eTable table );

	bool show( eTable table );

	bool add_task( const std::string& dir, const std::string& crontime );

	bool tasks( std::vector< TTask >& tasks_v );
	bool update_task( int task_id, bool treated );

	bool add_detection( const std::string& file_ );
	bool add_detections( const std::vector< std::string >& files_v );
};


