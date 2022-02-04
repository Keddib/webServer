#ifndef HEADERS_001_HPP
#define HEADERS_001_HPP

#include "ServersInterface.hpp"
#include <sstream>
#include <fstream>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

#include "../MACROS.hpp"

/*
** global object of serversInterface it's used by almost every peace of code
*/


// helper functions

bool notSpace(unsigned char ch);
void ft_split(const std::string &s, std::vector<std::string> &strs);
void simpleTokenizer(std::string &line, std::vector<std::string> &strs);
int isValidMethod(const std::string &meth);
// add helper func to check valid ip address
// add helper func to check valid port

#endif
