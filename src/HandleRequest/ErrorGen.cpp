#include "ErrorGen.hpp"


ErrorGen::ErrorGen() {}

ErrorGen::~ErrorGen(/* args */) {}


/*
** this functions is used when an error accured during request parsing
** before sending error response to client we check if there is a user defined
** error page
*/

Response *ErrorGen::getResponse(
	size_t server, int error,
	const std::string &Host,
	bool keepAlive,
	const std::vector<std::string> &head)
{
	Response *res = new Response();
	res->setCommonServerIndex(server);
	res->setStartLine("HTTP/1.1", error, getErrorMessage(error));
	res->setHeader(head); // add bunch of header specific to an the error;
	if (!keepAlive)
	{
		res->setHeader("Connection", "close");
		res->setKeepAlive(false);
	}
	// check if the user defined an error page for the error accured
	// if not we use out defualt error pages
	std::string errorPagePath;
	if (error >= 400)
	{
		const std::vector<std::pair<int, std::string> > &Errors =
		ServI[server].whichServer(Host).getErrorPages();
		// loop over error pages and check if the server has error pages
		for(size_t i=0; i < Errors.size(); i++)
		{
			if (Errors[i].first == error)
				errorPagePath = Errors[i].second;
		}
	}
	FileInfo fData;
	// we check if a error page path is correct we use it as response body
	if (!errorPagePath.empty() && (getFileInfo(errorPagePath, fData) == 0))
	{
		// check file type and add content-type header
		res->setHeader("Content-Type", fData.Ftype);
		// check file size and add content-lenght header
		res->setHeader("Content-Length", to_string(fData.size));
		res->setBodySize(fData.size);
		res->setHeader("Last-Modified", fData.Mtime, 1);
		// add file to body of request;
		res->setBodyfile(errorPagePath);
	}
	// if not we use our default one
	else
	{
		std::string errorPage;
		getErrorPage(error, errorPage);
		setDefaultErrorPage(res, errorPage.c_str());
	}
	// res->display();
	// exit(1);
	return res;
}


void ErrorGen::setDefaultErrorPage(Response *res, const char *errpage)
{
	size_t lenght = std::strlen(errpage);
	// add content-type header text/html;
	// add content-lenght header ;
	res->setHeader("Content-Type", "text/html");
	res->setHeader("Content-Length", to_string(lenght), 1);
	res->addBodyToBuffer(errpage);
}



Response *ErrorGen::get304Respone(size_t server, const std::string &Time, bool kAlive)
{
	Response *res = new Response();
	res->setCommonServerIndex(server);
	res->setStartLine("HTTP/1.1", 304, "Not Modified");
	if (!kAlive)
	{
		res->setHeader("Connection", "close");
		res->setKeepAlive(false);
	}
	res->setHeader("Last-Modified", Time, 1);
	return res;
}
