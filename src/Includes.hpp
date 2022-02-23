#ifndef UTILS_07_HPP
#define UTILS_07_HPP

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <netdb.h>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

struct	FileInfo
{
	int64_t		size;
	std::string	Mtime;
	std::string Ftype;
	bool		keepAlive;
};


struct CGIIresInfo{
	CGIIresInfo()
	: cont_type(false), location(false), error(0)
	{ status.first = false; status.second = 200; bodyExist = false;}
	bool							cont_type;
	bool							location;
	std::pair<bool, short>			status;
	short							bodyExist;
	int								error;
};


// helper functions
bool notSpace(unsigned char ch);
void ft_split(const std::string &s, std::vector<std::string> &strs);
void simpleTokenizer(std::string &line, std::vector<std::string> &strs);
int isValidMethod(const std::string &meth);
std::string ConvertIPtoString(uint32_t IP);
bool isFileAccessible(const std::string &path);
int getFileInfo(const std::string &fileName, FileInfo &fileI);
std::string getFileType(const std::string &path);
void getErrorPage(int error, std::string &page);
const char *getErrorMessage(int error);
std::string getOsName();
std::string getDate();
std::string ListDirectory(const std::string &Dir, const std::string &name);
std::string lookForIndexInDirectory(const std::string &Dir, const std::vector<std::string> &indexes, int &error);
std::string getMyIP();
char asciitolower(char in);
char asciiToUpper(char in);
std::string	to_string(long long n);
bool fileHasextension(const std::string &file, const std::string &exten);
unsigned int getFileSize(const char *file);
std::string get_method(int meth);
std::string getServerPort(size_t server);
void freeArray(char **arr);
int	IndexOf(const char *str, char c);
bool	my_strncmp(const char *s1, const char *s2, size_t n);

#endif
