/***************************************************************************
 *   Copyright (C) 2015 by Y.S.G   										   *
 *   ysgen0217@163.com   												   *
 *                                                                         *
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


#include "platform_lin.h"

#include <stdio.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>

using namespace platform;
using namespace library;

////////////////////////////////////////////////////////////////////////////////
#if 1
int32 wait_for_io_or_timeout(int32 fd, uint64 timeout, int for_read)
{
    struct pollfd pfd;
    int rc;

    pfd.fd = fd;
    pfd.events = for_read ? POLLIN : POLLOUT;

    do {
        rc = poll(&pfd, 1, timeout/1000000);
    } while (rc == -1 && errno == EINTR);
	
    if (rc < 0) {
		//error
        return -1;
    } else if (rc == 0) {
    	//timeout
    	return 1;
	} else {
        return 0;
    }
}
#else
int32 wait_for_io_or_timeout(int32 fd, uint64 timeout, int for_read)
{
    usleep(5 * 100);
	return 1;
}
#endif

////////////////////////////////////////////////////////////////////////////////
void PlatformAPI::createmutex(m_mutex_t *mutex)
{
	*mutex = new pthread_mutex_t;

    if (pthread_mutex_init(*mutex, NULL) != 0) {
        perror("createmutex error");
    }
}

void PlatformAPI::lockmutex(m_mutex_t mutex)
{
	if (pthread_mutex_lock(mutex) != 0) {
			perror("lockmutex error");
    }
}

bool PlatformAPI::trylockmutex(m_mutex_t mutex, struct timeval stTimeout)
{
	int32 iRet = pthread_mutex_trylock(mutex);
	if (iRet == -1) {
		switch (iRet)
		{
			case EBUSY:
			{
				while (select(0, 0, 0, 0, &stTimeout) < 0 && errno == EINTR);
				iRet = pthread_mutex_trylock(mutex);
				
				break;
			}
			default:
				break;
		}
	}

	return iRet == 0 ? true : false;
}

bool PlatformAPI::trylockmutex(m_mutex_t mutex)
{
	return pthread_mutex_trylock(mutex) == 0 ? true : false;
}

void PlatformAPI::unlockmutex(m_mutex_t mutex)
{
	if (pthread_mutex_unlock(mutex) != 0) {
			perror("unlockmutex error");
    }
}

void PlatformAPI::destroymutex(m_mutex_t mutex)
{
	pthread_mutex_destroy(mutex);
    delete mutex;
}

////////////////////////////////////////////////////////////////////////////////
void PlatformAPI::createrwmutex(m_rwmutex_t *mutex)
{
	*mutex = new pthread_rwlock_t;

    if( pthread_rwlock_init(*mutex, NULL) != 0 ) {
    	perror("createrwmutex error");
    }
}

void PlatformAPI::readerlockmutex(m_rwmutex_t mutex)
{
	if (pthread_rwlock_rdlock(mutex) != 0) {
		perror("readerlockmutex error");
    }
}

void PlatformAPI::writerlockmutex(m_rwmutex_t mutex)
{
	if (pthread_rwlock_wrlock(mutex) != 0) {
		perror("writerlockmutex error");
    }
}

bool PlatformAPI::tryreaderlockmutex(m_rwmutex_t mutex)
{
	return pthread_rwlock_tryrdlock(mutex) == 0 ? true : false;
}

bool PlatformAPI::trywriterlockmutex(m_rwmutex_t mutex)
{
	return pthread_rwlock_trywrlock(mutex) == 0 ? true : false;
}

void PlatformAPI::unlockrwmutex(m_rwmutex_t mutex)
{
	if (pthread_rwlock_unlock(mutex) != 0) {
		perror("unlockrwmutex error");
    }
}

void PlatformAPI::destroyrwmutex(m_rwmutex_t mutex)
{
	pthread_rwlock_destroy(mutex);
    delete mutex;
}


////////////////////////////////////////////////////////////////////////////////

int32 PlatformAPI::createsocket(int32 &sockethandle, int32 family, int32 type, int32 protocol)
{
	sockethandle = socket(family, type, protocol);
	if (sockethandle == -1) return STATUS_ERROR;

	return STATUS_SUCCESS;
}

int32 PlatformAPI::closesocket(int32 sockethandle)
{
	return close(sockethandle);
}

int32 PlatformAPI::bindsocket(int32 sockethandle, const struct sockaddr *myaddr, socklen_t addrlen)
{
	return bind(sockethandle, myaddr, addrlen);
}

int32 PlatformAPI::acceptsocket(int32 oldsockethandle, int32 &newsockethandle, struct sockaddr *cliaddr, socklen_t *addrlen, int32 timeout)
{
	int32 rc = 0;

	do {
		
		rc = accept(oldsockethandle, cliaddr, addrlen);
		if ((rc == -1 && errno == EWOULDBLOCK) && timeout > 0) {

			struct timeval tv;
			fd_set rdfs;

			tv.tv_sec = timeout/1000;
			tv.tv_usec = (timeout - tv.tv_sec * 1000) * 1000;

			FD_SET(oldsockethandle, &rdfs);
			int32 ready = select(oldsockethandle + 1, &rdfs, NULL, NULL, &tv);
			if (ready > 0) {
				// some thing to read
				continue;
			} else if (ready == 0) {
				// time out
				break;
			} else {
				// error happens
				if (errno == EINTR || errno == EWOULDBLOCK)
					continue;
				break;
			}
		}
	} while (rc == -1 && errno == EINTR);

	if (rc > 0) {
		
		newsockethandle = rc;
		return 0;
	} else {
		//Log(LOG_DEBUG, "errStr: %s", strerror(errno));
	}

	return -1;
}

//default is non-block mode
int32 PlatformAPI::connectsocket(int32 sockethandle, const struct sockaddr *seraddr, socklen_t addrlen, int32 timeout)
{
	int32 rc = -1;

	/* when connect is interupted by a signal, it put the socket to connect again, bu it will be 
	 * return immediately because a error: EADDRINUSE will happen.
	 */
	do {

		rc = connect(sockethandle, seraddr, addrlen);
	} while (rc == -1 && errno == EINTR);

	/* we can see EINPROGRESS the first time connect is called on a non-blocking
     * socket; if called again, we can see EALREADY
     */
	if ((rc == -1) && (errno == EINPROGRESS || errno == EALREADY) && timeout > 0) {

		/*
		 * check whether this socket is readable. if it is readable, then may two situation happens:
		 * 1. user data is coming.
		 * 2. error has happens.
		 *
		 * NOTE: here use "write fd" to check the socekt status.
		 * in this two situations, we use getsockopt to decide whether there is a error or not.
		 */
		rc = wait_for_io_or_timeout(sockethandle, timeout * 1000, 0);
		if (rc != 0) {

			return -1;
		}

		//only getsockopt return 0 and error is also 0 proves the client connect to server success.
		int32 error;
		socklen_t len = sizeof(error);
		if ((rc = getsockopt(sockethandle, SOL_SOCKET, SO_ERROR, (char *) &error, &len)) < 0) {
			//for solaris: getsockopt return -1, error stores the error code
			return -1;
		}

		//for Berkeley: getsockopt return zero, error stores the error code;
		if (error != 0) {
			return -1;
		}
	}

	if (rc == -1 && errno != EISCONN) {
		//Log(LOG_ERROR, "connect error1: %s", strerror(errno));
		return -1;
	}

	return 0;
}

