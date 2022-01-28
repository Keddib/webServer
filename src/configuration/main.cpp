#include "ConfigParser.hpp"


int main()
{
	ServersInterface srvs;
	try {
		ConfigParser parser;
		parser.parse();
		parser.getServers(srvs);
	} catch(std::string &s) {
		std::cerr << "Error -> " << s;
	}
	srvs.Display();
	return 0;
}
