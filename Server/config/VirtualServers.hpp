#ifndef VIRTUALSERVERS_19_HPP
#define VIRTUALSERVERS_19_HPP

#include <string>
#include <fstream>

class VirtualServers
{
	private:
		std::ifsream _configFile;
		bool _isFileFound;
	public:
		VirtualServers(const char *fileName);
		~VirtualServers();
};

VirtualServers::VirtualServers(const char *fileName)
: _configFile(fileName), _isFileFound(0)
{
	if (_configFile.is_open())
		_isFileFound = 1;
}

VirtualServers::~VirtualServers()
{
}

#endif
