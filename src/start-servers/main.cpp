#include "global.hpp"
#include "../configuration/ConfigParser.hpp"
#include "./manage-request.hpp"
#include <signal.h>


void	onWriteFail(int)
{
	g_client_closed = true;
	std::cerr << "CLIENT HAS closed connection before complete responsing\n";
}

int main()
{
	signal(SIGPIPE, onWriteFail);
	// just for debugging
	std::cout << getpid() << std::endl;

	std::map<int, int>	FdToIndex; // map from fd to index in common servers vector that is inside ServersInterface
	try {
		ConfigParser parser;
		parser.parse();
		parser.AddServersToServersInterface();
	} catch(std::string &s) {
		std::cerr << "Error -> " << s;
		return 1;
	}
	if (ServI.LaunchServers(FdToIndex))
		std::cout << "START WAS SUCCESSFULL\n";
	else
		exit(1);


	// ErrorGen p;

	// Response *l = p.getResponse(0, 405);
	// l->display();
	ManageRequest manage(FdToIndex);


	//manage.StartListening();
	manage.EP_StartLIstening();

	return 0;
}
