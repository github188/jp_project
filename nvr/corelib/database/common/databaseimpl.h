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
#ifndef __CORELIB_DATABASE_COMMON_DATABASEIMPL_H__
#define __CORELIB_DATABASE_COMMON_DATABASEIMPL_H__

#include "database.h"
#include "sqloperation.h"
#include "querycallback.h"
#include "sqldelaythread.h"

namespace corelib {
namespace database {

template<class Class>
bool Database::asyncQuery(Class *object, void (Class::*method)(QueryResult*), const char *sql)
{
	if (sql == NULL) return false;

	THREADID myId = (THREADID)pthread_self();
	QueryQueues::iterator iter = m_queryQueues.find(myId);
	if (iter == m_queryQueues.end()) return false;
	(static_cast<SqlDelayThtread*>(m_delayThreadBody))->Delay(new SqlQuery(sql, new QueryCallback<Class>(object, method, (QueryResult*)NULL), iter->second));
	
	return true;
}
		
template<class Class, typename ParamType1>
bool Database::asyncQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1), ParamType1 param1, const char *sql)
{
	if (sql == NULL) return false;

	THREADID myId = (THREADID)pthread_self();
	QueryQueues::iterator iter = m_queryQueues.find(myId);
	if (iter == m_queryQueues.end()) return false;
	(static_cast<SqlDelayThtread*>(m_delayThreadBody))->Delay(new SqlQuery(sql, new QueryCallback<Class, ParamType1>(object, method, (QueryResult*)NULL, param1), iter->second));
	
	return true;
}

}
}
#endif /* end of file */