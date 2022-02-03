#include "configuration/ConfigParser.hpp"
#include "HandleRequest/ReqHandler.hpp"



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
	ReqHandler hundler(srvs);
	Request req(0, POST, "/", 1);
	Response *res = hundler.getResponse(req);
	res = NULL;
	// srvs.Display();
	// CommonServers &cs = srvs[1];
	// const VirtualServer &myserver = cs.whichServer("site1.com");
	// const Location &myLoc = myserver.wichLocation("/photos/index.html");
	// myLoc.Display();
	return 0;
}
