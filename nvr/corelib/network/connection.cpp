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
#include <sstream>

#include "../../library/buffer/circularbuffer.h"
#include "../../library/concurrent/reentrant.h"
#include "../../library/io/outputstream.h"
#include "../../library/io/inputstream.h"
#include "../../library/util/system.h"
#include "../../library/net/socket.h"
#include "../../library/util/log.h"
#include "connectionhandler.h"
#include "connection.h"
#include "iohandler.h"

using namespace library;
using namespace corelib;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;

#define TCP_OUTPUT_CAPACITY MAX_BUFFER_SIZE_01m

namespace corelib {
namespace MTP_NETWORK {

struct OUTPUT {
	OUTPUT() : _b(0), _t(0), _q(0) {
	};
	
	OUTPUT(const char *buf, size_t len) : _b(0), _t(len), _q(len) {
		memcpy(_buf, buf, len);
	};
	
	size_t Space() {
		return TCP_OUTPUT_CAPACITY - _t;
	};
	
	void Add(const char *buf, size_t len) {
		memcpy(_buf + _t, buf, len);
		_t += len;
		_q += len;
	};
	
	size_t Remove(size_t len){
		_b += len;
		_q -= len;
		return _q;
	};
	
	const char* Buf(){
		return _buf + _b;
	};
	
	size_t Len(){
		return _q;
	};

	size_t _b; //begin
	size_t _t; //tail
	size_t _q; 
	char _buf[TCP_OUTPUT_CAPACITY];
};
typedef std::list<OUTPUT *> output_l;

////////////////////////////////////////////////////////////////////////////////
class ConnectionInfo
{
	public:

		std::string connId; //session id

		InputStream *in;
		OutputStream *out;
		
		bool closed; // indicate the connection is closed.
		bool inputClosed; // indicate the connection input stream is closed
		bool outputClosed; // indicate the connection output stream is closed
		
		uint16 localPort;
		uint16 remotePort;
		std::string localIp;
		std::string remoteIp;

		bool bCloseAndDelete; //when input or output stream is closed, the connection should be delete
		bool bFlushoutBeforeClose; // flush out outbuffer before connection closed

		bool bDelByHandler; // indicate whether connection is deleted by plex handler or the userself
		time_t tCreate; // time when connection is created
		time_t tClose; // time when connection is closed(input or output)
		
		time_t idleStartTime; // update when read/write operation happens

		OUTPUT *obufTop; // output buffer on top
		output_l outbuffer; //outbuffer list which contains a set of data block
		size_t outputDataLength; // indicate how many data stored in the outbuffer
		
		CircularBuffer inbuffer;
		bool bInputBufferDisabled; // indicate whether inbuffer is diabled or not

		uint64 bytesSent; // indicate number of bytes have sended since connection create
		uint64 bytesReceived; // indicate number of bytes have received since connection create.
		
	public:

		ConnectionInfo(uint64 id, Socket *socket) : connId(), in(NULL), out(NULL), closed(false), inputClosed(false),
				outputClosed(false), localIp(), remoteIp(), bCloseAndDelete(false), bFlushoutBeforeClose(false),
				bDelByHandler(false), tCreate(time(NULL)), tClose(-1), idleStartTime(time(NULL)), obufTop(NULL), 
				outbuffer(), outputDataLength(0), inbuffer(MAX_BUFFER_SIZE_03m), bInputBufferDisabled(true),
				bytesSent(0), bytesReceived(0)
		{
			
			std::ostringstream oss;

			localPort = socket->getLocalPort();
			remotePort = socket->getRemotePort();
			remoteIp = socket->getRemoteAddress();
			localIp = socket->getLocalAddress();
			
			oss << id << "." << remoteIp << ":" << remotePort << "/" << localPort << "-" 
				<< System::currentTimeMillis();
			oss << "." << socket->getSocketDescriptor();
			this->connId = oss.str();
		};

		virtual ~ConnectionInfo() {
		};
};

}
}

////////////////////////////////////////////////////////////////////////////////
Connection::Connection(uint64 connId, library::Socket *socket, bool own) : IOEvent(CONNECTION),
				ownedSocket(own), mutex_(), iohandler_(NULL), socket_(socket), connInfo_(NULL), 
				connectionHandler_(NULL), seqNumber_(1)
{
	this->init(connId, NULL, socket);
}

Connection::Connection(uint64 connId, ConnectionHandler* handler, library::Socket *socket, bool own) : IOEvent(CONNECTION),
				ownedSocket(own), mutex_(), iohandler_(NULL), socket_(socket), connInfo_(NULL), connectionHandler_(handler),
				seqNumber_(1)
{
	this->init(connId, NULL, socket);
}

Connection::~Connection()
{
	Synchronization autolock(mutex_);
	
	this->close();
	finish();

	Log(LOG_WARN, "____ %s - %d ____", __FUNCTION__, __LINE__);
}

