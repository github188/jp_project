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
#ifndef __CORELIB_DATABASE_DATABASEORACLE_H__
#define __CORELIB_DATABASE_DATABASEORACLE_H__

#include "../../../library/concurrent/lock.h"
#include "../common/database.h"
#include "ocilib.h"

namespace corelib {
namespace database {

class SqlOperation;
class MyOracle;
class DatatbaseOracle : public Database
{
	private:

		DatatbaseOracle(const DatatbaseOracle&);
		DatatbaseOracle &operator= (const DatatbaseOracle&);

	public:

		DatatbaseOracle();
		DatatbaseOracle(std::string &ip, uint16 p, std::string &u, std::string &pswd, std::string &s);
		~DatatbaseOracle();

	public:

		virtual bool dbInitialize();

		virtual void initDelayThread();
		virtual void haltDelayThread();

		virtual QueryResult* query(const char *sql);
		virtual bool execute(const char *sql);
		virtual bool directExecute(const char *sql);

		virtual bool beginTransaction();
		virtual bool commitTransaction();
		virtual bool rollbackTransaction();

		virtual operator bool ()const {return m_oracle != NULL;};

	public:

		bool execute(SqlOperation *operation);

		MyOracle *getDatabaseHandle()const {return m_oracle;};

	private:

		bool _transactionCmd(const char *sql);

	private:

		THREADID m_threadId; //current thread identifier who use this dabase instance
		MyOracle *m_oracle;
		library::Lock lock_;
};

}
}

#endif /* end of file */