#ifndef REQ_HANDLER_001_HPP
#define REQ_HANDLER_001_HPP

#include "../configuration/confHeaders.hpp"
#include "../start-servers/requset.hpp"
#include "Response.hpp"
#include <string>
#include <unistd.h>

extern ServersInterface ServI;

class ReqHandler
{
	private:
		/* data */

	public:
		ReqHandler();
		~ReqHandler();
		Response* getResponse(const Request&);
};

#endif
