#include "inetSocket.hpp"


ssize_t	readn(int fd, void *buffer, size_t n)
{
	ssize_t numRead;					/* # of bytes fetched by last read() */
	size_t totRead;						/* Total # of bytes read so far */
	char *buf;

	buf = (char *)buffer;						/* No pointer arithmetic on "void *" */
	for (totRead = 0; totRead < n; )
	{
		numRead = recv(fd, buf, n - totRead, 0);
		if (numRead == 0)				/* EOF */
			return totRead;				/* May be 0 if this is first read() */
		if (numRead == -1)
			return -1;				/* Some other error */
		totRead += numRead;
		buf += numRead;
		printf("read num = %lu\n", totRead);
	}
	return totRead;						/* Must be 'n' bytes if we get here */
}

ssize_t	writen(int fd, const void *buffer, size_t n)
{
	ssize_t numWritten;					/* # of bytes written by last write() */
	size_t totWritten; 					/* Total # of bytes written so far */
	const char *buf;
	buf = (char *)buffer;				/* No pointer arithmetic on "void *" */
	for (totWritten = 0; totWritten < n; )
	{
		numWritten = send(fd, buf, n - totWritten, MSG_DONTWAIT);
		if (numWritten <= 0)
				return -1;				/* Some other error */
		totWritten += numWritten;
		buf += numWritten;
	}
	return totWritten;					/* Must be 'n' bytes if we get here */
}
