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
#ifndef __LIBRARY_UTIL_LOGENTRY_H__
#define __LIBRARY_UTIL_LOGENTRY_H__

#include <map>
#include <string>
#include <vector>

#include "../../common/basetype.h"
#include "../../common/paradef.h"
#include "../common/handler.h"

namespace library {

typedef enum {
		
	LOG_DEBUG = 0,
		
	LOG_INFO = 1,
	
	LOG_WARN = 2,
	
	LOG_ERROR = 3
	
} LogLevel;

typedef enum {

	LOG_STDOUT = 0,
		
	LOG_FILE = 1,

	LOG_ALL = 2
	
} LogDest;

////////////////////////////////////////////////////////////////////////////////
class LogEntry
{
	public:

		LogEntry(std::string &n);

		LogEntry(const char *n);

		virtual ~LogEntry();

	public:
		
		LogEntry& operator+= (const char *cstr);

		LogEntry& operator+= (std::string &str);

		LogEntry& operator+= (uint32);

		virtual void initialLogHeader() = 0;
		
	protected:

		virtual void addString(const char *str) = 0;

		virtual void addString(std::string &str) = 0;
		
	protected:

		std::string name;
};

////////////////////////////////////////////////////////////////////////////////
class LogEntryImpl : public LogEntry
{

	enum 
	{
		MAX_LOG_BLOCK_SIZE = 2560
	};
	
	public:

		LogEntryImpl(std::string &n);

		LogEntryImpl(const char *n);

		~LogEntryImpl();

	public:
		
		const char *getdata()const {return value;};

		uint64 getLen()const {return (_curPosPtr - value);};
		
		std::string getEntryName() const {return name;};

		virtual void initialLogHeader();

	protected:

		virtual void addString(const char *str);

		virtual void addString(std::string &str);

		uint64 bytesLeft() const {
			return (uint64)(MAX_LOG_BLOCK_SIZE - (_curPosPtr - value) - 1);
		};

		std::string addtimeStr();

	private:
		char *_curPosPtr;
		char *value;
};

////////////////////////////////////////////////////////////////////////////////
class AgentLog
{
	public:

		AgentLog();
		
		AgentLog(LogLevel level, LogDest logwhere);

		~AgentLog();

	public:

		/**
		 * set write handler for log
		 *
		 * @param handler handler to set.
		 */
		void setHandler(LogDest dst, Handler *handler);

		/**
		 * set tag to the logentry handle without giving a name
		 */
		void setDefaultEntryTag(std::string tag) {_entrytag = tag;};

		void doLog(LogLevel l, int32 line, const char *file, const char *message);

	protected:

		/* get a logentry */
		LogEntry* startLog();

		/**
		 * write log to handler.
		 */
		void endLog(LogEntry *);

	private:

		std::string addLevelStr(LogLevel level);

		std::string addtimeStr();

	private:

		std::map<LogDest, Handler *> handlers;
		std::string _entrytag;
		LogLevel _level;
		LogDest _dst;
};

////////////////////////////////////////////////////////////////////////////////
class DefaultLog
{
	public:
		
		~DefaultLog();

	public:

		static void initialize();

		static void initialize(uint16 logLevel, uint16 logWhere, std::string &logFile, uint32 fileSize);

		static void log(LogLevel l, int32 line, const char *file, const char *format, ...);

		static void shutdown();

	protected:

		DefaultLog() {};

		DefaultLog(const DefaultLog&) {};

		DefaultLog& operator= (const DefaultLog&);

	private:

		static AgentLog *agent;
};

}

#endif /* end of file */
