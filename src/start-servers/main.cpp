#include "../configuration/ConfigParser.hpp"
#include "./manage-request.hpp"


int main()
{

	// just for debugging
	std::cout << getpid() << std::endl;

	ServersInterface srvs;
	std::map<int, int>	FdToIndex; // map from fd to index in common servers vector that is inside ServersInterface
	try {
		ConfigParser parser;
		parser.parse();
		parser.getServers(srvs);
	} catch(std::string &s) {
		std::cerr << "Error -> " << s;
		return 1;
	}
	if (srvs.LaunchServers(FdToIndex))
		std::cout << "START WAS SUCCESSFULL\n";
	else
		exit(1);

	ManageRequest manage(FdToIndex);


	manage.StartListening();
	
	return 0;
}
