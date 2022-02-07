#include "manage-request.hpp"

ManageRequest::ManageRequest(std::map<int, int> &fti) : aFdToIndex(fti)
{
	FD_ZERO(&all_fds);
	std::map<int, int>::const_iterator first(aFdToIndex.begin()), last(aFdToIndex.end());

	while (first != last)
	{
		FD_SET(first->first, &all_fds);
		++first;
	}
	--first;
	maxFd = first->first + 1;
}


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
