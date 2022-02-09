#include "manage-request.hpp"

ManageRequest::ManageRequest(std::map<int, int> &fti) : aFdToIndex(fti)
{
	int fd;
	std::map<int, int>::const_iterator first(aFdToIndex.begin()), last(aFdToIndex.end());
	epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		std::cout << "could not get epoll instance \n";
		exit(1);
	}
	while (first != last)
	{
		fd = first->first;
		ComFds.push_back(fd);
		event.data.fd = fd;
		event.events = EPOLLIN;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
		{
			std::cout << "could not this fd: " << first->first << "to epoll set\n";
			exit(1);
		}
		++first;
	}
	MSFDN = 1 + ComFds.back();
	ready_fds.reserve(ComFds.size() * 2); // just i'm assuming that could be all servers be accepting at the same time new connections so i need twice spac
}

Response	*ManageRequest::ConstructRequest(std::map<int, Request>::iterator &iter_to_req)
{
	int	read_data;
	// int	status;
	int	tot = 0;
	do {
		read_data = read(iter_to_req->first, buffer + tot, read_nb);
		if (read_data <= 0)
			break ;
		tot += read_data;
	}while (tot < read_nb);
	buffer[tot] = 0;
	return iter_to_req->second.AddToRequest(buffer, tot);
}

// epoll work here
void	ManageRequest::EP_StartLIstening()
{
	int curFd, tmpFd;
	int newConnections = 0;
	while (true)
	{
		curReady = FDS_That_ready_for_IO(newConnections);
		for (int i = 0; i < curReady; ++i)
		{
			curFd = ready_fds[i].data.fd;
			// i need first to check time out later!
			if (curFd < MSFDN)
			{
				// means that now i should accept new connction
				HandelNewConnection(tmpFd, curFd, newConnections);
			}
			else if (ready_fds[i].events & EPOLLIN)
			{
				// means that data direct towards some rewuest
				WorkOnRequest(curFd);
				
			}
			else
			{
				// means response
				WorkOnResponse(curFd);
			}
		}
	}
}

void	ManageRequest::WorkOnRequest(int curFd)
{
	iter_to_req = fdToRequest.find(curFd);
	Restmp = ConstructRequest(iter_to_req);
	if (Restmp)
	{
		// if we got inside means that request is done
		// start responding based on on the completed request
		// by the way do not close (i variable) here maybe you close it in response end
		fdToRequest.erase(curFd); // i will change this i will not remove it just call some rest on it and later decide if it will get removed or not  whene response is done if we would close connection i remove if not not removal required
		fdToResponse.insert(std::make_pair(curFd, ResponseWrapper(Restmp))); // this could leak if not deleted somewhere else just keep eye here
		event.data.ptr = NULL;
		event.data.fd = curFd;
		event.events = EPOLLOUT;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, curFd, &event) == -1)
		{
			std::cout << "could not change  to epoll set the fd: " << curFd << "\n";
			exit(1);
		}
	}
}

void	ManageRequest::WorkOnResponse(int curFd)
{
	iter_to_res = fdToResponse.find(curFd);
	if (iter_to_res != fdToResponse.end() &&  iter_to_res->second.SendingResponse(curFd, buffer, read_nb))
	{
		// if you are in this branch means that response is done
		// it is time to to check keepalive so you can decide if you will close or not base on it
		// and remove response from map
		event.data.fd = curFd;
		event.events = EPOLLIN;
		if (iter_to_res->second.CloseConnection())
		{
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, curFd, &event) == -1)
			{
				std::cout << "could not delete to epoll set the fd: " << curFd << "\n";
				exit(1);
			}
			close(curFd);
		}
		else
		{
			int srvFd = iter_to_res->second.getCommonSrvIndex();
			fdToRequest.insert(std::make_pair(curFd, Request(curFd, srvFd))); // this aFdToIndex[i] returns bad number
			if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, curFd, &event) == -1)
			{
				std::cout << "could not change some property  to epoll set the fd: " << curFd << "\n";
				exit(1);
			}
		}
		iter_to_res->second.Free_Com_response();
		fdToResponse.erase(curFd);
	}
}