int32 Connection::bind(const std::string &localIpaddress, int16 localPort)
{
	return this->socket_->bind(localIpaddress, localPort);
}

int32 Connection::connect(const std::string &remoteIpaddress, int16 remotePort)
{
	return this->socket_->connect(remoteIpaddress, remotePort);
}

int32 Connection::connect(const std::string &remoteIpaddress, int16 remotePort, int32 timeout)
{
	return this->socket_->connect(remoteIpaddress, remotePort, timeout);
}

int32 Connection::reConnect(const std::string &remoteIpaddress, int16 remotePort)
{
	return this->socket_->reConnect(remoteIpaddress, remotePort);
}

int32 Connection::reConnect(const std::string &remoteIpaddress, int16 remotePort, int32 timeout)
{
	return this->socket_->reConnect(remoteIpaddress, remotePort, timeout);
}

bool Connection::send(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char slask[5000] = {0}; // vsprintf / vsnprintf temporary
	vsnprintf(slask, sizeof(slask), format, ap);
	va_end(ap);
	return send(std::string(slask), true);
}

bool Connection::send(const std::string &str, bool synwriting)
{
	return send(str.c_str(), str.size(), synwriting);
}

bool Connection::send(const char* buf, size_t len, bool synwriting)
{
	// lock it for may be plexer call 'handlewrite'
	Synchronization autolock(mutex_);
	
	if (this->isClosed() || this->isOutputClosed() || closeAndDelete())
		return false;

	// update idStarttime
	setCurrentTime();
	
	if (this->connInfo_->obufTop) {
		Buffer(buf, len);
		return true;
	}

	// write once or block until write done?
	int32 nwrite = this->writeData(buf, len, synwriting);
	if (nwrite < 0) {
		setCloseAndDelete(true);
		setFlushoutBeforeClose(false);
		return false;
	} else {
		if (nwrite < (int32)len) {
			Buffer(buf + nwrite, len - nwrite);
		}
	}

	if (connectionHandler_ != NULL) {
		bool bRead = !this->isInputClosed();
		if (this->connInfo_->outbuffer.size())
			connectionHandler_->IConnectionHandler_Mod(this, bRead, true);
		else
			connectionHandler_->IConnectionHandler_Mod(this, bRead, false);
	}

	return true;
}

int32 Connection::writeData(const char* data, int32 len, bool synwriting)
{
	// update idStarttime
	setCurrentTime();
	
	if (this->isClosed() || this->isOutputClosed()) {
		Log(LOG_INFO, "Connection::writeData - connection outputstream is closed.");
		return -1;
	}

	if (this->connInfo_ && this->connInfo_->out == NULL) {
		Log(LOG_INFO, "Connection::writeData - write kernel is closed.");
		return -1;
	}

	if (data == NULL || len <= 0) {
		Log(LOG_DEBUG, "Connection::writeData - check input parameter.");
		return 0;
	}

	this->connInfo_->out->lock();
	int32 nwrite = this->connInfo_->out->write((const uint8*)data, len, synwriting);
	if (nwrite < 0) {
		Log(LOG_INFO, "Connection::writeData - kernel is closed.");
	} else {
		this->connInfo_->bytesSent += nwrite;
	}
	this->connInfo_->out->flush();
	this->connInfo_->out->unlock();

	return nwrite;
}

void Connection::Buffer(const char* buf, size_t len)
{
	size_t ptr = 0;
	this->connInfo_->outputDataLength += len;

	while (ptr < len) {

		size_t space = 0;
		if ((space = this->connInfo_->obufTop ? this->connInfo_->obufTop->Space() : 0) > 0)
		{
			const char *pbuf = buf + ptr;
			size_t sz = len - ptr;
			if (space >= sz) {
				this->connInfo_->obufTop->Add(pbuf, sz);
				ptr += sz;
			} else {
				this->connInfo_->obufTop->Add(pbuf, space);
				ptr += space;
			}
		} else {
			this->connInfo_->obufTop = new OUTPUT;
			this->connInfo_->outbuffer.push_back(this->connInfo_->obufTop);
		}
	}
}

void Connection::handle(std::string& socketId, SOCKETOP op)
{
	if (this->uniqueIdentifier() != socketId) {
		Log(LOG_WARN, "Connection::handle(%s) - different connection handle.", socketId.c_str());
		return;
	}

	if (op & SO_READ) {
		this->handleRead();
	} else if (op & SO_WRITE) {
		this->handleWrite();
	} else if (op & SO_TIMED) {
		this->close(); // ????
	} else if (op & SO_CLOSE) {
		this->close();
	} else {
		;
	}
}

