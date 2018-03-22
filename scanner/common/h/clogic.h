#pragma once

#include <string>
#include <vector>

namespace logic
{
	void detections(	const std::vector< std::string >& files_v, 
						const std::vector< std::string >& files_hashes_v, 
						const std::vector< std::string >& hashes_v, 
						std::vector< std::string >& detections_v );

	void scan(	CSQLite& bd, 
				std::vector< std::string >& hashes_v, 
				TTask& task );

	void parse_hash_file( const std::string& content, std::vector< std::string >& hashes_v );
	void calculate_md5( const std::vector< std::string >& files_v, std::vector< std::string >& md5_v );
}
