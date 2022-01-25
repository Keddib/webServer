#include "headers.hpp"

#define SERVER "server"
#define openBracket '{'
#define closingBracket '}'
#define AND '&'
#define equal '='


#define CONFIG_FILE "/Users/keddib/Desktop/Keddib/42Projects/WebServ/config/default.conf"


int parseConfigFile( std::string file = "" )
{

	if (file.size() == 0)
		file = CONFIG_FILE;

	std::cout << file;
	// std::ifstream is RAII, i.e. no need to call close
	std::ifstream cFile(file);
	if (cFile.is_open())
	{
		std::string line;
		while(getline(cFile, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			if(line.empty() || line[0] == '#')
				continue;
			std::cout << line << "\n";
		}
	}
	else {
		std::cerr << "Couldn't open config file for reading.\n";
		return 1;
	}
	return 1;
}
