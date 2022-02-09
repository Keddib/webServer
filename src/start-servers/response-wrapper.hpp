#include "../HandleRequest/Response.hpp"
#include <ctime>
#include "../MACROS.hpp"
class	ResponseWrapper
{
	private:
		std::time_t	startTime;
		Response 	*_com_response; // complete response
		std::fstream	&_body;
		const char*	_buffer; // most of time will hold just header
		unsigned int	bodySize;
		unsigned int	hasBeenRead;	
		size_t		_buffer_size;
	private:
		bool		SendingHeader(int fd, int &required_size);
		bool		SendingBody(int fd, char *storage_elment, int required_size);
	public:
		void		Free_Com_response();
		int		getCommonSrvIndex() const;
		bool		isStillValid(std::time_t);
		bool		CloseConnection() const;
		bool		SendingResponse(int fd, char *storage_elment, int required_size);
		ResponseWrapper(Response *rsp);
};
