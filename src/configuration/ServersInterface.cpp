#include "ServersInterface.hpp"

void	ServersInterface::AddServer(const VirtualServer &srv)
{
	std::vector<CommonServers>::iterator first(aServers.begin());
	std::vector<CommonServers>::iterator last(aServers.end());

	while (first != last)
	{
		if (*first == srv)
		{
			first->AddServer(srv);
			return ;
		}
		++first;
	}
	aServers.push_back(srv.GetHostAndPort());
	aServers.back().AddServer(srv);
}


ServersInterface::ServersInterface(const std::vector<VirtualServer> &srvs)
{
	std::vector<VirtualServer>::const_iterator first, last;

	first = srvs.begin();
	last = srvs.end();
	while (first != last)
	{
		this->AddServer(*first);
		++first;
	}
}

void	ServersInterface::Display() const
{
	std::vector<CommonServers>::const_iterator first(aServers.begin()), last(aServers.end());
	while (first != last)
	{
		first->Display();
		++first;
	}
}

CommonServers	&ServersInterface::operator[](int index)
{
	return aServers[index];
}


ServersInterface::iterator	ServersInterface::begin()
{
	return aServers.begin();
}

ServersInterface::iterator	ServersInterface::end()
{
	return aServers.end();
}