int32 PlatformAPI::listensocket(int32 sockethandle, int32 backlog)
{
	return listen(sockethandle, backlog);
}

int32 PlatformAPI::closesocketinput(int32 sockethandle)
{
	return shutdown(sockethandle, SHUT_RD);
}

int32 PlatformAPI::closesocketoutput(int32 sockethandle)
{
	return shutdown(sockethandle, SHUT_WR);
}

int32 PlatformAPI::getsocketoption(int32 sockethandle, int32 level, int32 optname, void *optval, socklen_t *optlen)
{
	return getsockopt(sockethandle, level, optname, optval, optlen);
}

int32 PlatformAPI::setsocketoption(int32 sockethandle, int32 level, int32 optname, void *optval, socklen_t optlen)
{
	return setsockopt(sockethandle, level, optname, optval, optlen);
}

//read one times when read something.
int32 PlatformAPI::read(int32 fd, char *buf, int32 len, int32 timeout)
{
	int32 nread = 0;
	
	do {
		nread = ::recv(fd, buf, len, 0);
	} while (nread == -1 && errno == EINTR);

	while ((nread == -1) && (errno == EAGAIN || errno == EWOULDBLOCK) && (timeout > 0)) {

		/* is buffer readable ? */
		int arv = wait_for_io_or_timeout(fd, timeout * 1000, 1);
		if (arv != STATUS_SUCCESS) {
			
			if (arv < 0) {
				return -1;
			} else {
				return 0;
			}
		} else {

			do {
				nread = ::recv(fd, buf, len, 0);
			} while (nread == -1 && errno == EINTR);
		}
	}

	return nread;
}

