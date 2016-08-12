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
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <sstream>

#include "logentry.h"
#include "logmanager.h"
#include "../io/filestreamhandler.h"
#include "../io/consolestreamhandler.h"

using namespace library;

LogEntry::LogEntry(std::string &n) : name(n)
{
}

LogEntry::LogEntry(const char *n) : name(n)
{
}

LogEntry::~LogEntry()
{
}

LogEntry& LogEntry::operator+= (const char *cstr)
{
	if (cstr == NULL) return *this;

	addString(cstr);

	return *this;
}

LogEntry& LogEntry::operator+= (std::string &str)
{
	if (str.size() == 0) return *this;

	addString(str.c_str());

	return *this;
}

LogEntry& LogEntry::operator+= (uint32 d)
{
	std::ostringstream oss;

	oss << d;
	
	addString(oss.str().c_str());

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
LogEntryImpl::LogEntryImpl(std::string &n) : LogEntry(n), _curPosPtr(NULL), value(NULL)
{
	_curPosPtr = value = new char[MAX_LOG_BLOCK_SIZE];
}

LogEntryImpl::LogEntryImpl(const char *n) : LogEntry(n), _curPosPtr(NULL), value(NULL)
{
	_curPosPtr = value = new char[MAX_LOG_BLOCK_SIZE];
}

LogEntryImpl::~LogEntryImpl()
{
	if (value) delete [] value;
	_curPosPtr = value = NULL;
}

void LogEntryImpl::addString(const char *str)
{
	std::ostringstream oss;

	if (str == NULL || ::strlen(str) <= 0)
	{
		oss << "[" << "NULL" << "]";
		::strcat(_curPosPtr, oss.str().c_str());
		_curPosPtr += oss.str().size();
		
		return;
	}

	size_t len = ::strlen(str);
	if (bytesLeft() >= len)
	{
		::strcat(_curPosPtr, str);
		_curPosPtr += len;
	}
	else
	{
		if (bytesLeft() >= 3)
		::strcat(_curPosPtr, "...");
		_curPosPtr += 3;
	}

	return;
}

void LogEntryImpl::addString(std::string &str)
{
	addString(str.c_str());
}

void LogEntryImpl::initialLogHeader()
{
	_curPosPtr = value;
	::memset(value, 0, MAX_LOG_BLOCK_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
AgentLog::AgentLog() : _level(LOG_DEBUG), _dst(LOG_STDOUT)
{
	handlers.clear();
}

AgentLog::AgentLog(LogLevel level, LogDest logwhere) : _level(level), _dst(logwhere)
{
	handlers.clear();
}

AgentLog::~AgentLog()
{
	std::map<LogDest, Handler*>::iterator iter;

	iter = handlers.begin();
	for (; iter != handlers.end(); ++iter)
	{
		Handler *handler = iter->second;
		if (handler) delete handler;
		handler = NULL;
	}
	
	handlers.clear();
}

void AgentLog::setHandler(LogDest dst, Handler *handler)
{
	std::map<LogDest, Handler*>::iterator iter;
	
	iter = handlers.find(dst);
	if (iter == handlers.end()) {
		handlers.insert(std::pair<LogDest, Handler*>(dst, handler));
	}
}

void AgentLog::doLog(LogLevel l, int32 line, const char *file, const char *message)
{
	std::string levevlstring;
	std::string entryname;
	std::string timestr;
	
	/* unlog the log record on l belows '_level' */
	if (_level > l) return;
	
	LogEntry *entry = startLog();
	entry->initialLogHeader();

	/* add time */
	timestr = addtimeStr();
	*entry += timestr;

	/* add thread name */
	entryname = ((LogEntryImpl*)entry)->getEntryName();
	*entry += "[";
	if (entryname.size() > 0)
		*entry += entryname;
	else
		*entry += _entrytag;
	*entry += "]";

	/* add level indication */
	levevlstring = addLevelStr(l);
	*entry += levevlstring;

	/* add message */
	*entry += message;

	/* add 'file' and 'line' location */
	if (l >= LOG_WARN)
	{
		*entry += " [";
		*entry += file;
		*entry += ", ";
		*entry += line;
		*entry += "]";
	}

	/* tail a line */
	*entry += "\n";
	
	endLog(entry);
}

/* get a log handler */
LogEntry* AgentLog::startLog()
{
	return LogManagerMgr::getInstance()->getLogEntry();
}

/* write log */
void AgentLog::endLog(LogEntry *entry)
{
	std::map<LogDest, Handler*>::iterator iter;

	iter = handlers.begin();
	for (; iter != handlers.end(); ++iter)
	{
		Handler *handler = iter->second;
		if (_dst == LOG_STDOUT || _dst == LOG_ALL) {
			if (handler) handler->publish(((LogEntryImpl*)entry)->getdata(), ((LogEntryImpl*)entry)->getLen());
		} else {
			if (iter->first >= _dst) {
				if (handler) handler->publish(((LogEntryImpl*)entry)->getdata(), ((LogEntryImpl*)entry)->getLen());
			}
		}
	}
}

std::string AgentLog::addLevelStr(LogLevel level)
{
	std::string color;
	
	switch (level)
	{
		case LOG_DEBUG:
			color += "[DEBUG]:";
			break;
		case LOG_INFO:
			color += "[INFOM]:";
			break;
		case LOG_WARN:
			color += "[WARNI]:";
			break;
		case LOG_ERROR:
			color += "[ERROR]:";
			break;
		default:
			color += "[DEBUG]:";
	}

	return color;
}

std::string AgentLog::addtimeStr()
{
	const char *week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	char timeStr[MAX_BUFFER_SIZE_064] = {0};
	struct timeval tv;
	std::string tm;
	char *ptr = NULL;

	gettimeofday(&tv, NULL);
	ptr = ctime(&tv.tv_sec);
	strncpy(timeStr, ptr, 19);
	snprintf(timeStr + 19, sizeof(timeStr) - 19, ".%06ld", tv.tv_usec);

#ifdef __WEEK_AND_TIME__
	struct tm *ptm;
	time_t now;

	::time(&now);
	ptm = ::localtime(&now);

	::snprintf(timeStr, sizeof(timeStr) - 1, "%s %02d:%02d:%02d", week[ptm->tm_wday],
			   ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
#endif

	tm += "[";
	tm += timeStr;
	tm += "]";

	return tm;
}

////////////////////////////////////////////////////////////////////////////////
AgentLog *DefaultLog::agent = NULL;
		
////////////////////////////////////////////////////////////////////////////////
DefaultLog::~DefaultLog()
{
}

void DefaultLog::initialize()
{
	agent = new AgentLog();

	agent->setDefaultEntryTag("MAINT");

	/* create stdout handler */
	agent->setHandler(LOG_STDOUT, new ConsoleStreamHandler());
}

void DefaultLog::initialize(uint16 logLevel, uint16 logWhere, std::string &logFile, uint32 fileSize)
{
	agent = new AgentLog((LogLevel)logLevel, (LogDest)logWhere);

	agent->setDefaultEntryTag("MAINT");

	/* create stdout handler */
	agent->setHandler(LOG_STDOUT, new ConsoleStreamHandler());

	/* create filestream handler */
	if (logFile.size() > 0) {
		agent->setHandler(LOG_FILE, new FileStreamHandler(logFile, fileSize));
	}
}

void DefaultLog::log(LogLevel l, int32 line, const char *file, const char *format, ...)
{
	char logRecord[MAX_BUFFER_SIZE_02k] = {0};
	va_list argPtr;

	if (agent == NULL) initialize();

	va_start(argPtr, format);

	snprintf(logRecord, sizeof(logRecord) - 1, "%s", format);
	vsprintf(logRecord, format, argPtr);

	va_end(argPtr);
	
	agent->doLog(l, line, file, logRecord);
}

void DefaultLog::shutdown()
{
	if (agent != NULL) delete agent;
	agent = NULL;
}