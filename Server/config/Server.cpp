#include "Server.hpp"

Server::Server(const Server &other)
{

}

Server::Server()
{
}

Server::~Server()
{
}

Server& Server::operator=(const Server &other)
{

}

uint32_t Server::getAddress() const
{
	return _address;
}

uint16_t Server::getPort() const
{
	return _port;
}

const std::string& Server::getServerName() const
{
	return _serverName;
}

const std::string& Server::getRoot() const
{
	return _root;
}

int Server::getClientMaxBodySize() const
{
	return _clientMaxBodySize;
}

bool Server::isAutoIndexOn() const
{
	return _autoIndex;
}
