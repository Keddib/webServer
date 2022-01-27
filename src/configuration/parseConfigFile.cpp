#include "headers.hpp"
#include <sstream>

#define CONFIG_FILE "/Users/keddib/Desktop/Keddib/42Projects/WebServ/config/default.conf"

bool notSpace(unsigned char ch)
{
	return !std::isspace(ch);
}

void removeSpacesFromStart(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
}

void removeSpacesFromEnd(std::string &s)
{
	s.erase(s.find_last_not_of(" \t") + 1);
}


void simpleTokenizer(std::string &line, std::vector<std::string> &tokens)
{
	std::stringstream ss(line);
	std::string token;
	while (ss >> token) {
		tokens.push_back(token);
	}
}


bool serverBlock( std::string &line )
{
	static bool isServer(0);
	static bool isOpen(0);

	std::vector<std::string> tokens;
	simpleTokenizer(line, tokens);
	if (isS)
	if (tokens.size() > 2)
		throw "unexpected token\n";
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "server")
			isServer = 1;
		else if (tokens[i] == "{")
			isOpen = 1;
	}

	return true;
}



int parseConfigFile( std::string file )
{

	if (file.size() == 0)
		file = CONFIG_FILE;
	size_t lineNum(0);
	// std::ifstream is RAII, i.e. no need to call close
	std::ifstream cFile(file);
	if (cFile.is_open())
	{
		std::string line;
		while(getline(cFile, line))
		{
			lineNum++;
			removeSpacesFromStart(line);
			if(line.empty() || line[0] == '#')
				continue;
			removeSpacesFromEnd(line);
			try {
				serverBlock(line);
			} catch( const char *err ) {
				std::cerr << "ConfigFile: ("<< lineNum << ") " << err;
			}
			std::cout << line << "\n";
		}
	}
	else {
		std::cerr << "ConfigFile: Couldn't open " << file + "\n";
		return 1;
	}
	return 1;
}
