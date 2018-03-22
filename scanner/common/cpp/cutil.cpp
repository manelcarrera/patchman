#include "cutil.h"

std::string CUtil::to_lower( std::string s ) 
{
    std::transform( s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower( c ); });
    return s;
}

std::ifstream::pos_type CUtil::filesize( const char* filename )
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}
