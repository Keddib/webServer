#ifndef COMMON_SERVERS_HPP
#define COMMON_SERVERS_HPP

#include "VirtualServer.hpp"

class	CommonServers
{
	private:
		std::pair<unsigned int, int>	aCSID; // common server id (host, port)
		std::vector<VirtualServer> aServers;

	public:
		CommonServers(const std::pair<unsigned int, int> &ID);
		void	AddServer(const VirtualServer &server);
		bool	operator==(const VirtualServer &server);
		bool	operator==(const std::pair<unsigned int, int> &id);
		std::pair<unsigned int, int> GetIpPortCommonServer() const;
		void	Display() const;
		const VirtualServer&  whichServer( const std::string &) const;
		// return the server to hundle the request based on serverName
	private:
		CommonServers() {}
		CommonServers& operator=(const CommonServers &){return *this;}
		bool ifServerNameNotExist(const std::string&) const;
};

#endif
