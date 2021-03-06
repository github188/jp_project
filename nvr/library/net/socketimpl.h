/***************************************************************************
 *   Copyright (C) 2015 by root   				   						   *
 *   ysgen0217@163.com   							   					   *
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
#ifndef __LIBRARY_NET_SOCKETIMPL_H__
#define __LIBRARY_NET_SOCKETIMPL_H__

#include <string>

#include "../../common/basetype.h"

namespace library {

class InputStream;
class OutputStream;

typedef enum {

	/**
     * Disable Nagle's algorithm for this connection. Written data to the network is not
     * buffered pending acknowledgment of previously written data.  Valid for TCP sockets.
     */
	SOCKET_OPTION_TCP_NODELAY = 1,

	/**
     * Fetch the local address binding of a socket (this option cannot be "set" only "gotten",
     * since sockets are bound at creation time, and so the locally bound address cannot be
     * changed). The default local address of a socket is INADDR_ANY, meaning any local address
     * on a multi-homed host. A multi-homed host can use this option to accept connections to
     * only one of its addresses (in the case of a ServerSocket or DatagramSocket), or to
     * specify its return address to the peer (for a Socket or DatagramSocket). The parameter
     * of this option is an InetAddress.
     *
     * inherit of serversocket
     */
    SOCKET_OPTION_BINDADDR,

	/**
     * Sets SO_REUSEADDR for a socket. This is used only for MulticastSockets in decaf, and it is
     * set by default for MulticastSockets.
     */
	SOCKET_OPTION_REUSEADDR,

	/**
     * Sets SO_BROADCAST for a socket. This option enables and disables the ability of the process
     * to send broadcast messages. It is supported for only datagram sockets and only on networks
     * that support the concept of a broadcast message (e.g. Ethernet, token ring, etc.), and it
     * is set by default for DatagramSockets.
     */
	SOCKET_OPTION_BROADCAST,

	/**
     * Set which outgoing interface on which to send multicast packets. Useful on hosts with
     * multiple network interfaces, where applications want to use other than the system default.
     * Takes/returns an InetAddress.
     *
     * Valid for Multicast: DatagramSocketImpl.
     */
	SOCKET_OPTION_IP_MULTICAST_IF,

	/**
     * Same as above. This option is introduced so that the behaviour with IP_MULTICAST_IF will
     * be kept the same as before, while this new option can support setting outgoing interfaces
     * with either IPv4 and IPv6 addresses.
     */
	SOCKET_OPTION_IP_MULTICAST_IF2,

	/**
     * This option enables or disables local loopback of multicast datagrams. This option is enabled
     * by default for Multicast Sockets.
     */
	SOCKET_OPTION_IP_MULTICAST_LOOP,

	/**
     * This option sets the type-of-service or traffic class field in the IP header for a TCP or
     * UDP socket.
     */
	SOCKET_OPTION_IP_TOS,

	/**
     * Specify a linger-on-close timeout. This option disables/enables immediate return from a
     * close() of a TCP Socket. Enabling this option with a non-zero Integer timeout means that
     * a close() will block pending the transmission and acknowledgment of all data written to the
     * peer, at which point the socket is closed gracefully. Upon reaching the linger timeout, the
     * socket is closed forcefully, with a TCP RST. Enabling the option with a timeout of zero
     * does a forceful close immediately. If the specified timeout value exceeds 65,535 it will
     * be reduced to 65,535.
     *
     * Valid only for TCP: SocketImpl
     *
     * inherit of serversocket
     */
	SOCKET_OPTION_LINGER,

	/**
     * Set a timeout on blocking Socket operations.  The option must be set prior to entering a
     * blocking operation to take effect.
     */
	SOCKET_OPTION_TIMEOUT,

	/**
     * Set a hint the size of the underlying buffers used by the platform for outgoing network
     * I/O. When used in set, this is a suggestion to the kernel from the application about the
     * size of buffers to use for the data to be sent over the socket. When used in get, this
     * must return the size of the buffer actually used by the platform when sending out data
     * on this socket. Valid for all sockets: SocketImpl, DatagramSocketImpl
     *
     * inherit of serversocket
     */
	SOCKET_OPTION_SNDBUF,

	/**
     * Set a hint the size of the underlying buffers used by the platform for incoming network
     * I/O. When used in set, this is a suggestion to the kernel from the application about the
     * size of buffers to use for the data to be received over the socket. When used in get,
     * this must return the size of the buffer actually used by the platform when receiving in
     * data on this socket. Valid for all sockets: SocketImpl, DatagramSocketImpl.
     *
     * inherit of serversocket
     */
	SOCKET_OPTION_RCVBUF,

	/**
     * When the keepalive option is set for a TCP socket and no data has been exchanged across
     * the socket in either direction for 2 hours (NOTE: the actual value is implementation
     * dependent), TCP automatically sends a keepalive probe to the peer. This probe is a TCP
     * segment to which the peer must respond. One of three responses is expected: 1. The peer
     * responds with the expected ACK. The application is not notified (since everything is OK).
     * TCP will send another probe following another 2 hours of inactivity. 2. The peer responds
     * with an RST, which tells the local TCP that the peer host has crashed and rebooted. The
     * socket is closed. 3. There is no response from the peer. The socket is closed. The
     * purpose of this option is to detect if the peer host crashes.
     *
     * Valid only for TCP socket: SocketImpl
     */
	SOCKET_OPTION_KEEPALIVE,

	/**
     * When the OOBINLINE option is set, any TCP urgent data received on the socket will be
     * received through the socket input stream. When the option is disabled (which is the default)
     * urgent data is silently discarded.
     *
     * inherit of serversocket
     */
	SOCKET_OPTION_OOBINLINE,
     
} SOCKET_OPTION;

class SocketImpl
{
	private:

		SocketImpl(const SocketImpl&);

		SocketImpl& operator= (const SocketImpl&);
		
	public:

		SocketImpl();

		virtual ~SocketImpl();

	public:

		virtual int32 create() = 0;

		virtual int32 bind(const std::string &ipaddress, int32 port)= 0;

		virtual int32 listen(int32 backlog) = 0;

		virtual int32 accept(SocketImpl *socket) = 0;

		virtual int32 connect(const std::string& hostname, int port, int timeout) = 0;

		virtual InputStream *getInputStream() = 0;

		virtual OutputStream *getOutputStream() = 0;

		virtual int32 available() = 0;
		
		virtual void close() = 0;

		virtual void shutdownInput() = 0;

		virtual void shutdownOutput() = 0;

		virtual int32 getOption(int32 option) const = 0;

		virtual int32 setOption(int32 option, int32 value) = 0;

		virtual std::string getLocalAddress() = 0;

		virtual std::string getRemoteAddress() = 0;

		virtual uint16 getLocalPort() = 0;

		virtual uint16 getRemotePort() = 0;

		virtual int32 getSocketDescriptor() const = 0;

		virtual void clone(SocketImpl *src) {};

		virtual bool isclose() = 0;
};

}

#endif /* end of file */
