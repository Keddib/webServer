#ifndef ERROR_GEN_001_HPP
#define ERROR_GEN_001_HPP

#include "../configuration/confHeaders.hpp"
#include "Response.hpp"
#include <string>
#include <cstring>

extern ServersInterface ServI;

/*
** this class is used when an error accured during request parsing
** before sending error response to client we check if there is a user defined
** error page
*/



class ErrorGen
{
	public:
		ErrorGen();
		~ErrorGen();
		Response *getResponse(size_t server, int error, const std::string &Host = "");
	private:
		void setDefaultErrorPage(Response *res, const char *errpage);
};

bool isFileExiste(const std::string &path);
int getFileSize(const std::string &filename);
std::string getFileType(const std::string &path);
std::string getFileLastModifiedTime(const std::string &fileName);
const char *getErrorPage(int error);
const char *getErrorMessage(int error);


#endif
