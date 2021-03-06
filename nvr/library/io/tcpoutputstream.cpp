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
#include "tcpoutputstream.h"
#include "../net/tcpsocket.h"
#include "../util/log.h"

using namespace library;

TcpOutputStream::TcpOutputStream(Tcpsocket *tcpsocket) : socket(tcpsocket), closed(FALSE)
{
}

TcpOutputStream::~TcpOutputStream()
{
}

void TcpOutputStream::flush()
{
}

void TcpOutputStream::close()
{
	if (closed) return;

	this->socket->shutdownOutput();
	this->closed = TRUE;
}

int32 TcpOutputStream::doWriteByteArray(const uint8 *buffer, int32 size, int32 offset, int32 length, bool bSyncWriting)
{
	int32 iRet = -1;

	if (closed) {

		Log(LOG_ERROR, "TcpOutputStream is closed.");
		return -1;
	}
	
	if (buffer == NULL) {
		
		Log(LOG_ERROR, "TcpOutputStream buffer is null.");
		return 0;
	}

	if (size <= 0 || length <= 0 || offset < 0) {
		
		Log(LOG_ERROR, "TcpOutputStream input parameter is InValid: size: %d, length: %d", size, length);
		return 0;
	}

	if (length - offset > size){
		
		Log(LOG_ERROR, "TcpOutputStream write too much data: %d", length - offset);
		return 0;
	}

	if (bSyncWriting)
		iRet = this->socket->write(buffer, size, offset, length);
	else
		iRet = this->socket->write1(buffer, size, offset, length);
	
//	if (iRet < length) {
//		Log(LOG_DEBUG, "tcpoutputstream is closed.");
//	}

	return iRet;
}

bool TcpOutputStream::isclosed()
{
	return this->closed;
}