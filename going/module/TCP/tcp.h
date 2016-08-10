/*
 * tcp.h
 *
 *  Created on: 2016��1��28��
 *      Author: going
 */

#ifndef TCP_H_
#define TCP_H_
#include "socket.h"
class TCP_SERVER
{
public:
	TCP_SERVER(uint16 port, cchar *addr_str = "127.0.0.1");

	~TCP_SERVER();

	int32 accept();

	ssize_t recv(void *buf, uint32 len);

	ssize_t send(void *buf, uint32 len);

	int32
	select(int32 nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
		   struct timeval *timeout);

	int32
	get_fd(){ return fd;}

	int32
	get_conn_fd() {return conn_fd;}

private:
	SOCKET TCP;
	int32 fd;
	int32 conn_fd;
	uint16 ser_port;
	sockaddr_in local_addr;
	sockaddr_in peer_addr;
};
class TCP_CLIENT
{
public:
	TCP_CLIENT(uint16 port, cchar *addr_str );

	~TCP_CLIENT();

	ssize_t Recv(void *buf, uint32 len);

	ssize_t Send(void *buf, uint32 len);

	int32
	Select(int32 nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
		   struct timeval *timeout);
	int32
	Get_fd(){ return fd;}

private:
	SOCKET TCP;
	int32 fd;
	uint16 cli_port;
	sockaddr_in peer_addr;
};




#endif /* TCP_H_ */
