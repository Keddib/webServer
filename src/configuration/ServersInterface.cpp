#include "ServersInterface.hpp"

void	ServersInterface::AddServer(const VirtualServer &srv)
{
	std::vector<CommonServers>::iterator first(aServers.begin());
	std::vector<CommonServers>::iterator last(aServers.end());

	while (first != last)
	{
		if (*first == srv)
		{
			// if server_name is identical this server get ignored
			// display a warning message like:
			// nginx: [warn] conflicting server name "_" on 0.0.0.0:80, ignored
			first->AddServer(srv);
			return ;
		}
		++first;
	}
	aServers.push_back(srv.GetHostAndPort());
	aServers.back().AddServer(srv);
}


ServersInterface::ServersInterface()
{

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

bool	close_prev_fds(std::map<int, int> &fdToIndex)
{
	std::map<int, int>::iterator first(fdToIndex.begin()), last(fdToIndex.end());

	while (first != last)
	{
		close(first->first); // maybe other way to close fd because i think close is not allowed
		++first;
	}
	return false;
}

bool	ServersInterface::LaunchServers(std::map<int, int> &fdToIndex) const
{
	// be carefaul what should happen if one fails to start
	// should close all servers that passed before
	// return true on success false on fail
	int size = aServers.size();
	std::pair<int, int>	tmp;
	bool			status;
	for (int i = 0; i < size; ++i)
	{
		tmp = StartServerAt(i, status);
		if (status)
			fdToIndex.insert(tmp);
		else
			return close_prev_fds(fdToIndex);
	}
	return true;
}

void	print_on_error(int n, ...)
{
	va_list ptr;

	va_start(ptr, n);

	for (int i = 0; i < n; i++)
	std::cout << "\033[1;31m" << va_arg(ptr, const char *);
	va_end(ptr);
	std::cout <<"\033[0m\n";
	exit(1);
}

std::pair<int, int>	ServersInterface::StartServerAt(int index, bool &status) const
{
	std::pair<unsigned int, int>	info(aServers[index].GetIpPortCommonServer());
	int			fd;
	struct sockaddr_in	tmp_addr;

	bzero(&tmp_addr, sizeof(tmp_addr));
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = info.second;
	tmp_addr.sin_addr.s_addr = info.first;

	status = true;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	//fcntl(fd, F_SETFL, O_NONBLOCK);// look carefully here
	if (fd < 0) {
		print_on_error(1, "socket faild to open");
		status = false;
		return std::pair<int, int>();
	}
	if (bind(fd, (struct sockaddr*)&tmp_addr, sizeof(tmp_addr)) < 0) {
		char str[INET_ADDRSTRLEN];
		struct sockaddr_in sa;
		sa.sin_addr.s_addr = aServers[index].GetIpPortCommonServer().first;
		inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
		print_on_error(2, "could not bind to: ", str);
		status = false;
		return std::pair<int, int>();
	}
	if (listen(fd, MAX_BACK_LOG) < 0) {
		print_on_error(1 , "check back log number");
		status = false;
		return std::pair<int, int>();
	}
	return std::make_pair(fd, index);
}