void Connection::handleRead()
{
	char tempbuffer[MAX_BUFFER_SIZE_50k] = {0};

	int32 nread = this->readData(tempbuffer, sizeof(tempbuffer));
	if (nread <= 0) {
		this->closeInput();
		setCloseAndDelete(true);
		if (this->iohandler_ != NULL) {
			this->iohandler_->OnConnectionDisConnect(this, (nread == 0 ? SHUT_RD: SHUT_ER));
		}
//		setFlushoutBeforeClose(false);
	} else {
		// user can read it from getInput if setted bufferinput flag
		if (!this->connInfo_->bInputBufferDisabled) {
			bool success = this->connInfo_->inbuffer.write(tempbuffer, nread);
			if (!success) {
				Log(LOG_WARN, "Connection::handleRead - input buffer overflow.");
			}

			//notify
			if (this->iohandler_ != NULL) {
				this->iohandler_->OnConnectionReceive(this);
			}
		} else {
			if (this->iohandler_ != NULL) {
				this->iohandler_->OnConnectionReceiveRawData(this, tempbuffer, nread);
			}
		}
	}
}

int32 Connection::readData(char *data, int32 len)
{
	// update idStarttime
	setCurrentTime();
	
	if (this->isClosed() || this->isInputClosed()) {
		Log(LOG_INFO, "Connection::readData - connection inputstream is closed.");
		return -1;
	}

	if (this->connInfo_ && this->connInfo_->in == NULL) {
		Log(LOG_INFO, "Connection::readData - read kernel is closed.");
		return -1;
	}

	if (data == NULL || len <= 0) {
		Log(LOG_DEBUG, "Connection::readData - check input parameter.");
		return 0;
	}

	this->connInfo_->in->lock();
	int32 nread = this->connInfo_->in->read((uint8*)data, len);
	this->connInfo_->in->unlock();
	if (nread < 0) {
		Log(LOG_INFO, "Connection::readData - kernel is closed.");
	} else {
		// count
		this->connInfo_->bytesReceived += nread;
	}

	return nread;
}

void Connection::handleWrite()
{
	// lock it for may be user call 'send'
	Synchronization autolock(mutex_);
	
	if (this->connInfo_->closed || this->connInfo_->outputClosed) return;

	bool repeat = false;
	do {

		if (this->connInfo_->outbuffer.empty()) {
			Log(LOG_INFO, "Connection::handleWrite - outbuffer empty.");
			break;
		}

		output_l::iterator it = this->connInfo_->outbuffer.begin();
		OUTPUT *p = *it;
		repeat = false;
		int32 nwrite = this->writeData(p->Buf(), p->Len(), false);
		if (nwrite < 0) {
			this->closeOutput();
			setCloseAndDelete(true);
			setFlushoutBeforeClose(false);
			if (this->iohandler_ != NULL) {
				this->iohandler_->OnConnectionDisConnect(this, SHUT_WR);
			}
			break;
		} else {
			size_t left = p->Remove(nwrite);
			this->connInfo_->outputDataLength -= nwrite;
			if (left == 0) {
				delete p;
				this->connInfo_->outbuffer.erase(it);
				if (!this->connInfo_->outbuffer.size()) {
					this->connInfo_->obufTop = NULL;
				} else {
					repeat = true;
				}
			}
		}
	} while (repeat);

	if (connectionHandler_ != NULL) {
		bool bRead = !this->isInputClosed();
		if (this->connInfo_->outbuffer.size())
			connectionHandler_->IConnectionHandler_Mod(this, bRead, true);
		else
			connectionHandler_->IConnectionHandler_Mod(this, bRead, false);
	}
}

size_t Connection::readInput(char *buf, size_t size)
{
	if (this->connInfo_->bInputBufferDisabled) return 0;

	size_t sz = size < getInputLength() ? size : getInputLength();
	this->connInfo_->inbuffer.read(buf, sz);

	return sz;
}

size_t Connection::peekInput(char *buf, size_t size)
{
	if (this->connInfo_->bInputBufferDisabled) return 0;

	size_t sz = size < getInputLength() ? size : getInputLength();
	this->connInfo_->inbuffer.peek(buf, sz);

	return sz;
}

size_t Connection::getInputLength()
{
	if (!this->connInfo_->bInputBufferDisabled)
		return this->connInfo_->inbuffer.getLength();
	else 
		return 0;
}

size_t Connection::getOutputLength()
{
	return this->connInfo_->outputDataLength;
}

uint64 Connection::getBytesReceived(bool clear)
{
	uint64 temp = this->connInfo_->bytesReceived;
	if (clear) {
		this->connInfo_->bytesReceived = 0;
	}

	return temp;
}

uint64 Connection::getBytesSended(bool clear)
{
	uint64 temp = this->connInfo_->bytesSent;
	if (clear) {
		this->connInfo_->bytesSent = 0;
	}

	return temp;
}

