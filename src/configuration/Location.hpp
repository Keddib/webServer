#ifndef LOCATION_001_HPP
#define LOCATION_001_HPP

#include "../MACROS.hpp"
#include <arpa/inet.h> //added
#include <sys/socket.h> //added
#include <string>
#include <vector>
#include <utility>
#include <iostream>

class Location
{
	private:
		std::string _prefix;
		std::string _root;
		std::string _CGI;
		std::string _CGIext;
		std::string _upload;
		std::pair<int, std::string> _ret;
		std::vector<std::string> _index;
		std::vector<int> _methods;
		bool _autoIndex;
		unsigned int _cgiTimeOut;
	public:
		Location( const std::string &prefix );
		~Location();
		void setRoot( const std::string &path );
		void setAutoIndex( bool x=0 );
		void setReturn( int, const std::string& );
		void setIndex( const std::string& );
		void setMethods( int );
		void setCGI( const std::string&, const std::string& );
		void setUpload( const std::string& );
		void setCGItimeOut( unsigned int time);
		void fillAllowedMethods(std::string &allow) const;
		const std::string& getPrefix() const;
		bool isMethodAllowed(int meth) const;
		const std::string& getRoute() const;
		const std::vector<std::string>& getIndexes() const;
		bool isAutoIndexOn() const;
		bool isRedirect() const;
		bool isCGI() const;
		bool isUPLOAD() const;
		int getRedirectCode() const;
		const std::string& getRedirectURI() const;
		const std::string& getCGIext() const;
		const std::string& getCGIpath() const;
		const std::string& getUPLOADpath() const;
		unsigned int getCGItimeOut() const;
		void Display() const;

};

std::string	to_string(long long n);

#endif
