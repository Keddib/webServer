#include "../Response/Response.hpp"
#include "../MACROS.hpp"
extern	bool	g_client_closed;

class	ResponseWrapper
{
	private:
		bool			resIsDone;
		std::time_t		lasTimeWereHere;
		Response 		*_com_response; // complete response
		std::fstream	&_body;
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
		~ResponseWrapper();
};