void Connection::shutdown()
{
	Synchronization autolock(mutex_);

//	if (connectionHandler_ != NULL)
//		connectionHandler_->IConnectionHandler_Mod(this, false, false);
//	closeOutput();
	this->connInfo_->bCloseAndDelete = true;
}

void Connection::close()
{
	if (this->connInfo_ == NULL) {
		Log(LOG_ERROR, "Connection::close - connection info is null.");
		return;
	}
	if (this->connInfo_->closed) return ;

	this->socket_->close();
	this->connInfo_->closed = true;
	this->connInfo_->inputClosed = true;
	this->connInfo_->outputClosed = true;
	this->connInfo_->tClose = time(NULL);

	Log(LOG_DEBUG, "Connection::close - connection alive: %llu seconds, send: %llu bytes, receive: %llu bytes.", 
		this->connInfo_->tClose - this->connInfo_->tCreate, this->connInfo_->bytesSent, this->connInfo_->bytesReceived);
}

void Connection::closeInput()
{
	if (this->connInfo_ == NULL) {
		Log(LOG_ERROR, "Connection::closeInput - connection info is null.");
		return;
	}

	if (this->connInfo_->closed || this->connInfo_->inputClosed) return ;
	
	if (this->connInfo_ && this->connInfo_->in == NULL) {
		this->connInfo_->inputClosed = true;
		return;
	}

	this->connInfo_->in->close();
	this->connInfo_->inputClosed = true;
}

void Connection::closeOutput()
{
	if (this->connInfo_ == NULL) {
		Log(LOG_ERROR, "Connection::closeOutput - connection info is null.");
		return;
	}

	if (this->connInfo_->closed || this->connInfo_->outputClosed) return ;

	if (this->connInfo_ && this->connInfo_->out == NULL) {
		this->connInfo_->outputClosed = true;
		return;
	}

	this->connInfo_->out->close();
	this->connInfo_->outputClosed = true;
}

bool Connection::isClosed()
{
	return this->connInfo_->closed;
}

bool Connection::isInputClosed()
{
	return this->connInfo_->inputClosed;
}

bool Connection::isOutputClosed()
{
	return this->connInfo_->outputClosed;
}

std::string Connection::uniqueIdentifier()const
{
	return this->connInfo_->connId;
}

uint16 Connection::getLocalPort()const
{
	return (uint16)(this->connInfo_->localPort);
}

uint16 Connection::getRemotePort()const
{
	return (uint16) (this->connInfo_->remotePort);
}

std::string Connection::getLocalIpAddress()const
{
	return this->connInfo_->localIp;
}

std::string Connection::getRemoteIpAddress()const
{
	return this->connInfo_->remoteIp;
}

int32 Connection::getSocketDescriptor()const {
	return this->socket_->getSocketDescriptor();
}

void Connection::setCloseAndDelete(bool value)
{
	this->connInfo_->bCloseAndDelete = value;
}

bool Connection::closeAndDelete()
{
	return this->connInfo_->bCloseAndDelete;
}

void Connection::setFlushoutBeforeClose(bool value)
{
	this->connInfo_->bFlushoutBeforeClose = value;
}

bool Connection::flushoutBeforeClose()
{
	return this->connInfo_->bFlushoutBeforeClose;
}

void Connection::setDeleteByHandler(bool value)
{
	this->connInfo_->bDelByHandler = value;
}

bool Connection::deleteByHandler()
{
	return this->connInfo_->bDelByHandler;
}

void Connection::setDisableInputBuffer(bool value)
{
	this->connInfo_->bInputBufferDisabled = value;
}

bool Connection::disableInputBuffer()
{
	return this->connInfo_->bInputBufferDisabled;
}

void Connection::setCurrentTime()
{
	this->connInfo_->idleStartTime = time(NULL);
}

bool Connection::isTimedOut(time_t now)
{
	// 15 seconds limit
	if (now - this->connInfo_->idleStartTime >= 15) {
		return true;
	}

	return false;
}

void Connection::setIohandler(IOHandler *handler)
{
	this->iohandler_ = handler;
}

void Connection::init(uint64 connId, Transport *transport, Socket *socket)
{
	if (socket == NULL) {
		Log(LOG_ERROR, "Connection::init - input is null.");
		return;
	}

	connInfo_ = new ConnectionInfo(connId, socket);
	this->connInfo_->in = socket->getInputStream();
	this->connInfo_->out = socket->getOutputStream();
}

void Connection::finish()
{
	// delete outbuffer
	output_l::iterator it = this->connInfo_->outbuffer.begin();
	for (; it != this->connInfo_->outbuffer.end(); ++it) {
		OUTPUT *p = *it;
		delete p;
	}
	this->connInfo_->outbuffer.clear();
	
	if (this->connInfo_) delete connInfo_;
	connInfo_ = NULL;

	if (ownedSocket) delete this->socket_;
	this->socket_ = NULL;
}
