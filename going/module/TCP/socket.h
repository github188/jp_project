/*
 * socket.h
 *
 *  Created on: 2016��1��28��
 *      Author: going
 */

#ifndef SOCKET_H_G
#define SOCKET_H_G
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../../include/base_type.h"
#include <error.h>
#include <stdio.h>
class SOCKET{
public:
	SOCKET(){};
	virtual ~SOCKET(){};
	virtual int32
	accept(int32 fd, struct sockaddr *sa, socklen_t *salenptr);

	virtual void
	bind(int32 fd, const struct sockaddr *sa, socklen_t salen);

	virtual void
	connect(int32 fd, const struct sockaddr *sa, socklen_t salen);

	virtual void
	getpeername(int32 fd, struct sockaddr *sa, socklen_t *salenptr);

	virtual void
	getsockname(int32 fd, struct sockaddr *sa, socklen_t *salenptr);

	virtual void
	getsockopt(int32 fd, int32 level, int32 optname, void *optval, socklen_t *optlenptr);

	virtual void
	listen(int32 fd, int32 backlog);

	virtual ssize_t
	recv(int32 fd, void *ptr, size_t nbytes, int32 flags);

	virtual ssize_t
	recvfrom(int32 fd, void *ptr, size_t nbytes, int32 flags,
			 struct sockaddr *sa, socklen_t *salenptr);

	virtual ssize_t
	recvmsg(int32 fd, struct msghdr *msg, int32 flags);

	virtual int32
	select(int32 nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	       struct timeval *timeout);

	virtual ssize_t
	send(int32 fd, const void *ptr, size_t nbytes, int32 flags);

	virtual void
	sendto(int32 fd, const void *ptr, size_t nbytes, int32 flags,
		   const struct sockaddr *sa, socklen_t salen);

	virtual void
	sendmsg(int32 fd, const struct msghdr *msg, int32 flags);

	virtual void
	setsockopt(int32 fd, int32 level, int32 optname, const void *optval, socklen_t optlen);

	virtual void
	shutdown(int32 fd, int32 how);

	virtual int32
	sockatmark(int32 fd);

	virtual int32
	socket(int32 family, int32 type, int32 protocol);

	virtual void
	socketpair(int32 family, int32 type, int32 protocol, int32 *fd);

	virtual void
	close(int32 fd);




};




#endif /* SOCKET_H_ */
