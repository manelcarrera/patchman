#include "csqlite.h"
#include "cutil.h"

// SQLite3
// LIB /DEF:sqlite3.def /MACHINE:x64
// https://dcravey.wordpress.com/2011/03/21/using-sqlite-in-a-visual-c-application/
// Download : https://www.sqlite.org/download.html

// cmd prompt Visual Studio 
//https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs

// SQLite GUI Client (Portable)
// http://sqlitebrowser.org/

#include "sqlite3.h"

#include <iostream> // std::cout


CSQLite::CSQLite( std::string file )
{
	m_db = nullptr;
	m_file = file;
}

CSQLite::~CSQLite()
{
	if( m_db )
	{
		close();
	}
}

void CSQLite::close()
{
   // Close Database
   //std::cout << "Closing MyDb.db ..." << std::endl;
   sqlite3_close( m_db );
   std::cout << "Closed " << m_file << std::endl;
}

bool CSQLite::open()
{
	bool res = false;

	int rc;
 
	//std::cout << "Opening MyDb.db ..." << std::endl;
	rc = sqlite3_open( m_file.c_str(), &m_db);
	if (rc)
	{
		std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(m_db) << std::endl << std::endl;
		sqlite3_close(m_db);
	}
	else
	{
		//std::cout << "DB: " << m_file << std::endl;
		res = true;
	}
	return res;
}


bool CSQLite::add_files( const std::vector< std::string >& files_v, const std::vector< std::string >& hashes_v )
{
	bool res = false;
	for( int i=0; i < files_v.size(); i++ )
	{
		res = add_file( files_v[ i ], hashes_v[ i ] );
		if( !res )
			break;
	}
	return res;
}

bool CSQLite::add_file( const std::string& file_, const std::string& hash )
{
	bool res = false;

	char *error;

	//std::cout << "Inserting a value into 'Files' ..." << std::endl;

	std::string sql_s = string_sprintf("INSERT INTO Files VALUES( '%s', '%s' )", file_.c_str(), hash.c_str() );
	   
	const char *sql_cp = sql_s.c_str();
	int rc = sqlite3_exec(m_db, sql_cp, NULL, NULL, &error);
	if (rc)

	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg( m_db ) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		std::cout << "Inserted a value into 'Files'." << std::endl;
		res = true;
	}
	return res;
}

bool CSQLite::add_detections( const std::vector< std::string >& files_v )
{
	bool res = false;
	for( int i=0; i < files_v.size(); i++ )
	{
		res = add_detection( files_v[ i ] );
		if( !res )
			break;
	}
	return res;
}

bool CSQLite::add_detection( const std::string& file_ )
{
	bool res = false;

	char *error;

	//std::cout << "Inserting a value into 'Files' ..." << std::endl;

	std::string sql_s = string_sprintf("INSERT INTO Detections VALUES( '%s' )", file_.c_str() );
	   
	const char *sql_cp = sql_s.c_str();
	int rc = sqlite3_exec(m_db, sql_cp, NULL, NULL, &error);
	if (rc)

	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg( m_db ) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		std::cout << "Inserted a value into 'Detections'." << std::endl;
		res = true;
	}
	return res;
}

bool CSQLite::add_hashes( const std::vector< std::string >& hashes_v )
{
	bool res = false;

	for( int i=0; i < hashes_v.size(); i++ )
	{
		res = add_hash( hashes_v[ i ] );
		if( !res )
			break;
	}
	return res;
}

bool CSQLite::add_hash( const std::string& hash )
{
	bool res = false;

	char *error;

	std::string sql_s = string_sprintf("INSERT INTO Hashes VALUES( '%s' )", hash.c_str() );

	const char *sql_cp = sql_s.c_str();
	int rc = sqlite3_exec(m_db, sql_cp, NULL, NULL, &error);
	if (rc)
	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(m_db) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		std::cout << "Inserted a value into 'Hashes'." << std::endl;
		res = true;
	}
	return res;
}

bool CSQLite::add_task( const std::string& dir, const std::string& crontime )
{
	bool res = false;

	char *error;

	std::string sql_s = string_sprintf("INSERT INTO Tasks (Dir,Crontime) VALUES( '%s', '%s' )", dir.c_str(), crontime.c_str() );

	const char *sql_cp = sql_s.c_str();
	int rc = sqlite3_exec(m_db, sql_cp, NULL, NULL, &error);
	if (rc)
	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(m_db) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		std::cout << "Inserted a value into 'Tasks'." << std::endl;
		res = true;
	}
	return res;
}

bool CSQLite::update_task( int task_id, bool treated )
{
	bool res = false;

	char *error;

	std::string sql_s = string_sprintf("UPDATE Tasks SET Treated = %d where Id = %d", treated, task_id );

	const char *sql_cp = sql_s.c_str();
	int rc = sqlite3_exec(m_db, sql_cp, NULL, NULL, &error);
	if (rc)
	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(m_db) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		std::cout << "Updated table 'Tasks'." << std::endl;
		res = true;
	}
	return res;
}


bool CSQLite::tasks( std::vector< TTask >& tasks_v )
{
	bool res = true;

	const char *sql_cp = "SELECT * FROM Tasks where Treated = 0;";
	char **results = NULL;
	int rows, columns;
	char *error;

	sqlite3_get_table( m_db, sql_cp, &results, &rows, &columns, &error );

	for( int i = 1; i <= rows; i++ )
    {
		std::string dir = results[ (i*columns) ];
		std::string cronetime = results[ (i*columns)+1 ];
		int id = atoi( results[ (i*columns)+2 ] );

		TTask task = { id, dir, cronetime };

		tasks_v.push_back( task );
    }

	sqlite3_free_table(results);

	return res; //FIXME: error tracking not implemented
}


