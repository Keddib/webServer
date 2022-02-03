#ifndef ERROR_GEN_001_HPP
#define ERROR_GEN_001_HPP

#include "../configuration/confHeaders.hpp"
#include "Response.hpp"
#include <string>

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
};

#endif
