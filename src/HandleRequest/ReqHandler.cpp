#include "ReqHandler.hpp"

ReqHandler::ReqHandler()
{}

ReqHandler::~ReqHandler()
{

}

Response* ReqHandler::getResponse(const Request &req)
{
	size_t index = req.getCSindex();
	Location const *loc = &ServI[index].whichServer(req.getHostName()).whichLocation(req.getResource());
	loc->Display();

	// check if req.method is accepted on location
	// check http version if 0 set connection to close/ KA to 0;
	// if redirect return redirect response
	// if resource is file check if exist
		// if exist and not CGI script open file and return response
	return NULL;
}