bool CSQLite::add_paths( const std::vector< std::string >& paths_v )
{
	bool res = false;

	char *error;

	for( int i=0; i < paths_v.size(); i++ )
	{
	   // Execute SQL
	   //std::cout << "Inserting a value into 'Paths' ..." << std::endl;

	   std::string sql_s = string_sprintf("INSERT INTO Paths VALUES( '%s' )", paths_v[ i ].c_str() );
	   
	   const char *sql_cp = sql_s.c_str();
	   int rc = sqlite3_exec(m_db, sql_cp, NULL, NULL, &error);
	   if (rc)
	   {
		  std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(m_db) << std::endl << std::endl;
		  sqlite3_free(error);
	   }
	   else
	   {
		  std::cout << "Inserted a value into 'Paths'." << std::endl;
		  res = true;
	   }
	}
	return res;
}

bool CSQLite::clean( eTable table )
{
	bool res = false;

	char *error;

	switch( table )
	{
		case eTable::All:
		{
			res = true;
			std::vector< eTable > tables_v = { Files, Hashes, Paths, Detections, Tasks };
			for( int i=0; i< tables_v.size(); i++ )
				res = res && clean( (eTable)i );
			break;
		}
		default:
		{
			std::string name = m_sTable[ table ];
			// Execute SQL
			//std::cout << "Cleaning " << name << "..." << std::endl;
			std::string sql_s = string_sprintf("DELETE FROM %s;", name.c_str() );
			const char *sql_cp = sql_s.c_str();
			int rc = sqlite3_exec(m_db, sql_cp, NULL, NULL, &error);
			if (rc)
			{
				std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(m_db) << std::endl;
				sqlite3_free(error);
			}
			else
			{
				std::cout << "Table cleaned : " << name << std::endl;
				res = true;
			}
			break;
		}
	}
	return res;
}

bool CSQLite::show( eTable table )
{
	bool res = false;

	std::string name = m_sTable[ table ];

	std::cout << "Table:" << name << std::endl;

   // Display
   //std::cout << "Retrieving values in " << name << "..." << std::endl;
   std::string sql_s = string_sprintf("SELECT * FROM %s;", name.c_str() );
   const char *sql_cp = sql_s.c_str();
   char **results = NULL;
   int rows, columns;
   char *error;
   sqlite3_get_table(m_db, sql_cp, &results, &rows, &columns, &error);

	for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
    {
        for (int colCtr = 0; colCtr < columns; ++colCtr)
        {
			// Determine Cell Position
			int cellPosition = (rowCtr * columns) + colCtr;
 
			// Display Cell Value
			std::cout.width(12);
			std::cout.setf(std::ios::left);
			std::cout << results[cellPosition] << " ";
        }
 
        // End Line
        std::cout << std::endl;
 
        // Display Separator For Header
        if (0 == rowCtr)
        {
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				std::cout.width(12);
				std::cout.setf(std::ios::left);
				std::cout << "~~~~~~~~~~~~ ";
			}
			std::cout << std::endl;
        }
    }

	sqlite3_free_table(results);

	return res; //FIXME: error tracking not implemented
}

int CSQLite::demo()
{
   int rc;
   char *error;
 
   // Open Database
   std::cout << "Opening MyDb.db ..." << std::endl;
   sqlite3 *db;
   //rc = sqlite3_open("MyDb.db", &db);
   std::string dbfile = "MyDb.db";
   rc = sqlite3_open( dbfile.c_str(), &db);
   if (rc)
   {
      std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl << std::endl;
      sqlite3_close(db);
      return 1;
   }
   else
   {
      std::cout << "Opened MyDb.db." << std::endl << std::endl;
   }
 
   // Execute SQL
   std::cout << "Creating MyTable ..." << std::endl;
   const char *sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
   rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
   if (rc)
   {
      std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
      sqlite3_free(error);
   }
   else
   {
      std::cout << "Created MyTable." << std::endl << std::endl;
   }
 
   // Execute SQL
   std::cout << "Inserting a value into MyTable ..." << std::endl;
   const char *sqlInsert = "INSERT INTO MyTable VALUES(NULL, 'A Value');";
   rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
   if (rc)
   {
      std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
      sqlite3_free(error);
   }
   else
   {
      std::cout << "Inserted a value into MyTable." << std::endl << std::endl;
   }
 
   // Display MyTable
   std::cout << "Retrieving values in MyTable ..." << std::endl;
   const char *sqlSelect = "SELECT * FROM MyTable;";
   char **results = NULL;
   int rows, columns;
   sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
   if (rc)
   {
      std::cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << std::endl << std::endl;
      sqlite3_free(error);
   }
   else
   {
      // Display Table
      for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
      {
         for (int colCtr = 0; colCtr < columns; ++colCtr)
         {
            // Determine Cell Position
            int cellPosition = (rowCtr * columns) + colCtr;
 
            // Display Cell Value
            std::cout.width(12);
            std::cout.setf(std::ios::left);
            std::cout << results[cellPosition] << " ";
         }
 
         // End Line
         std::cout << std::endl;
 
         // Display Separator For Header
         if (0 == rowCtr)
         {
            for (int colCtr = 0; colCtr < columns; ++colCtr)
            {
               std::cout.width(12);
               std::cout.setf(std::ios::left);
               std::cout << "~~~~~~~~~~~~ ";
            }
            std::cout << std::endl;
         }
      }
   }
   sqlite3_free_table(results);
 
   // Close Database
   std::cout << "Closing MyDb.db ..." << std::endl;
   sqlite3_close(db);
   std::cout << "Closed MyDb.db" << std::endl << std::endl;
 
   // Wait For User To Close Program
   std::cout << "Please press any key to exit the program ..." << std::endl;
   std::cin.get();
 
   return 0;
}
