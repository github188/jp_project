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

#include "iostream"
#include "mutex.h"
#include <map>
#include <string>
#include <vector>
#include <stdio.h>

#define LOG(level, format, ...) DefaultLog::log(level, __LINE__, __FILE__, format, ##__VA_ARGS__)
#define MAX_BUFFER_SIZE_064 64
using namespace library;
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

class AgentLog
{
	public:

		AgentLog();

		AgentLog(LogLevel level, char * log_file_name);

		~AgentLog();

	public:

		/**
		 * set write handler for log
		 *
		 * @param handler handler to set.
		 */

		/**
		 * set tag to the logentry handle without giving a name
		 */

 void doLog(LogLevel l, int32 line, const char *file, const char *message);

	protected:

		/* get a logentry */


		/**
		 * write log to handler.
		 */

	private:

 	 std::string addLevelStr(LogLevel level);

 	 std::string addtimeStr();

	private:
		FILE * fl_stream;
		std::string _entrytag;
		LogLevel _level;
		Mutex *_lock;
		char file_line_buf[100];
};

class DefaultLog
{
	public:

		~DefaultLog();

	public:

		static void initialize();

		static void initialize(uint16 logLevel, char * log_file_name);

		static void log(LogLevel l, int32 line, const char *file, const char *format, ...);

		static void shutdown();

	protected:

		DefaultLog() {};

		DefaultLog(const DefaultLog&) {};

		DefaultLog& operator= (const DefaultLog&);

	private:

		static AgentLog *agent;
};





