#ifndef SERVER_001_HPP
#define SERVER_001_HPP

#include <string>
#include <iostream>
#include <vector>

#define	ON	1
#define	OFF	0

class Server
{
	private:
		uint32_t _address;
		uint16_t _port;
		int _clientMaxBodySize;
		std::string _serverName;
		std::string _root;
		bool _autoIndex;
		std::vector<std::string> _indexFiles;
	public:
		Server();
		Server(const Server &);
		Server& operator=(const Server &);
		~Server();
		// getters
		uint32_t getAddress() const;
		uint16_t getPort() const;
		int getClientMaxBodySize() const;
		const std::string& getServerName() const;
		const std::string& getRoot() const;
		bool isAutoIndexOn() const;
		// setters
};

#endif
