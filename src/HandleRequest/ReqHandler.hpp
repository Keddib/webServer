#ifndef REQ_HANDLER_001_HPP
#define REQ_HANDLER_001_HPP

#include "../configuration/ServersInterface.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <string>
#include <unistd.h>

class ReqHandler
{
	private:
		ServersInterface &_SI;
		/* data */

	public:
		ReqHandler(ServersInterface &SI);
		~ReqHandler();
		Response* getResponse(const Request&);
};

#endif
