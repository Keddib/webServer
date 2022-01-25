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
	aServers.push_back(server);
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
