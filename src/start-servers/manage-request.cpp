#include "manage-request.hpp"

std::fstream file_obj;
ManageRequest::ManageRequest(std::map<int, int> &fti) : aFdToIndex(fti)
{
	file_obj.open("./backup", std::fstream::out);
	int tmpMax;
	numOfFds = 0;
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
		++numOfFds;
		fd = first->first;
		tmpMax = first->first;
		event.data.fd = fd;
		event.events = EPOLLIN;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
		{
			std::cout << "could not this fd: " << first->first << "to epoll set\n";
			exit(1);
		}
		++first;
	}
	MSFDN = 1 + tmpMax;
	ready_fds.reserve(numOfFds * 2); // just i'm assuming that could be all servers be accepting at the same time new connections so i need twice spac
}

Response	*ManageRequest::ConstructRequest(std::map<int, Request>::iterator &iter_to_req)
{
	int	read_data;
	int	tot = 0;
	bzero(buffer, BUFFER_SIZE);
	do {
		read_data = read(iter_to_req->first, buffer + tot, read_nb);
		file_obj << buffer << "|\n";
		if (read_data <= 0) // and if client close fd it will removed by timeCheck function later so it does not matter here
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
		epoll_struct_not_needed = 0;
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
		// now i will remove ready_fds that are not needed
		timeCheck();
		cleanUnusedResponses();
		ready_fds.resize(ready_fds.capacity() - epoll_struct_not_needed);
	}
}

// time check function
void	ManageRequest::timeCheck()
{
	iter_to_req = fdToRequest.begin();
	tmp2_req_iter = fdToRequest.end(); // end iterator

	tmp1_req_iter = iter_to_req;

	while (iter_to_req != tmp2_req_iter)
	{
		++tmp1_req_iter; // hold next valid node in map
		if (!iter_to_req->second.isStillValid(std::time(NULL)))
		{
			++epoll_struct_not_needed;
			event.data.fd = iter_to_req->first;
			event.events = EPOLLIN;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, iter_to_req->first, &event))
			{
				std::cout << "could not delete " << iter_to_req->first << "\n";
				exit(1);
			}
			--numOfFds;
			close(iter_to_req->first);
			fdToRequest.erase(iter_to_req);
		}
		iter_to_req = tmp1_req_iter;
	}
}
// end of time check function
// clean responses related fuctions
void	ManageRequest::cleanUnusedResponses()
{
	// note maybe you will think if client send request then closed then by this logic i will hold fd for a littel while
	// so if he request again before i remove response he will get on the same index and cause problem bu he will never get
	// on the same index because i still hold related fd
	iter_to_res = fdToResponse.begin();
	tmp2_res_iter = fdToResponse.end();
	tmp1_res_iter = iter_to_res;
	event.events = EPOLLOUT;
	while (iter_to_res != tmp2_res_iter)
	{
		++tmp1_res_iter;
		if (!iter_to_res->second.isStillValid())
		{
			event.data.fd = iter_to_res->first;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, iter_to_res->first, &event))
			{
				std::cout << "could not delete: " << iter_to_res->first << "\n";
				exit(1);
			}
			--numOfFds;
			close(iter_to_res->first);
			iter_to_res->second.Free_Com_response();
			fdToResponse.erase(iter_to_res);
			fdToRequest.erase(iter_to_res->first);
		}
		iter_to_res = tmp1_res_iter;
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
		fdToResponse.insert(std::make_pair(curFd, ResponseWrapper(Restmp))); // this could leak if not deleted somewhere else just keep eye here
		event.data.ptr = NULL;
		event.data.fd = curFd;
		event.events = EPOLLOUT;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, curFd, &event) == -1)
		{
			std::cout << "could not change  to epoll set the fd: " << curFd << "\n";
			exit(1);
		}
		iter_to_req->second.DisableRequest(); // it's like it does not exist in fdRequest map
	}
}

void	ManageRequest::WorkOnResponse(int curFd)
{
	iter_to_res = fdToResponse.find(curFd);
	if (iter_to_res != fdToResponse.end() &&  iter_to_res->second.SendingResponse(curFd, buffer, read_nb))
	{
		iter_to_req = fdToRequest.find(curFd);
		// if you are in this branch means that response is done
		// it is time to to check keepalive so you can decide if you will close or not base on it
		// and remove response from map
		event.data.fd = curFd;
		event.events = EPOLLIN;
		// should i remove file associeted with body
		if (!iter_to_res->second.isFileUsed()){
			remove(iter_to_req->second.getFileName());
		}
		if (iter_to_res->second.CloseConnection())
		{
			++epoll_struct_not_needed;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, curFd, &event) == -1)
			{
				std::cout << "could not delete to epoll set the fd: " << curFd << "\n";
				exit(1);
			}
			--numOfFds;
			close(curFd);
			fdToRequest.erase(iter_to_req); // line k1
		}
		else
		{
			// look at line k1
			// if i am at this branch that means we do not need to remove request from map
			// so i will just rest the already insrted one
			iter_to_req->second.RESET();
			int srvFd = iter_to_res->second.getCommonSrvIndex();
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
	// i guess i will imporve here i will accept all connections at once maybe
	++newConnections;

	tmpFd = accept(curFd, (struct sockaddr *)(&client_addr), &address_len);
	std::cout << tmpFd << "acc fd \n";
	fcntl(tmpFd, F_SETFL, O_NONBLOCK);
	++numOfFds;
	event.data.fd = tmpFd;
	event.events = EPOLLIN;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tmpFd, &event) == -1)
	{
		std::cout << "could not add to epoll set the fd: " << tmpFd << " server fd: " << curFd << "\n";
		exit(1);
	}
	if (fdToResponse.count(tmpFd))
	{
		std::cout << "request already here\n";
		exit(1);
	}
	fdToRequest.insert(std::make_pair(tmpFd, Request(tmpFd, aFdToIndex[curFd], client_addr)));
	// give extra eye on aFdToIndex[curFd]
}


int	ManageRequest::FDS_That_ready_for_IO(int &newConnections)
{
	ready_fds.reserve(ready_fds.capacity() + newConnections);
	newConnections = 0;
	return epoll_wait(epoll_fd, ready_fds.data(), numOfFds, EPOLL_RETRY); // -1 for block
}
