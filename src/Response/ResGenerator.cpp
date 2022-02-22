#include "ResGenerator.hpp"

ResGenerator::ResGenerator() {}

ResGenerator::~ResGenerator(/* args */) {}

/*
** this class is used to generate response :
** -> error response
** -> redirect response
** -> cach response
*/

Response *ResGenerator::createResponse(size_t server, int code)
{
	Response *res = new Response();
	res->setStartLine("HTTP/1.1", code, getErrorMessage(code));
	return res;
}

Response *ResGenerator::getRedirecResponse(size_t server, int code,
			const std::string &Location,
			const std::string &Host,
			bool keepAlive
			)
{
	Response *res = createResponse(server, code);
	// add location
	res->setHeader(Location);
	setConnection(res, keepAlive);
	std::string errorPage;
	getErrorPage(code, errorPage);
	setDefaultErrorPage(res, errorPage.c_str());
	return res;
}
Response *ResGenerator::get200Response(size_t server, const std::string &fileName, FileInfo &Fdata)
{
	Response *res = createResponse(server, 200);
	setConnection(res, Fdata.keepAlive);
	res->setHeader("Content-Type", Fdata.Ftype);
	res->setHeader("Content-Length", to_string(Fdata.size));
	res->setBodySize(Fdata.size);
	res->setHeader("Last-Modified", Fdata.Mtime, 1);
	res->setBodyfile(fileName);
	res->display();
	return res;
}

Response *ResGenerator::getErrorResponse(
	size_t server,
	int error,
	const std::string &Host,
	bool keepAlive,
	const std::string &Header
	)
{
	Response *res = createResponse(server, error);
	setConnection(res, keepAlive);
	res->setHeader(Header);
	if (setUserErrorPage(res, server, Host, error))
		return res;
	else { // if not we use our default one
		std::string errorPage;
		getErrorPage(error, errorPage);
		setDefaultErrorPage(res, errorPage.c_str());
	}
	return res;
}

Response* ResGenerator::getErrorResponse( Response *res, int error, const std::string &Host )
{
	res->setStartLine("HTTP/1.1", error, getErrorMessage(error));
	setConnection(res, res->isKeepAlive());
	if (setUserErrorPage(res, res->getCommonServerIndex(), Host, error))
		return res;
	else { // if not we use our default one
		std::string errorPage;
		getErrorPage(error, errorPage);
		setDefaultErrorPage(res, errorPage.c_str());
	}
	return res;
}

bool ResGenerator::setUserErrorPage(Response *res, size_t server, const std::string &Host, int error)
{
	std::string errorPagePath;
	const std::vector<std::pair<int, std::string> > &Errors =
	ServI[server].whichServer(Host).getErrorPages();
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
		res->setHeader("Content-Type", fData.Ftype);
		// check file size and add content-lenght header
		res->setHeader("Content-Length", to_string(fData.size));
		res->setBodySize(fData.size);
		res->setHeader("Last-Modified", fData.Mtime, 1);
		// add file to body of request;
		res->setBodyfile(errorPagePath);
		return true;
	}
	return false;
}

void ResGenerator::setConnection(Response *res, bool keepAlive)
{
	if (!keepAlive)
	{
		res->setHeader("Connection", "close");
		res->setKeepAlive(false);
	} else
		res->setHeader("Connection", "keep-alive");
}

void ResGenerator::setDefaultErrorPage(Response *res, const char *errpage)
{
	size_t lenght = std::strlen(errpage);
	// add content-type header text/html;
	// add content-lenght header ;
	res->setHeader("Content-Type", "text/html");
	res->setHeader("Content-Length", to_string(lenght), 1);
	res->addBodyToBuffer(errpage);
}

Response *ResGenerator::get304Response(size_t server, const std::string &Time, bool kAlive)
{
	Response *res = createResponse(server, 304);
	setConnection(res, kAlive);
	res->setHeader("Last-Modified", Time, 1);
	return res;
}

Response *ResGenerator::get204Response(size_t server, bool keepAlive)
{
	Response *res = createResponse(server, 204);
	setConnection(res, keepAlive);
	res->setHeader("\r\n");
	return res;
}

Response *ResGenerator::GetDirListingResponse(const std::string &PATH, const std::string &name,  size_t server, bool connection)
{
	Response *res = createResponse(server, 204);
	setConnection(res, connection);
	res->setHeader("Content-Type", "text/html");
	const std::string &s = ListDirectory(PATH, name);
	size_t size = s.size();
	res->setBodySize(size);
	// content lenght
	res->setHeader("Content-Length", to_string(size), 1);
	res->addBodyToBuffer(s);
	return res;
}
