/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016��1��31��
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   11.cpp                                                                      *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "socket.h"

int32
SOCKET::accept(int32 fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int32		n;

	if ( (n = accept(fd, sa, salenptr)) < 0) {
			perror("accept error");
			return -1;
	}
	return(n);
}

void
SOCKET::bind(int32 fd, const struct sockaddr *sa, socklen_t salen)
{
	agin:
	if (bind(fd, sa, salen) < 0){
		perror("bind error");
		::sleep(1);
		goto agin;
	}
}

void
SOCKET::connect(int32 fd, const struct sockaddr *sa, socklen_t salen)
{	agin:
	if (connect(fd, sa, salen) < 0){
		perror("connect error");
		goto agin;
	}
}

void
SOCKET::getpeername(int32 fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if (getpeername(fd, sa, salenptr) < 0)
		perror("getpeername error");
}

void
SOCKET::getsockname(int32 fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if (getsockname(fd, sa, salenptr) < 0)
		perror("getsockname error");
}

void
SOCKET::getsockopt(int32 fd, int32 level, int32 optname, void *optval, socklen_t *optlenptr)
{
	if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
		perror("getsockopt error");
}


/* include Listen */
void
SOCKET::listen(int32 fd, int32 backlog)
{
	char	*ptr;

		/*4can override 2nd argument with environment variable */
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);

	if (listen(fd, backlog) < 0)
		perror("listen error");
}
/* end Listen */


ssize_t
SOCKET::recv(int32 fd, void *ptr, size_t nbytes, int32 flags)
{
	ssize_t		n;

	if ( (n = recv(fd, ptr, nbytes, flags)) < 0)
		perror("recv error");
	return(n);
}

ssize_t
SOCKET::recvfrom(int32 fd, void *ptr, size_t nbytes, int32 flags,
		 struct sockaddr *sa, socklen_t *salenptr)
{
	ssize_t		n;

	if ( (n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
		perror("recvfrom error");
	return(n);
}

ssize_t
SOCKET::recvmsg(int32 fd, struct msghdr *msg, int32 flags)
{
	ssize_t		n;

	if ( (n = recvmsg(fd, msg, flags)) < 0)
		perror("recvmsg error");
	return(n);
}

int32
SOCKET::select(int32 nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
	int32		n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		perror("select error");
	return(n);		/* can return 0 on timeout */
}

ssize_t
SOCKET::send(int32 fd, const void *ptr, size_t nbytes, int32 flags)
{
	if (send(fd, ptr, nbytes, flags) != (ssize_t)nbytes){
		perror("send error");
		return -1;
	}
	else{
		return nbytes;
	}
}

void
SOCKET::sendto(int32 fd, const void *ptr, size_t nbytes, int32 flags,
	   const struct sockaddr *sa, socklen_t salen)
{
	if (sendto(fd, ptr, nbytes, flags, sa, salen) != (ssize_t)nbytes)
		perror("sendto error");
}

void
SOCKET::sendmsg(int32 fd, const struct msghdr *msg, int32 flags)
{
	uint32	i;
	ssize_t			nbytes;

	nbytes = 0;	/* must first figure out what return value should be */
	for (i = 0; i < msg->msg_iovlen; i++)
		nbytes += msg->msg_iov[i].iov_len;

	if (sendmsg(fd, msg, flags) != nbytes)
		perror("sendmsg error");
}

void
SOCKET::setsockopt(int32 fd, int32 level, int32 optname, const void *optval, socklen_t optlen)
{
	if (setsockopt(fd, level, optname, optval, optlen) < 0)
		perror("setsockopt error");
}

void
SOCKET::shutdown(int32 fd, int32 how)
{
	if (shutdown(fd, how) < 0)
		perror("shutdown error");
}

int32
SOCKET::sockatmark(int32 fd)
{
	int32		n;

	if ( (n = sockatmark(fd)) < 0)
		perror("sockatmark error");
	return(n);
}

/* include Socket */
int32
SOCKET::socket(int32 family, int32 type, int32 protocol)
{
	int32		n;

	if ( (n = socket(family, type, protocol)) < 0)
		perror("socket error");
	return(n);
}
/* end Socket */

void
SOCKET::socketpair(int32 family, int32 type, int32 protocol, int32 *fd)
{
	int32		n;

	if ( (n = socketpair(family, type, protocol, fd)) < 0)
		perror("socketpair error");
}
void
SOCKET::close(int32 fd)
{
	int32		n;
	if( (n = close(fd)) < 0)
		perror("close error");
}



