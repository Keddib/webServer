#ifndef ERROR_GEN_001_HPP
#define ERROR_GEN_001_HPP

#include "../configuration/confHeaders.hpp"
#include "Response.hpp"
#include "utils.hpp"

#define	READ	0
#define	WRITE	1

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
		Response *getResponse(size_t server, int error,
			const std::string &Host = "",
			bool keepAlive = 0,
			const std::vector<std::string> &head = std::vector<std::string>()
			);
		Response *get304Respone(size_t server, const std::string &time, bool kAlive);
	private:
		void setDefaultErrorPage(Response *res, const char *errpage);
};

#endif
