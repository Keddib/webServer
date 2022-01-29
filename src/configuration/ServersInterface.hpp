#ifndef SERVERS_INTERFACE_001_HPP
#define SERVERS_INTERFACE_001_HPP

#include "CommonServers.hpp"

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
		// operator [] i will give it host , port pair and will return refernce
		CommonServers	&operator[](int index);
		iterator	begin();
		iterator	end();
};

#endif
