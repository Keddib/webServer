#ifndef	__MANAGE_REQUEST__
#define	__MANAGE_REQUEST__

#include "requset.hpp"
#include "response-wrapper.hpp"
#include "../MACROS.hpp"
#include <sys/epoll.h>
#include <stdio.h>

class	ManageRequest
{


	private:
		unsigned int								numOfFds;
		// new stuff related to epoll
		struct sockaddr_in							client_addr;
		socklen_t									address_len;
		struct epoll_event 							event;
		int											epoll_fd;
		std::vector<struct epoll_event>				ready_fds;
		short										MSFDN;
		int											curReady;
		int											epoll_struct_not_needed;
		const	static int							read_nb = BUFFER_SIZE; // this number will be used to read number of bytes at each time data is available
		std::map<int, int>							&aFdToIndex;
		std::map<int, Request>						fdToRequest;
		std::map<int, ResponseWrapper>				fdToResponse;
		Response									*Restmp;
		char										buffer[read_nb * 2];
		std::map<int, ResponseWrapper>::iterator	iter_to_res, tmp1_res_iter, tmp2_res_iter;
		std::map<int, Request>::iterator 			iter_to_req, tmp1_req_iter, tmp2_req_iter;
	public:
		ManageRequest(std::map<int, int>	&fdToIndex);
		void								EP_StartLIstening();
	private:
		void		cleanUnusedResponses();
		void		timeCheck();
		void		HandelNewConnection(int tmpFd, int curFd, int &newConnections);
		int			FDS_That_ready_for_IO(int &newConnections);
		void		WorkOnRequest(int curFd);
		void		WorkOnResponse(int curFd);
		bool		TimeOut(int fd);
		void		ListeningOnReadEnd(int);
		void		ListeningOnWriteEnd(int);
		Response	*ConstructRequest(std::map<int, Request>::iterator &req);
};


#endif
