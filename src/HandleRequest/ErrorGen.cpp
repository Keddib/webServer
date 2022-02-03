#include "ErrorGen.hpp"


ErrorGen::ErrorGen() {}

ErrorGen::~ErrorGen(/* args */) {}

/*
** this functions is used when an error accured during request parsing
** before sending error response to client we check if there is a user defined
** error page
*/

Response *ErrorGen::getResponse(size_t server, int error, const std::string &Host = "")
{
	// if valid
	Response *res;
	// check if the user defined an error page for the error accured
	// if not we use out defualt error pages
	const std::vector<std::pair<int, std::string> > &Errors =
	ServI[server].whichServer(Host).getErrorPages();
	std::string errorPagePath;
	// loop over error pages and check if the server has error pages
	for(size_t i=0; i < Errors.size(); i++)
	{
		if (Errors[i].first == error)
			errorPagePath = Errors[i].second;
	}
	// we check if a error page path is correct
	// if not we use our default one
	if (!errorPagePath.empty())
	{
		//check if path is valid and create a response with the file in body
	}

	// if not valid we create a respone with

	Response *res = new Response();
	return res;
}
