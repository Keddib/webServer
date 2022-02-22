#ifndef SERVERS_INTERFACE_001_HPP
#define SERVERS_INTERFACE_001_HPP

// #include "../Includes.hpp"
#include <vector>
#include "CommonServers.hpp"
#include "../MACROS.hpp"
#include <unistd.h>
#include <stdarg.h>
#include <cstring>
#include <map>


class ServersInterface
{
	private:
		std::vector<CommonServers>	aServers;
	public:
		typedef	std::vector<CommonServers>::iterator	iterator;
		ServersInterface();
		ServersInterface(const std::vector<VirtualServer> &srvs);
		void	AddServer(const VirtualServer &srv);
		void	Display() const;
		bool	LaunchServers(std::map<int, int> &) const;
		std::pair<int, int>	StartServerAt(int, bool &) const;
		CommonServers	&operator[](int index);
		iterator	begin();
		iterator	end();
};

#endif
