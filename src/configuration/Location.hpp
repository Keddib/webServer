#ifndef LOCATION_001_HPP
#define LOCATION_001_HPP

#include <string>
#include <vector>
#include <utility>

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
		std::string CGI;
		std::pair<int, std::string> _ret;
		std::vector<std::string> _index;
		std::vector<std::string> _methods;
		unsigned int _client_max_body_size;
		bool _autoIndex;
		// add upload path
	public:
		Location( const std::string &prefix );
		~Location();
		void setRoot( const std::string &path );
		void setAutoIndex( bool x=0 );
		void setReturn( int, const std::string& );
		void setIndex( const std::string& );
		void setMethods( const std::string& );
		void setCGI( const std::string& );
		void setMaxBodySize( const std::string &);
		void setUploadPath(const std::string &);
		// set default values from server
		void setDefualtValues(
				const std::string &root,
				const std::vector<std::string> &indexes,
				const unsigned int &MaxBodySize,
				bool autoIndex
			);

};

std::ostream& operator<<(std::ostream&, const Location&);

#endif
