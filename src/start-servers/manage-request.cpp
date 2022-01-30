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
	int tmpfd;
	std::map<int, Request>::iterator iter_to_req;
	while (true)
	{
		Select();
		for (int i = 3; i < maxFd; ++i) // start i = 3 becaus 0 1 2 is defalut for read write error
		{
			if (FD_ISSET(i, &read_set))
			{
				if (aFdToIndex.count(i)) // this means that this fd is one of servers sockets
				{
					// i nead to accept connection
					tmpfd = accept(i, NULL, NULL);
					if (tmpfd + 1 > maxFd)
						maxFd = tmpfd + 1;
					fcntl(tmpfd, F_SETFL, O_NONBLOCK);
					FD_SET(tmpfd, &all_fds);
					fdToRequest.insert(std::make_pair(tmpfd, Request(i)));
					// the above line is saying in genral that this is tmpfd has relation with the request and in requset i store with sever should handel aka (i)
				}
				else
				{
					// means that client has sent some data
					// find the requset at i and call on  it  AddToRequest
					iter_to_req = fdToRequest.find(i);
					if (ConstructRequest(iter_to_req))
					{
						// if we got inside means that request is done
						// start responding based on on the completed request
					}
				}
			}
		}
	}
}

bool	ManageRequest::ConstructRequest(std::map<int, Request>::iterator &iter_to_req)
{
	int	read_data;
	int	status;
	int	tot = 0;
	do {
		read_data = read(iter_to_req->first, buffer + tot, read_nb);
		if (read_data <= 0)
			break ;
		tot += read_data;
	}while (tot < read_nb);
	buffer[tot] = 0;
	status = iter_to_req->second.AddToRequest(buffer, tot);
	// status will determin if request is done or error or not done yet
	return false;
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
