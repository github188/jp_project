/***************************************************************************
 *   Copyright (C) 2016 by going
 *   Created on: 2016年4月24日
 *   Author: going                                           *
 *   going369@163.com                                                   *                                                                  *
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
#include "log.h"
#include <string>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
using namespace library;

AgentLog::AgentLog(LogLevel level, char * log_file_name) : _level(level)
{
	fl_stream =  fopen(log_file_name,"w");
	_lock = new Mutex;
}
AgentLog::AgentLog()
{

	fl_stream =  fopen("tem.log","w");
	_lock = new Mutex;
}

AgentLog::~AgentLog()
{
	delete _lock;
}


 void AgentLog::doLog(LogLevel l, int32 line, const char *file, const char *message)
{
	std::string log_string;
	std::string levevlstring;
	std::string entryname;
	std::string timestr;

	/* unlog the log record on l belows '_level' */
	if (_level > l) return;

	log_string = addLevelStr(l);

	/* add time */
	timestr = addtimeStr();
	log_string += timestr;


	/* add level indication */
	levevlstring = addLevelStr(l);
	log_string += levevlstring;

	/* add message */
	log_string += message;

	/* add 'file' and 'line' location */
	if (l >= LOG_WARN)
	{
		memset(file_line_buf,0,100);
		sprintf(file_line_buf,"#[%s,%d]#",file,line);

		log_string += file_line_buf;
	}

	/* tail a line */
	log_string += "\n";
	char * buf = new char[log_string.size()];
	strncpy(buf, log_string.c_str(), log_string.size());
	_lock->lock();
	fwrite(buf, sizeof(char), log_string.size(), fl_stream);
	fflush(fl_stream);
	_lock->unlock();
	delete [] buf;

}

/* get a log handler */


/* write log */


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

AgentLog *DefaultLog::agent = NULL;

////////////////////////////////////////////////////////////////////////////////
DefaultLog::~DefaultLog()
{
}

void DefaultLog::initialize()
{
	agent = new AgentLog();


	/* create stdout handler */
}

void DefaultLog::initialize(uint16 logLevel, char * log_file_name)
{
	agent = new AgentLog((LogLevel)logLevel, log_file_name);



	/* create stdout handler */


	/* create filestream handler */


}

void DefaultLog::log(LogLevel l, int32 line, const char *file, const char *format, ...)
{
	char logRecord[1024*2] = {0};
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
