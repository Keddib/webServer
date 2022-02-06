#include "configuration/ConfigParser.hpp"
#include "HandleRequest/ReqHandler.hpp"
#include "HandleRequest/ErrorGen.hpp"

ServersInterface ServI;



int main()
{
	try {
		ConfigParser parser("/Users/keddib/Desktop/Keddib/42Projects/webServer/src/config/default.conf");
		parser.parse();
		parser.AddServersToServersInterface();
	} catch(std::string &s) {
		std::cerr << "Error -> " << s;
		return 1;
	}
	ServI.Display();
	std::cout << "example of error response\n";
	ErrorGen errGenerator;
	Response *res = errGenerator.getResponse(0, 400);
	res->display();
		// CommonServers &cs = srvs[1];
		// const VirtualServer &myserver = cs.whichServer("site1.com");
		// const Location &myLoc = myserver.wichLocation("/photos/index.html");
		// myLoc.Display();

	return 0;
}
