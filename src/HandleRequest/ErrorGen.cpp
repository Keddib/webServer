#include "ErrorGen.hpp"


ErrorGen::ErrorGen() {}

ErrorGen::~ErrorGen(/* args */) {}


/*
** this functions is used when an error accured during request parsing
** before sending error response to client we check if there is a user defined
** error page
*/

Response *ErrorGen::getResponse(size_t server, int error, const std::string &Host,const std::vector<std::string> &head)
{
	Response *res = new Response();
	res->setStartLine("HTTP/1.1", error, getErrorMessage(error));
	res->setHeader("Connection", "close", 0);
	res->setKeepAlive(false);
	res->setHeader(head); // add bunch of header specific to an the error;
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
	FileInfo fData;
	// we check if a error page path is correct we use it as response body
	if (!errorPagePath.empty() && (getFileInfo(errorPagePath, fData) == 0))
	{
		// check file type and add content-type header
		res->setHeader("Content-Type", fData.Ftype, 0);
		// check file size and add content-lenght header
		res->setHeader("Content-Length", std::to_string(fData.size), 0);
		res->setHeader("Last-Modified", fData.Mtime, 1);
		// add file to body of request;
		res->setBodyfile(errorPagePath);
	}
	// if not we use our default one
	else
		setDefaultErrorPage(res, getErrorPage(error));
	return res;
}


void ErrorGen::setDefaultErrorPage(Response *res, const char *errpage)
{
	size_t lenght = std::strlen(errpage);
	// add content-type header text/html;
	// add content-lenght header ;
	res->setHeader("Content-Type", "text/html", 0);
	res->setHeader("Content-Length", std::to_string(lenght), 1);
	res->addBodyToBuffer(errpage);
}
