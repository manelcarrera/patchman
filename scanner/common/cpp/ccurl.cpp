#include "ccurl.h"

#include "curl/curl.h"

// cURL
// https://stackoverflow.com/questions/47878888/getting-curl-to-work-with-visual-studios-2017
// https://curl.haxx.se/libcurl/c/getinmemory.html


static size_t write_callback( void *contents, size_t size, size_t nmemb, void *userp )
{ 
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void CCUrl::get( const std::string& url, const std::string& content )
{
	CURL *curl;
	CURLcode res; 

	//std::string read_buffer;

	curl_global_init(CURL_GLOBAL_ALL); 

	curl = curl_easy_init();
	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);

		#ifdef SKIP_PEER_VERIFICATION
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		#endif

		#ifdef SKIP_HOSTNAME_VERIFICATION
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		#endif

		res = curl_easy_perform(curl);

		if( res != CURLE_OK )
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror( res ) );

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}
