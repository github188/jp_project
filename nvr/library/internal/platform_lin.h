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
#ifndef __LIBRARY_INTERNAL_PLATFORMLIN_H__
#define __LIBRARY_INTERNAL_PLATFORMLIN_H__

#include "../../common/basetype.h"
#include "../common/threadhandle.h"
#include "platformdefs.h"

#include <sys/socket.h>
#include <vector>

namespace platform {

class PlatformAPI
{
	private:
		PlatformAPI();

		PlatformAPI(const PlatformAPI &);

		PlatformAPI& operator= (const PlatformAPI &);

	public:
		~PlatformAPI() {};

	public: //mutex processing methods
		/**
         * Creates a new Mutex instance at the location given by the mutex pointer
         * argument.  The mutex must be destroyed by calling the destroymutex
         * method when it is no longer needed.
         *
         * @param mutex
         *      Pointer to a memory location where the new Mutex is to be stored.
         */
		static void createmutex(m_mutex_t *mutes);

		static void lockmutex(m_mutex_t mutex);

		static bool trylockmutex(m_mutex_t mutex, struct timeval stTimeout);

		static bool trylockmutex(m_mutex_t mutex);

		static void unlockmutex(m_mutex_t mutex);

		static void destroymutex(m_mutex_t mutex);

	public: //Reader / Writer Mutex processing methods.
	
		static void createrwmutex(m_rwmutex_t *mutex);

		static void readerlockmutex(m_rwmutex_t mutex);

		static void writerlockmutex(m_rwmutex_t mutex);

		static bool tryreaderlockmutex(m_rwmutex_t mutex);

		static bool trywriterlockmutex(m_rwmutex_t mutex);

		static void unlockrwmutex(m_rwmutex_t mutex);

		static void destroyrwmutex(m_rwmutex_t mutex);

	public: // Condition processing methods
	
		static void createcondition(m_condition_t *cond);

		static void notify(m_condition_t cond);

		static void notifyall(m_condition_t cond);

		static void waitoncondition(m_condition_t cond, m_mutex_t mutex);

		static bool waitoncondition(m_condition_t cond, m_mutex_t mutex, int64 mills, int32 nanos);

		static void destroycondition(m_condition_t condition);

	public: //thread methods

		static void initprioritymapping(int32 maxPriority, std::vector<int32>& mapping);
		
		static void createnewthread(m_thread_t *handle, library::threadingTask tmain, void *threadarg, 
				int priority, int64 stacksize, uint64 *threadid);

		static void detachthread(m_thread_t hanlde);

		static void jointhread(m_thread_t hanlde);

		static void exitthread();

		static m_thread_t getcurrentthread();

		static int64 getcurrentthreadid();

		static int32 getPriority(m_thread_t thread);

		static void setPriority(m_thread_t thread, int32 priority);

		static int64 getstacksize(m_thread_t thread);

		static void setstacksize(m_thread_t thread, int64 stacksize);

		/**
         * Pause the current thread allowing another thread to be scheduled for
         * execution, no guarantee that this will happen.
         */
        static void yeild();

	public:  // Thread Local Methods
	
        static void createtlskey(m_tls_key* key);

        static void destroytlskey(m_tls_key key);

        static void* gettlsvalue(m_tls_key tlsKey);

        static void settlsvalue(m_tls_key tlskey, void* value);

	public: //socket

		static int32 createsocket(int32 &sockethadnle, int32 family, int32 type, int32 protocol);

		/**
		 * default handle:
		 * close the socket and return back imediately. data in the send buffer will be sended as much as possible.
		 * 
		 * note:
		 * struct linger {
		 *	int32 l_onoff;
		 *	int32 l_linger;
		 * }
		 * default handle may be changed when SO_LINGER is set with diffent situations.
		 * 1). l_onoff = 0: equal to default handle.
		 * 2). l_onoff = 1, l_linger = 0: closed the socket and kernel will discard the all data bufferd in the
		 *                                send buffer, and will send a RST to peer, this can avoid a TIME_WAIT state happen.
		 * 3). l_onoff = 1, l_linger != 0: 'close; will block until the data in the send buffer send out or timeout is happend.
		 *                                 in this situation, user must check the errorcode when close is returned. if
		 *                                 errorcode equals EWOULDBLOCK, then send buffer will be cleaned and data is not sended.
		 * Note:
		 * in 3) this means the user's data has been send to server and user receives the server's ACK, but
		 *        user doesn't known whether the server application has been read the data in its send buffer or not.
		 */
		static int32 closesocket(int32 socketfd);

		/**
		 * get the connected socket from the queue of 'listen'. timeout is the max time for client to connect.
		 */
		static int32 acceptsocket(int32 oldsockethandle, int32 &newsockethandle, struct sockaddr *cliaddr, socklen_t *addrlen, int32 timeout);

		/**
		 * user may bind ipaddress or port or both of them.
		 * normally, client need not bind any address and port, the kernel will choose it when connect returns success.
		 * for server, it always bind a well-known port for client connecting.
		 *
		 * but: if client binds a address, this means the client sends a packet with source ip address filled with it.
		 *      for tcp server, this means server only receive packets with the destination ipaddress equal to the binds
		 */
		static int32 bindsocket(int32 socketfd, const struct sockaddr *myaddr, socklen_t addrlen);

		/**
		 * error may be occured when:
		 * 1. client has not receive a syn ack from server due to timeout. errorcode: ETIMEDOUT
		 * 2. recive a RST ack, this means that there is no server listen on that port. errorcode: ECONNREFUSED
		 * 3. the SYN packet has been meat a destination unreachable. errorcode: EHOSTUNREACH or ENETUNREACH
		 *
		 * note: if connect is error, user must close the socket and reopen it.
		 */
		static int32 connectsocket(int32 sockethandle, const struct sockaddr *seraddr, socklen_t addrlen, int32 timeout);

		/**
		 * this function put the socekt created by 'createsocket' to passive state.
		 * @param backlog the sum of unconnected(three handshake not over) and connected sockets.
		 * 
		 * when the three handshake is over, a connected socket is added to the queue. if 'accept' is done, then
		 * it will remove the first element of the queue.
		 */
		static int32 listensocket(int32 socketfd, int32 backlog);

		/**
		 * close input. data in receive buffer will be discarded.
		 * later data sended to this socket will be discard silently discard without notifying to user.
		 *
		 * note: close inputstream doesn't mean the close is closed. user must call the 'close' to close it.
		 */
		static int32 closesocketinput(int32 socketfd);

		/**
		 * close the outputstream. data in the send buffer will send to peer.
		 *
		 * note: user must call the 'close' to close the socket descripiter.
		 */
		static int32 closesocketoutput(int32 socketfd);

		static int32 getsocketoption(int32 socket, int32 level, int32 optname, void *optval, socklen_t *optlen);

		static int32 setsocketoption(int32 socket, int32 level, int32 optname, void *optval, socklen_t optlen);

		static int32 read(int32 fd, char *buf, int32 len, int32 timeout);

		static int32 write(int32 fd, const char *buf, int32 len, int32 timeout);

		static int32 recvfrom(int32 fd, char *buf, int32 *len, int32 flags, struct sockaddr *from, int32 *fromlen, int32 timeout);

		static int32 sendto(int32 fd, const char *buf, int32 *len, int32 flags, const struct sockaddr *to, int32 tolen, int32 timeout);
};

}

#endif
