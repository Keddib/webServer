#ifndef	__MANAGE_REQUEST__
#define	__MANAGE_REQUEST__

#include "requset.hpp"
#define	BUFFER_SIZE	1024 // make sure that this number is fiting in int type
class	ManageRequest
{
	private:
		int			maxFd;
		const	static int	read_nb = BUFFER_SIZE; // this number will be used to read number of bytes at each time data is available
		fd_set	read_set, write_set, all_fds;
		std::map<int, int>	&aFdToIndex;
		std::map<int, Request>	fdToRequest;
		char			buffer[read_nb * 2];
	public:
		ManageRequest(std::map<int, int> &fdToIndex);
		void		StartListening();
	private:
		bool	ConstructRequest(std::map<int, Request>::iterator &req);
		void		Select();
};


#endif