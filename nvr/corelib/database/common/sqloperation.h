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
#ifndef __CORELIB_DATABASE_COMMON_SQLOPERATION_H__
#define __CORELIB_DATABASE_COMMON_SQLOPERATION_H__

#include <queue>
#include <string.h>
#include <stdlib.h>

#include "../../../library/util/lockqueue.h"

namespace corelib {
namespace database {

class IQueryCallback;
class Database;
class SqlOperation
{
    public:
        virtual void OnRemove() { delete this; }
        virtual void Execute(Database *db) = 0;
        virtual ~SqlOperation() {}
};

////////////////////////////////////////////////////////////////////////////////
class SqlStatement : public SqlOperation
{
    private:
        const char *m_sql;
    public:
        SqlStatement(const char *sql) : m_sql(strdup(sql)){}
        ~SqlStatement() { free((void*)const_cast<char*>(m_sql)); }
        void Execute(Database *db);
};

////////////////////////////////////////////////////////////////////////////////
class SqlTransaction : public SqlOperation
{
	private:
		std::queue<const char *> m_queue;
	public:

		SqlTransaction() {};
		~SqlTransaction();
		void delayExecute(const char *sql) { m_queue.push(strdup(sql)); }
		void Execute(Database *db);
};

////////////////////////////////////////////////////////////////////////////////
class SqlResultQueue : public library::LockedQueue<IQueryCallback*>
{
    public:
        SqlResultQueue() {}
		~SqlResultQueue();
        void Update();
};

////////////////////////////////////////////////////////////////////////////////
class SqlQuery : public SqlOperation
{
	private:
		const char *m_sql;
		IQueryCallback *m_callback;
		SqlResultQueue *m_queue;
	public:

		SqlQuery(const char *sql, IQueryCallback * callback, SqlResultQueue *queue)
            : m_sql(strdup(sql)), m_callback(callback), m_queue(queue) {}
		~SqlQuery() { free((void*)const_cast<char*>(m_sql)); }
		void Execute(Database *db);
};

}
}

#endif /* end of file */