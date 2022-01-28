#include "VirtualServer.hpp"

VirtualServer::VirtualServer(const std::pair<unsigned int, int> &srv)
: _serverName("_"),
_client_max_body_size(1048576)
{
	aServerInfo = srv;
}


bool	VirtualServer::operator==(const VirtualServer &rhs)
{
	return (aServerInfo == rhs.aServerInfo);
}


std::pair<unsigned int, int>	VirtualServer::GetHostAndPort() const
{
	return aServerInfo;
}


void	 VirtualServer::Display() const
{
	std::cout << "server :\n";
	std::cout << "\tHost :" << aServerInfo.first << " Port: " << aServerInfo.second << '\n';
	std::cout << "\tserver name: " << _serverName + "\n" <<
	"\t_MaxBodySize: " << std::to_string(_client_max_body_size) + "\n";
	for (size_t i = 0; i < _locations.size(); i++)
		_locations[i].Display();
}


void VirtualServer::setHostValue(unsigned int Addr)
{
	// inet_addr resolve adress string to binary representation
	// of address in net byte order
	// aServerInfo.first = inet_addr(host.c_str());
	aServerInfo.first = Addr;
}

void VirtualServer::setPortValue(int port)
{
	// htons return network byte order format
	// aServerInfo.second = htons(std::atol(port.c_str()));
	aServerInfo.second = port;
}

void VirtualServer::setServerNameValue(const std::string &sName)
{
	_serverName = sName;
}

void VirtualServer::setMaxBodySize(unsigned long long max)
{
	_client_max_body_size = max;
}

void VirtualServer::addLocation(const Location &loc)
{
	_locations.push_back(loc);
}

std::vector<Location>& VirtualServer::getLocationsToEdit()
{
	return _locations;
}
