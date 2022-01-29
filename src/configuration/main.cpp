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
		return 1;
	}
	srvs.Display();
	// CommonServers &cs = srvs[1];
	// const VirtualServer &myserver = cs.whichServer("site1.com");
	// const Location &myLoc = myserver.wichLocation("/photos/index.html");
	// myLoc.Display();
	return 0;
}
