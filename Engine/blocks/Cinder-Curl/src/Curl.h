#pragma once

#include <map>
#include <string>
#include <vector>
#include <curl/curl.h>

#include "cinder/app/AppNative.h"
#include "cinder/Filesystem.h"

namespace mndl 
{
	namespace curl 
	{
		class Curl
		{
		public:
			Curl(){};
			~Curl(){};
			static std::string post( const std::string &url, std::vector<std::string>           &params );
			static std::string post( const std::string &url, std::map<std::string, std::string> &params );
			static std::string postStand(const std::string &url, std::map< std::string, std::string> &m, const  std::string& access_token);
			static std::string deleteRequest(const std::string &url,std:: map<std::string,std::string> &m);
			static std::string postUpload( const  std::string &url, std::map<std::string, std::string> &params, std::string path );
			static std::string postUploadFB( const std::string &url, const std::string &  ACCESSTOKEN, const std::string &  path, const std::string &  message);
	
			static std::string postImage( const std::string &Serverurl, const std::string &Imageurl );
			static std::string Curl::postImageStand(const std::string &Serverurl, const std::string &Imageurl, const std::string& access_token);
			static std::string sendMail( );

			static std::string get ( const std::string &url );
			static std::string getStand(const std::string &url, const std::string& access_token);
			static bool       ftpUpload( const std::string &ftpUrl, const std::string &userName, const std::string &password, const ci::fs::path &path );
	
			static CURL * curl; 
			static void clean ();

		protected:
			static int writer( char *data, size_t size, size_t nmemb, std::string *buffer );
			static std::string easyCurl(const std::string &url, bool post, const std::string &postParamString);
			static std::string easyCurlStand(const std::string &url, bool post, const std::string &postParamString, const std::string &access_token);
			
			static std::string easyCurlUpload( const std::string &url, bool post, const std::string &postParamString, std::FILE* file ); 	
			static std::string urlEncode( const std::string &c );
			static std::string char2Hex( char dec );
		};
	}
}