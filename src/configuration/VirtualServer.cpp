#include "VirtualServer.hpp"

VirtualServer::VirtualServer(const std::pair<unsigned int, int> &srv)
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
	std::cout << "Host :" << aServerInfo.first << " Port: " << aServerInfo.second << std::endl;
}
