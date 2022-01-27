#ifndef VIRTUALSERVER_001_HPP
#define VIRTUALSERVER_001_HPP

#include "Location.hpp"
#include <utility>
#include <iostream>
#include <arpa/inet.h>

/*

server {
	# directives specific to servers
		host 0.0.0.0;
		port 80;
		server_name _;
		error_page 404 /404.html
		client_max_body_size 1000000 #bytes
		root /tmp/www;
		index pop.hmlt index.html index.htm;

	#directives specific to location;
		root /tmp/www;
		index pop.hmlt index.html index.htm;
		accepted_medthos POST DELETE GET
		autoindex off
		return 301 127.0.0.1:9000/data
		CGI /tmp/app/cgi_module.js
		upload /tmp/upload
}

*/


class ErrorPage {
	public:
		const std::pair<int, std::string> err;
		const std::string& getPath() const;
		// ErrorPage(int errCode, std::string path) : err.first(errCode), err.second(path) {}
		int getErrCode() const { return err.first; }
		// void setPath(std::string path) { err.second = path; }
};


class VirtualServer
{
	private:
		std::pair<unsigned int, int> aServerInfo;// ip , port in this order
		std::string _serverName;
		std::vector<ErrorPage> _errorPages;
		std::vector<Location> _locations;
		unsigned int _client_max_body_size; // default 1048576Bytes = 1MB

	public:
		VirtualServer(const std::pair<unsigned int, int> &);
		bool	operator==(const VirtualServer &rhs);
		std::pair<unsigned int, int>	GetHostAndPort() const;
		std::vector<Location>& getLocationsToEdit();
		void	Display() const;
		void addLocation(const Location &location);
		void setHostValue(unsigned int);
		void setPortValue(int);
		void setServerNameValue(const std::string &s);
		void setMaxBodySize(unsigned long long);
		// const Loaction& wichLocation( const std::string &Path)
		// return which location  based on the path requested
};

#endif
