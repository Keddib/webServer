#ifndef VIRTUALSERVER_001_HPP
#define VIRTUALSERVER_001_HPP

#include "Location.hpp"
// i move header from here to location.hpp
#include "../MACROS.hpp"
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


class VirtualServer
{
	private:
		std::pair<unsigned int, int> aServerInfo;// ip , port in this order
		std::string _serverName;
		std::vector<std::pair<int, std::string> > _errorPages;
		std::vector<Location> _locations;
		unsigned int _client_max_body_size; // default 1048576Bytes = 1MB
		Location _errLocation;

	public:
		VirtualServer(const std::pair<unsigned int, int> &);
		bool	operator==(const VirtualServer &rhs);
		std::pair<unsigned int, int>	GetHostAndPort() const;
		std::vector<Location>& getLocationsToEdit();
		void	Display() const;
		void addLocation(const Location &location);
		void setHostValue(std::string &);
		void setPortValue(int);
		void setServerNameValue(const std::string &s);
		void setMaxBodySize(unsigned long long);
		void setErrorPage(int , const std::string& );
		// return which location  based on the path requested
		const Location& whichLocation( const std::string &Path) const;
		const std::string& getServerName() const;
		unsigned int getClientMaxBodySize() const;
		const std::vector<std::pair<int, std::string> >& getErrorPages() const;
};

int findLocationURL(const std::string &path, const std::string &prefix);

#endif
