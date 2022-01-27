#ifndef HEADERS_001_HPP
#define HEADERS_001_HPP

#include "VirtualServer.hpp"
#include "CommonServers.hpp"
#include "ServersInterface.hpp"
#include "Location.hpp"
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <arpa/inet.h>

#define CONFIG_FILE "/Users/keddib/Desktop/Keddib/42Projects/WebServ/config/default.conf"
#define D_PORT 8000

/*
** define allowed directives
*/

#define HOST 1
#define PORT 2
#define S_NAME 3
#define ROOT 4
#define INDEX 5
#define AUTO 6
#define CMBS 7
#define ERRPG 8
#define ACCMETH 9
#define LOCATION 10
#define RET 11
#define UPLD 12
#define CGI 13
#define NONE -1

// helper functions

bool notSpace(unsigned char ch);
void ft_split(const std::string &s, std::vector<std::string> &strs);
void simpleTokenizer(std::string &line, std::vector<std::string> &strs);
bool isValidMethod(const std::string &meth);

#endif
