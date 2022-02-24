#include "response-wrapper.hpp"

ResponseWrapper::ResponseWrapper(Response *c_rsp) :
_com_response(c_rsp), _body(c_rsp->getBody())/*,
_buffer(c_rsp->getBuffer().c_str())*/
{
	resIsDone = false;
	lasTimeWereHere = std::time(NULL);
	hasBeenRead = 0;
}

ResponseWrapper::~ResponseWrapper()
{
	_com_response = NULL;
}


bool		ResponseWrapper::SendingResponse(int fd, char *storage_elment,  int required_size)
{
	lasTimeWereHere = std::time(NULL);
	if (!resIsDone)
	{
		resIsDone = _com_response->isReady();
		if (resIsDone) {
			_buffer = _com_response->getBuffer().c_str();
			_buffer_size = _com_response->getBuffer().size();
			bodySize = _com_response->getBodySize();
		}
		else
			return false;
	}
	g_client_closed = false;
	if (*_buffer)
	{
		// which means that header is not done sending yet
		if (SendingHeader(fd, required_size))
			return true;
	}
	// if you got here means that there's body that need to be sent
	if (!*_buffer){
		if (!bodySize)
			return true;
		return SendingBody(fd, storage_elment, required_size); // means response not done yet
	}
	return false;
}

/*
 fd is connected to a pipe or socket whose reading end is
closed.  When this happens the writing process will also
receive a SIGPIPE signal.  (Thus, the write return value
is seen only if the program catches, blocks or ignores
this signal )
 */

bool	ResponseWrapper::SendingBody(int fd, char *storage_elment, int required_size)
{
	// if body done sending returns true
	// if sigpipe happend which means that connection is closed by client do some modification and return true
	// if still sending return false;
	int read_data;
	if (hasBeenRead + required_size > bodySize)
		required_size = bodySize - hasBeenRead;
	_body.read(storage_elment, required_size);
	read_data = write(fd, storage_elment, required_size);
	if (g_client_closed)
	{
		_com_response->setKeepAlive(false);
		return true;
	}
	hasBeenRead += read_data;
	_body.seekg(hasBeenRead); // this very important problem could arise here
	return (hasBeenRead >= bodySize);
}


bool	ResponseWrapper::SendingHeader(int fd, int &required_size)
{
	// if true is returned means that there's no body and response id done
	int read_data;
	if ((size_t)required_size > _buffer_size)
	{
		read_data = write(fd, _buffer, _buffer_size);
		if (g_client_closed)
		{
			_com_response->setKeepAlive(false);
			return true; // which indacte that response is done combined setKeepAlive to false result will remove request and response related this connection
		}
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

bool		ResponseWrapper::CloseConnection() const
{
	return !_com_response->isKeepAlive();
}

bool	ResponseWrapper::isStillValid()
{
	return (std::time(NULL) - lasTimeWereHere < RS_TMOUT);
}

int		ResponseWrapper::getCommonSrvIndex() const
{
	return _com_response->getCommonServerIndex();
}


void		ResponseWrapper::Free_Com_response()
{
	_com_response->getBody().close();
	delete _com_response;
}


bool	ResponseWrapper::isFileUsed() const
{
	// should return false if body was store in file and not been used for upload porpuses
	// else return true which indactes that either the file was and used or was not at first place
	return _com_response->isFileUsed();
}
