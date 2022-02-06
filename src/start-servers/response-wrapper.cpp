#include "response-wrapper.hpp"

ResponseWrapper::ResponseWrapper(Response *c_rsp) :
_com_response(c_rsp), _body(c_rsp->getBody()),
_buffer(c_rsp->getBuffer().c_str())
{
	startTime = std::time(NULL);
	bodySize = c_rsp->getBodySize();
	hasBeenRead = 0;
	_buffer_size = c_rsp->getBuffer().size();
}


bool		ResponseWrapper::SendingResponse(int fd, char *storage_elment,  int required_size)
{
	if (*_buffer)
	{
		// which means that header is not done sending yet
		if (SendingHeader(fd, required_size))
			return true;
	}
	// if you got here means that there's body that need to be sent
	return SendingBody(fd, storage_elment, required_size); // means response not done yet
}

bool	ResponseWrapper::SendingBody(int fd, char *storage_elment, int required_size)
{
	int read_data;
	if (hasBeenRead + required_size > bodySize)
		required_size = bodySize - hasBeenRead;
	_body.read(storage_elment, required_size);
	read_data = write(fd, storage_elment, required_size);
	hasBeenRead += read_data;
	_body.seekg(hasBeenRead); // this very important problem could arise here
	return (hasBeenRead == bodySize);
}


bool	ResponseWrapper::SendingHeader(int fd, int &required_size)
{
	// if true is returned means that there's no body and response id done
	int read_data;
	if (required_size > _buffer_size)
	{
		read_data = write(fd, _buffer, _buffer_size);
		_buffer += read_data; // if header was all sent _buffer will be "" so we will get in this branch again
		required_size -= read_data; // new size that we will try to send from body in case of heaader is done
	}
	if (*_buffer == 0)
	{
		if (!_body.is_open())
			return true; // indactes that we are done with the response
	}
	return false;
}

bool		ResponseWrapper::isStillValid(std::time_t cur_t)
{
	return (cur_t - startTime < RQ_RS_TIME_OUT);
}

bool		ResponseWrapper::CloseConnection() const
{
	return !_com_response->isKeepAlive();
}
