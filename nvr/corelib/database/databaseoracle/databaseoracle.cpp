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
#include <assert.h>
#include "ocilib.h"
#include "myoracle.h"
#include "databaseoracle.h"
#include "queryresultoracle.h"
#include "../common/sqloperation.h"
#include "../common/sqldelaythread.h"
#include "../../../library/util/log.h"
#include "../../../library/util/system.h"
#include "../../../library/concurrent/thread.h"

using namespace library;
using namespace corelib;
using namespace corelib::database;

////////////////////////////////////////////////////////////////////////////////
static std::string host = "192.168.0.130";
static std::string sid = "ZHSICSDB";
static uint16 port = 1521;
static std::string user = "si01";
static std::string passwd = "jp2011";

////////////////////////////////////////////////////////////////////////////////
DatatbaseOracle::DatatbaseOracle() : Database(), m_oracle(NULL)
{
	m_oracle = new MyOracle();
}

DatatbaseOracle::DatatbaseOracle(std::string &ip, uint16 p, std::string &u, std::string &pswd, std::string &s) : 
Database(), m_oracle(NULL)
{
	host = ip; sid = s; port = p; user = u; passwd = pswd;
	m_oracle = new MyOracle();
}

DatatbaseOracle::~DatatbaseOracle()
{
	if (m_delayThread) haltDelayThread();
	
	if (m_oracle) {
		m_oracle->shutdown();
		delete m_oracle;
		m_oracle = NULL;
	}
}

bool DatatbaseOracle::dbInitialize()
{
	Database::dbInitialize();
	MyOracle::Initialize();
	initDelayThread();

	bool bConnected = m_oracle->connectDb(host, port, sid, user, passwd, OCI_SESSION_DEFAULT);
	
	return bConnected;
}

void DatatbaseOracle::initDelayThread()
{
	assert(!m_delayThread);

	m_delayThread = new Thread(m_delayThreadBody = new SqlDelayThtread(this), "SQLDY");
	m_delayThread->start();
}

void DatatbaseOracle::haltDelayThread()
{
	if (!m_delayThreadBody || !m_delayThread) return;

	(static_cast<SqlDelayThtread*>(m_delayThreadBody))->stop();
	delete m_delayThread;
	delete m_delayThreadBody;

	m_delayThread = NULL;
	m_delayThreadBody = NULL;
}

QueryResult* DatatbaseOracle::query(const char *sql)
{
	if (m_oracle == NULL) return NULL;
	
	Synchronization queryAutoLock(lock_);

	uint64 _s = System::currentTimeMillis();

	QueryResult *result = m_oracle->query(sql);
	
	Log(LOG_DEBUG, "DatatbaseOracle::query - %s [%llu ms].", sql, System::currentTimeMillis() - _s);
	
	return result;
}

bool DatatbaseOracle::execute(const char *sql)
{
	if (m_oracle == NULL) return false;

	// don't use queued execution if it has not been initialized
	if (!m_delayThreadBody) return directExecute(sql);

	m_threadId = (THREADID)pthread_self();
	TransactionQueues::iterator i = m_tranQueues.find(m_threadId);
	if (i != m_tranQueues.end() && i->second != NULL) {
		i->second->delayExecute(sql);
	} else {
		dynamic_cast<SqlDelayThtread*>(m_delayThreadBody)->Delay(new SqlStatement(sql));
	}

	return true;
}

bool DatatbaseOracle::execute(SqlOperation *operation)
{
	if (!m_oracle || !m_delayThreadBody) return false;

	dynamic_cast<SqlDelayThtread*>(m_delayThreadBody)->Delay(operation);

	return true;
}

bool DatatbaseOracle::directExecute(const char *sql)
{
	if (!m_oracle) return false;

	Synchronization autoLock(lock_);

	uint64 _s = System::currentTimeMillis();

	bool bSucced = m_oracle->execute(sql);

	Log(LOG_DEBUG, "DatatbaseOracle::query - SQL: %s [%llu ms].", sql, System::currentTimeMillis() - _s);

	return bSucced;
}

bool DatatbaseOracle::beginTransaction()
{
	if (!m_oracle) return false;

	if (!m_delayThreadBody)
	{
		if (m_threadId == (THREADID)pthread_self()) return false;

		lock_.lock();
		///!oracle has not need transaction start declare.
//		if (!_transactionCmd("START TRANSACTION"))
//		{
//			lock_.unlock();
//			return false;
//		}
		return true;
	}

	m_threadId = (THREADID)pthread_self();
	TransactionQueues::iterator i = m_tranQueues.find(m_threadId);
	if (i != m_tranQueues.end() && i->second != NULL) {
		delete i->second;
		i->second = NULL;
	}
	m_tranQueues[m_threadId] = new SqlTransaction();

	return true;
}

///!do run sql or commit
bool DatatbaseOracle::commitTransaction()
{
	if (!m_oracle) return false;

	if (!m_delayThreadBody)
	{
		if (m_threadId == (THREADID)pthread_self()) return false;

		bool res = _transactionCmd("COMMIT");
		m_threadId = 0;
		lock_.unlock();
		return res;
	}

	m_threadId = (THREADID)pthread_self();
	TransactionQueues::iterator i = m_tranQueues.find(m_threadId);
	if (i != m_tranQueues.end() && i->second != NULL)
	{
		dynamic_cast<SqlDelayThtread*>(m_delayThreadBody)->Delay(i->second);
		i->second = NULL;
		return true;
	}

	return false;
}

bool DatatbaseOracle::rollbackTransaction()
{
	if (!m_oracle) return false;

	if (!m_delayThreadBody)
	{
		if (m_threadId == (THREADID)pthread_self()) return false;

		bool res = _transactionCmd("ROLLBACK");
		m_threadId = 0;
		lock_.unlock();
		return res;
	}

	m_threadId = (THREADID)pthread_self();
	TransactionQueues::iterator i = m_tranQueues.find(m_threadId);
	if (i != m_tranQueues.end() && i->second != NULL) {
		delete i->second;
		i->second = NULL;
	}

	return true;
}

bool DatatbaseOracle::_transactionCmd(const char *sql)
{
	if (sql == NULL) return false;
	if (!m_oracle) return false;

	bool bSucced = m_oracle->execute(sql);

	return bSucced;
}
