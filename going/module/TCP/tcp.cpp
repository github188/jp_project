/*
 * tcp_server.cpp
 *
 *  Created on: 2016��1��28��
 *      Author: going
 */
#include "tcp.h"
#include <iostream>
#include <string.h>
using std::cout;
using std::endl;
//**********************************server*******************************************//
TCP_SERVER::TCP_SERVER(uint16 port, cchar *addr_str)
{
	ser_port = port;
	::memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_port = htons(ser_port);
	local_addr.sin_family = AF_INET;
	::inet_aton(addr_str, &local_addr.sin_addr);
	fd = TCP.socket(AF_INET, SOCK_STREAM, 0);
	socklen_t len = sizeof(local_addr);
	TCP.bind(fd, (const struct sockaddr *)&local_addr,  len);
	TCP.listen(fd, SOMAXCONN);

}

int32 TCP_SERVER::accept()
{
	socklen_t len = sizeof(peer_addr);
	conn_fd = TCP.accept(fd, (sockaddr *) &peer_addr, &len);
	cout << inet_ntoa(peer_addr.sin_addr) << " connect!!! port::"  << peer_addr.sin_port << endl;
	return conn_fd;
}

ssize_t TCP_SERVER::recv(void *buf, uint32 len)
{
	ssize_t n;
	n = TCP.recv(conn_fd, buf, len, 0);
	cout << n << " byte recv!!!" << endl;
	return n;
}

ssize_t TCP_SERVER::send(void *buf, uint32 len)
{
	ssize_t n;
	n = TCP.send(conn_fd, buf, len, 0);
	cout << n << " byte send!!!" << endl;
	return n;
}


int32
TCP_SERVER::select(int32 nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	   struct timeval *timeout)
{
	int32		n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0){
		perror("select error");
	}
	return(n);		/* can return 0 on timeout */
}


TCP_SERVER::~TCP_SERVER()
{
	TCP.close(fd);
}



//**********************************client*******************************************//
TCP_CLIENT::TCP_CLIENT(uint16 port, cchar *addr_str)
{
	cli_port = port;
	::memset(&peer_addr, 0, sizeof(peer_addr));
	peer_addr.sin_port = htons(cli_port);
	peer_addr.sin_family = AF_INET;
	::inet_aton(addr_str, &peer_addr.sin_addr);
	fd = TCP.socket(AF_INET, SOCK_STREAM, 0);
	TCP.connect(fd, (struct sockaddr *) &peer_addr, sizeof(peer_addr));
}


ssize_t TCP_CLIENT::Recv(void *buf, uint32 len)
{
	ssize_t n;
	n = TCP.recv(fd, buf, len, 0);
	if(n > 0){
		cout << n << " byte recv!!!" << endl;
	}
	else{
		perror("fail to recv");
	}
	return n;
}

ssize_t TCP_CLIENT::Send(void *buf, uint32 len)
{
	ssize_t n;
	n = TCP.send(fd, buf, len, 0);
	cout << n << " byte send!!!" << endl;
	return n;
}
int32
TCP_CLIENT::Select(int32 nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	   struct timeval *timeout)
{
	int32		n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0){
		perror("select error");
	}
	return(n);		/* can return 0 on timeout */
}

TCP_CLIENT::~TCP_CLIENT()
{
	TCP.close(fd);
}

