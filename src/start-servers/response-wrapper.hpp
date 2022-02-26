#include "../Response/Response.hpp"
#include <poll.h>
#include "../MACROS.hpp"
extern	bool	g_client_closed;

class	ResponseWrapper
{
	private:
		struct	pollfd	read_poll_fd;
		short			failAttempts;
		bool			resIsDone;
		std::time_t		lasTimeWereHere;
		Response 		*_com_response; // complete response
		int				_body;
		const char*		_buffer; // most of time will hold just header
		unsigned int	bodySize;
		unsigned int	hasBeenRead;
		size_t			_buffer_size;
	private:
		bool		SendingHeader(int fd, int &required_size);
		bool		SendingBody(int fd, char *storage_elment, int required_size);
	public:
		bool		isFileUsed() const;
		bool		isStillValid();
		void		Free_Com_response();
		int			getCommonSrvIndex() const;
		bool		CloseConnection() const;
		bool		SendingResponse(int fd, char *storage_elment, int required_size);
		ResponseWrapper(Response *rsp);
		ResponseWrapper(const ResponseWrapper &cp);
		ResponseWrapper&operator=(const ResponseWrapper &rhs);
		~ResponseWrapper();
};
