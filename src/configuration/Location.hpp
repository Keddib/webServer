#ifndef LOCATION_001_HPP
#define LOCATION_001_HPP

#include <string>
#include <vector>
#include <utility>
#include <iostream>


/*
#directives specific to location;
	index pop.hmlt index.html index.htm;
	root /tmp/www;
	accepted_medthos POST DELETE GET
	autoindex off
	return 301 127.0.0.1:9000/data
	CGI /tmp/app/cgi_module.js
	upload /tmp/upload
*/

class Location
{
	private:
		std::string _prefix;
		std::string _root;
		std::string _CGI;
		std::string _upload;
		std::pair<int, std::string> _ret;
		std::vector<std::string> _index;
		std::vector<int> _methods;
		bool _autoIndex;
	public:
		Location( const std::string &prefix );
		~Location();
		void Display() const;
		void setRoot( const std::string &path );
		void setAutoIndex( bool x=0 );
		void setReturn( int, const std::string& );
		void setIndex( const std::string& );
		void setMethods( int );
		void setCGI( const std::string& );
		void setUpload( const std::string& );
		const std::string& getPrefix() const;
		
};

#endif
