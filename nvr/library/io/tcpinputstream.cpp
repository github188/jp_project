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
#include "../concurrent/reentrant.h"
#include "../net/tcpsocket.h"
#include "tcpinputstream.h"
#include "../util/log.h"

using namespace library;

TcpInputStream::TcpInputStream(Tcpsocket *tcpsocket) : socket(tcpsocket), closed(FALSE)
{
}

TcpInputStream::~TcpInputStream()
{
}

void TcpInputStream::close()
{
	if (closed) return;

	this->socket->shutdownInput();
	this->closed = TRUE;
}

bool TcpInputStream::avail()
{
	if (closed) return FALSE;
	
	int32 iRet = this->socket->available();
	if (iRet <= 0)
		return FALSE;
	else
		return TRUE;
}

int32 TcpInputStream::doReadByteArray(uint8 *buffer, int32 size, int32 offset, int32 length)
{
	int32 iRet = -1;

	if (closed) {

		Log(LOG_ERROR, "TcpInputStream is closed.");
		return -1;
	}
	
	if (buffer == NULL) {
		
		Log(LOG_ERROR, "buffer is null.");
		return 0;
	}

	if (size <= 0 || length <= 0 || offset < 0) {
		
		Log(LOG_ERROR, "input parameter is InValid: size: %d, length: %d", size, length);
		return 0;
	}

	if (length - offset > size){
		
		Log(LOG_ERROR, "read too much data: %d", length - offset);
		return 0;
	}

	iRet = this->socket->read(buffer, size, offset, length);
	if (iRet <= 0) {
		Log(LOG_DEBUG, "tcpinputstream is closed.");
	}

	return iRet;
}

bool TcpInputStream::isclosed()
{
	return this->closed;
}