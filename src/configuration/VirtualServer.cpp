#include "VirtualServer.hpp"

VirtualServer::VirtualServer(const std::pair<unsigned int, int> &srv)
: _serverName("_"), _client_max_body_size(1048576),  _errLocation("")
{
	aServerInfo = srv;
	_errLocation.setRoot(DEFAULT_ROOT);
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
	"\t_MaxBodySize: " << to_string(_client_max_body_size) + "\n";

	for (size_t i = 0; i < _errorPages.size(); i++)
		std::cout << "\t error_page : " << to_string(_errorPages[i].first) + " " << _errorPages[i].second + "\n";
	for (size_t i = 0; i < _locations.size(); i++)
		_locations[i].Display();
}


void VirtualServer::setHostValue(std::string &host)
{
	// inet_addr resolve adress string to binary representation
	// of address in net byte order
	aServerInfo.first = inet_addr(host.c_str());
}

void VirtualServer::setPortValue(int port)
{
	// htons return network byte order format
	aServerInfo.second = htons(port);
}

void VirtualServer::setServerNameValue(const std::string &sName)
{
	_serverName = sName;
}

void VirtualServer::setMaxBodySize(unsigned long long max)
{
	_client_max_body_size = max;
}

void VirtualServer::setErrorPage(int code, const std::string& path)
{
	std::pair<int, std::string> err;
	err.first = code;
	err.second = path;
	_errorPages.push_back(err);
}

void VirtualServer::addLocation(const Location &loc)
{
	_locations.push_back(loc);
}

std::vector<Location>& VirtualServer::getLocationsToEdit()
{
	return _locations;
}


const std::string& VirtualServer::getServerName() const
{
	return _serverName;
}

const Location& VirtualServer::whichLocation( const std::string &Path) const
{
	int max(0);
	int tmp_max(0);
	int index(-1);
	size_t size = _locations.size();
	for (size_t i = 0; i < size; i++)
	{
		tmp_max = findLocationURL(Path, _locations[i].getPrefix());
		if (tmp_max > max)
		{
			max = tmp_max;
			index = i;
		}
	}
	if (index == -1)
		return _errLocation;
	return _locations[index];
}

unsigned int VirtualServer::getClientMaxBodySize() const
{
	return _client_max_body_size;
}

const std::vector<std::pair<int, std::string> >& VirtualServer::getErrorPages() const
{
	return _errorPages;
}
