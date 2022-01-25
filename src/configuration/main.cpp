#include "headers.hpp"


int parseConfigFile( std::string file = "" );


int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "WebServ/1.0 : too many arguments\n";
		return 1;
	}
	if (argc < 2)
		parseConfigFile();
	return 0;
}