void	ManageRequest::HandelNewConnection(int tmpFd, int curFd, int &newConnections)
{
	++newConnections;
	
	tmpFd = accept(curFd, NULL, NULL);
	fcntl(tmpFd, F_SETFL, O_NONBLOCK);
	ComFds.push_back(tmpFd);
	event.data.fd = tmpFd;
	event.events = EPOLLIN;
	
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tmpFd, &event) == -1)
	{
		std::cout << "could not add to epoll set the fd: " << tmpFd << "\n";
		exit(1);
	}
	fdToRequest.insert(std::make_pair(tmpFd, Request(curFd, aFdToIndex[curFd])));
	// give extra eye on aFdToIndex[curFd]
}


int	ManageRequest::FDS_That_ready_for_IO(int &newConnections)
{
	ready_fds.reserve(ready_fds.size() + newConnections);
	newConnections = 0;
	return epoll_wait(epoll_fd, ready_fds.data(), ComFds.size(), -1); // -1 for block
}










/*
void	ManageRequest::StartListening()
{
	// start accepting connections
	while (true)
	{
		Select();
		for (int i = 3; i < maxFd; ++i) // start i = 3 becaus 0 1 2 is defalut for read write error
		{
			if (TimeOut(i))
				continue ;
			if (FD_ISSET(i, &read_set))
			{
				ListeningOnReadEnd(i);
			}
			if (FD_ISSET(i, &write_set))
			{
				ListeningOnWriteEnd(i);
			}
		}
	}
}

///////
bool	ManageRequest::TimeOut(int curFd)
{
	iter_to_req = fdToRequest.find(curFd);
	if (iter_to_req != fdToRequest.end())
	{
		if (!iter_to_req->second.isStillValid(std::time(NULL)))
		{
			FD_CLR(curFd, &all_fds);
			close(curFd);
			fdToRequest.erase(iter_to_req);
			return true;
		}
	}
	else
	{
		iter_to_res = fdToResponse.find(curFd);
		if (iter_to_res != fdToResponse.end() &&  !iter_to_res->second.isStillValid(std::time(NULL)))
		{
			FD_CLR(curFd, &all_fds);
			close(curFd);
			fdToResponse.erase(iter_to_res);
			return true;
		}
	}
	return false;
}
///////
void	ManageRequest::ListeningOnWriteEnd(int curFd)
{
	iter_to_res = fdToResponse.find(curFd);
	if (iter_to_res != fdToResponse.end() &&  iter_to_res->second.SendingResponse(curFd, buffer, read_nb))
	{
		// if you are in this branch means that response is done
		// it is time to to check keepalive so you can decide if you will close or not base on it
		// and remove response from map
		if (iter_to_res->second.CloseConnection())
		{
			FD_CLR(curFd, &all_fds);
			close(curFd);
		}
		else
		{
			int srvFd = iter_to_res->second.getCommonSrvIndex();
			fdToRequest.insert(std::make_pair(curFd, Request(curFd, srvFd))); // this aFdToIndex[i] returns bad number
		}
		fdToResponse.erase(curFd);
	}
}



void	ManageRequest::ListeningOnReadEnd(int curFd)
{
	int tmpfd;
	if (aFdToIndex.count(curFd)) // this means that this fd is one of servers sockets
	{
		// i nead to accept connection
		tmpfd = accept(curFd, NULL, NULL);
		if (tmpfd + 1 > maxFd)
			maxFd = tmpfd + 1;
		fcntl(tmpfd, F_SETFL, O_NONBLOCK);
		FD_SET(tmpfd, &all_fds);
		fdToRequest.insert(std::make_pair(tmpfd, Request(curFd, aFdToIndex[curFd]))); // this aFdToIndex[i] returns bad number
		// the above line is saying in genral that this is tmpfd has relation with the request and in requset i store with sever should handel aka (i)
	}
	else
	{
		// means that client has sent some data
		// find the requset at i and call on  it  AddToRequest
		iter_to_req = fdToRequest.find(curFd);
		Restmp = ConstructRequest(iter_to_req);
		if (Restmp)
		{
			// if we got inside means that request is done
			// start responding based on on the completed request
			// by the way do not close (i variable) here maybe you close it in response end
			fdToRequest.erase(curFd);
			fdToResponse.insert(std::make_pair(curFd, ResponseWrapper(Restmp))); // this could leak if not deleted somewhere else just keep eye here
		}
	}
}


void	ManageRequest::Select()
{
	read_set = all_fds;
	write_set = all_fds;
	if (select(maxFd, &read_set, &write_set, NULL, NULL) < 0)
	{
		std::cout << "Error accured in select\n";
		exit(2);
	}
}

*/

