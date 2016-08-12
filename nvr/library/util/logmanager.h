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
#ifndef __LIBRARY_UTIL_LOGMANAGER_H__
#define __LIBRARY_UTIL_LOGMANAGER_H__

#include <string>
#include <map>

#include "singleton.h"

namespace library {

/**
 * create and store logentries for threads.
 * this class is created for only once with Singleton, users should
 * initializes it before other threads is running in main thread.
 *
 */
class LogEntry;
class LogManager
{
	public:

		LogManager();
		
		~LogManager();

	protected:

		LogManager(const LogManager&) {};

		LogManager& operator= (const LogManager&);

	public:

		LogEntry* getDefaultLogEntry();

		/* get a logentry attached with this thread */
		LogEntry* getLogEntry();

		/* delete all logentries */
		void shutdownAll();

		/* destroy the thread logentry */
		void shutdownSelf();

	private:

		/* create a logentry with name 'DEFT' */
		void initialize();

		/* create a logEntry with thead id, user can set name for this thread */
		LogEntry* createEntry(uint64 id);

	private:

		std::map<uint64, LogEntry *> entries;
		
		LogEntry *defaultEntry;
};

////////////////////////////////////////////////////////////////////////////////
/* make it be a global instance */
typedef Singleton<LogManager> LogManagerMgr;

////////////////////////////////////////////////////////////////////////////////

}

#endif /* end of file */
