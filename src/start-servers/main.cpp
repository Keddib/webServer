#include "global.hpp"
#include "../configuration/ConfigParser.hpp"
#include "./manage-request.hpp"
#include <signal.h>


void	onWriteFail(int)
{
	g_client_closed = true;
}

void __Error()
{
	std::cerr << "too many arguments. See 'Webserv --help'.\n";
	exit (1);
}

void __help()
{
	std::cout << "usage: Webserv [--help] [-t path_to_config_file] [path_to_config_file]\n\n";
	std::cout << "None                      \tStart server with default config\n\n";
	std::cout << "[--help]                  \tshow command usage\n\n";
	std::cout << "[-t] [path_to_config_file]\t check configuration\n\n";
	std::cout << "[path_to_config_file]     \tStart server\n";
	exit(0);
}

std::string hundleArgs(int argc, char **argv)
{
	if (argc > 3)
		__Error();
	if (argc == 2)
	{
		if (std::string(argv[1]) == "--help")
			__help();
		else
			return argv[1];
	}
	else if (argc == 3)
	{
		if (std::string(argv[1]) == "-t") // check config argv[1]
		{
			try {
				ConfigParser parser(argv[2]);
				parser.parse();
			} catch(std::string &s) {
				std::cerr << "Error -> " << s <<"[KO]\n";
				exit(1);
			}
			std::cout << "Webserv: the configuration file " << argv[2] << " syntax is ok\n";
			std::cout << "Webserv: configuration file " << argv[2] << " test is successful\t[OK]\n";
			exit(0);
		}
		else
			__help();
	}
	return CONFIG_FILE;
}

int main(int argc, char **argv)
{
	std::string file = hundleArgs(argc , argv);
	signal(SIGPIPE, onWriteFail);
	// just for debugging
	std::cout << getpid() << std::endl;
	std::map<int, int>	FdToIndex; // map from fd to index in common servers vector that is inside ServersInterface
	try {
		ConfigParser parser(file);
		parser.parse();
		parser.AddServersToServersInterface();
	} catch(std::string &s) {
		std::cerr << "Error -> " << s << "[KO]\n";
		return 1;
	}
	if (ServI.LaunchServers(FdToIndex))
		std::cout << "START WAS SUCCESSFULL\n";
	else
		exit(1);
	ManageRequest manage(FdToIndex);
	//manage.StartListening();
	manage.EP_StartLIstening();
	return 0;
}
