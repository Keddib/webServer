#include "ConfigParser.hpp"


int main()
{
	try {
		ConfigParser parser;
		parser.parse();
	} catch(std::string &s) {
		std::cout << s;
	}
	ServersInterface srvs;
	parser.getServers(srvs);
	return 0;
}
