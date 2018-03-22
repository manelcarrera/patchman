#include "copenssl.h"

#include <openssl/md5.h>

#include "cutil.h" //string_sprintf


std::string COpenssl::md5( const char* filename, bool strip )
{
	bool res = false;
	std::string hash_s;

    unsigned char c[ MD5_DIGEST_LENGTH ];
    //char *filename="file.c";
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    size_t bytes;
    unsigned char data[1024];
	unsigned char data2[1024];

    if (inFile == NULL) 
	{
        printf ("%s can't be opened.\n", filename);
        //return 0;
    }
	else
	{
		MD5_Init (&mdContext);

		if( strip )
		{
			while ((bytes = fread (data, 1, 1024, inFile)) != 0)
			{
				int j=0;
				for( int i=0; i< bytes; i++ )
				{
					if( data[ i ] == ' ' )
					{
					}
					else
					{
						data2[ j ] = data[ i ];
						j++;
					}
				}
				MD5_Update (&mdContext, data2, j);
			}
		}
		else // regular usage
		{
			while ((bytes = fread (data, 1, 1024, inFile)) != 0)
				MD5_Update (&mdContext, data, bytes);
		}

		MD5_Final (c,&mdContext);
    
		for(int i = 0; i < MD5_DIGEST_LENGTH; i++) 
		{
			//printf("%02x", c[i]);
			hash_s += string_sprintf("%02x", c[ i ]);
		}
		//printf (" %s\n", filename);
		//std::cout << "sprintf: " << hash_s << std::endl; 

		res = true;
    
		fclose (inFile);
		//return std::string( c );
		//return 0;
	}
	return hash_s;
}

