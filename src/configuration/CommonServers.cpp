#include "CommonServers.hpp"

CommonServers::CommonServers(const std::pair<unsigned int, int> &ID)
{
	aCSID.first = ID.first;	 // host
	aCSID.second = ID.second; // port
}

bool	CommonServers::operator==(const VirtualServer &srv)
{
	if (aCSID == srv.GetHostAndPort())
		return true;
	return false;
}

void	CommonServers::AddServer(const VirtualServer &server)
{
	if (ifServerNameNotExist(server.getServerName()))
		aServers.push_back(server);
	else
	{
		// << "" on " 0.0.0.0":" , ignored"
		char str[INET_ADDRSTRLEN];
		struct sockaddr_in sa;
		sa.sin_addr.s_addr = server.GetHostAndPort().first;
		inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
		std::cout << "WebServ: [warn] conflicting server_name \"" << server.getServerName()
		+ "\" on " << str << ":" << ntohs(server.GetHostAndPort().second) << " ignored\n";
	}
}

void	CommonServers::Display() const
{
	std::cout << "start of " << aCSID.first << " , " << aCSID.second <<   std::endl;
	std::vector<VirtualServer>::const_iterator first(aServers.begin()), last(aServers.end());
	while (first != last)
	{
		first->Display();
		++first;
	}
	std::cout << std::endl;
}


bool	CommonServers::operator==(const std::pair<unsigned int, int> &id)
{
	return (aCSID == id);
}

std::pair<unsigned int, int> CommonServers::GetIpPortCommonServer() const
{
	return aCSID;
}

bool CommonServers::ifServerNameNotExist(const std::string &sName) const
{
	size_t size = aServers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (sName == aServers[i].getServerName())
			return false;
	}
	return true;
}


const VirtualServer& CommonServers::whichServer( const std::string &sName ) const
{
	size_t size = aServers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (sName == aServers[i].getServerName())
			return aServers[i];
	}
	return aServers[0];
}
