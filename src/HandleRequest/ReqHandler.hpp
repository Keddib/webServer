#ifndef REQ_HANDLER_001_HPP
#define REQ_HANDLER_001_HPP

#include "../configuration/confHeaders.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <string>
#include <unistd.h>

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
