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
#include "querycallback.h"
#include "sqloperation.h"
#include "database.h"

using namespace corelib;
using namespace corelib::database;

void SqlStatement::Execute(Database * db)
{
	if (db == NULL) return;

	db->directExecute(m_sql);
}
////////////////////////////////////////////////////////////////////////////////
SqlTransaction::~SqlTransaction()
{
	while (!m_queue.empty())
	{
		free((void*)const_cast<char *>(m_queue.front()));
		m_queue.pop();
	}
}

void SqlTransaction::Execute(Database *db)
{
	if (db == NULL) return ;
	if (m_queue.empty()) return;

	db->directExecute("START TRANSACTION");
	while (!m_queue.empty())
	{
		const char *sql = m_queue.front();
		m_queue.pop();

		if (!db->directExecute(sql))
		{
			free((void*)const_cast<char *>(sql));
			db->directExecute("ROLLBACK");
			while (!m_queue.empty()) // once rollback happens, all transaction should be deleted
			{
				free((void*)const_cast<char *>(m_queue.front()));
				m_queue.pop();
			}
			return;
		}

		free((void*)const_cast<char *>(sql));
	}
	db->directExecute("COMMIT");
}

////////////////////////////////////////////////////////////////////////////////
SqlResultQueue::~SqlResultQueue()
{
	while(!empty())
    {
        IQueryCallback * callback = next();
        delete callback;
    }
}

void SqlResultQueue::Update()
{
	/// execute the callbacks waiting in the synchronization queue
    while(!empty())
    {
        IQueryCallback * callback = next();
        callback->Execute();
        delete callback;
    }
}

////////////////////////////////////////////////////////////////////////////////
void SqlQuery::Execute(Database *db)
{
	if(!m_callback || !m_queue) return;
    /// execute the query and store the result in the callback
    m_callback->SetResult(db->query(m_sql));
    /// add the callback to the sql result queue of the thread it originated from
    m_queue->add(m_callback);
}
