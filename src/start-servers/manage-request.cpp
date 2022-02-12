#include "manage-request.hpp"

std::fstream log_file;
#include <signal.h>
void	SIG_HANDL(int)
{
	log_file.close();
}


ManageRequest::ManageRequest(std::map<int, int> &fti) : aFdToIndex(fti)
{
	signal(SIGABRT, SIG_HANDL);
	log_file.open("./start-servers/test/log", std::fstream::out);
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
		ready_fds.resize(ready_fds.size() - epoll_struct_not_needed);
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
				++epoll_struct_not_needed;
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
	// i guess i will imporve here i will accept all connections at once maybe
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
	fdToRequest.insert(std::make_pair(tmpFd, Request(tmpFd, aFdToIndex[curFd])));
	// give extra eye on aFdToIndex[curFd]
}


int	ManageRequest::FDS_That_ready_for_IO(int &newConnections)
{
	ready_fds.reserve(ready_fds.capacity() + newConnections);
	newConnections = 0;
	return epoll_wait(epoll_fd, ready_fds.data(), ComFds.size(), -1); // -1 for block
}