//only write to peer once on success. user wants to write a lot of bytes should calculate the remaining bytes and send again.
int32 PlatformAPI::write(int32 fd, const char *buf, int32 len, int32 timeout)
{
	int32 nwrite = 0;
	
	do {
		nwrite = ::send(fd, buf, len, 0);
	} while (nwrite == -1 && errno == EINTR);

	while ((nwrite == -1) && (errno == EAGAIN || errno == EWOULDBLOCK) && (timeout > 0)) {

		/* is buffer writeable ? */
		int arv = wait_for_io_or_timeout(fd, timeout * 1000, 0);
		if (arv != STATUS_SUCCESS) {
			
			//if error happens or send all data time out, this means socket is closed.
			//we should close the outputstream.
			if (arv < 0) {
				return -1;
			} else {
				return 0;
			}
		} else {
		
			do {
				nwrite = ::send(fd, buf, len, 0);
			} while (nwrite == -1 && errno == EINTR);
		}
	}

	return nwrite;
}

/* used for UDP */
int32 PlatformAPI::recvfrom(int32 fd, char *buf, int32 *len, int32 flags, struct sockaddr *from, int32 *fromlen, int32 timeout)
{
	int32 nread = 0;

	do {

		nread = ::recvfrom(fd, buf, *len, flags, from, (socklen_t *)fromlen);
	} while (nread == -1 && errno == EINTR);

	while ((nread == -1) && (errno == EAGAIN || errno == EWOULDBLOCK) && (timeout > 0)) {

		/* is buffer readable ? */
		int arv = wait_for_io_or_timeout(fd, timeout * 1000, 1);
		if (arv != STATUS_SUCCESS) {
			
			*len = 0;
			if (arv < 0)
				return -1;
			else
				return 0;
		} else {
		
			do {
				
				nread = ::recvfrom(fd, buf, *len, flags, from, (socklen_t *)fromlen);
			} while (nread == -1 && errno == EINTR);
		}
	}

	/* some error has happened on this socket */
	if (nread == -1) {
		
		*len = 0;
		return -1;
	}

	*len = nread;
	return 0;
}

/* used for UDP */
int32 PlatformAPI::sendto(int32 fd, const char *buf, int32 *len, int32 flags, const struct sockaddr *to, int32 tolen, int32 timeout)
{
	int32 nwrite = 0;

	do {
		
		nwrite = ::sendto(fd, buf, (*len), flags, to, tolen);
	} while (nwrite == -1 && errno == EINTR);

	while ((nwrite == -1) && (errno == EAGAIN || errno == EWOULDBLOCK) && (timeout > 0)) {

		/* is buffer writeable ? */
		int arv = wait_for_io_or_timeout(fd, timeout * 1000, 0);
		if (arv != STATUS_SUCCESS) {
			
			//if error happens or send all data time out, this means socket is closed.
			//we should close the outputstream.
			*len = 0;
			if (arv < 0)
				return -1;
			else
				return 0;
		} else {
		
			do {
				
				nwrite = ::sendto(fd, buf, (*len), flags, to, tolen);
			} while (nwrite == -1 && errno == EINTR);
		}
	}

	if (nwrite == -1) {
        *len = 0;
        return -1;
    }
    *len = nwrite;
    return 0;
}
