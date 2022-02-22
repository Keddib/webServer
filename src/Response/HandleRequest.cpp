#include "ReqHandler.hpp"

Response* HandleRequest(const Request &req)
{
	ReqHandler requestHand(req);
	return requestHand.getResponse();
}
