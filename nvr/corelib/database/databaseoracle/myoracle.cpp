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
#include "myoracle.h"
#include "queryresultoracle.h"
#include "../common/sqlstring.h"
#include "../../../library/util/log.h"

using namespace library;
using namespace corelib;
using namespace corelib::database;

////////////////////////////////////////////////////////////////////////////////
static void dbErrHandler(OCI_Error *err)
{  
    int32 iErrType = OCI_ErrorGetType(err);  
    char *pcErrMsg  = (char*)OCI_ErrorGetString(err);
  
    Log(LOG_ERROR, "%s-%s", (iErrType == OCI_ERR_WARNING) ? "warning" : "error", 
		pcErrMsg);
}

////////////////////////////////////////////////////////////////////////////////
size_t MyOracle::db_count = 0;

////////////////////////////////////////////////////////////////////////////////
MyOracle::MyOracle() : m_oracle(NULL)
{
	
}

MyOracle::~MyOracle()
{
}

void MyOracle::Initialize()
{
	if (db_count++ == 0) {

		if (!OCI_Initialize(dbErrHandler, NULL, OCI_ENV_CONTEXT | OCI_ENV_THREADED)) {
			Log(LOG_ERROR, "MyOracle::MyOracle - FATAL ERROR, oracle library init fail.");
			exit(1);
		} else {
			Log(LOG_DEBUG, "MyOracle::MyOracle - oracle library INIT SUCCESS.");
		}
	}
}

void MyOracle::shutdown()
{
	if (m_oracle) {
		OCI_ConnectionFree(m_oracle);
		m_oracle= NULL;
	}

	if (--db_count == 0) {
		OCI_Cleanup();
	}
}

bool MyOracle::connectDb(std::string &host, uint16 port, std::string &sid, std::string &user, std::string &passwd, uint32 mode)
{
	SqlString dbConnStr;

	dbConnStr << host << ":" << port << "/" << sid;
	m_oracle = OCI_ConnectionCreate(dbConnStr.preview().c_str(), user.c_str(), passwd.c_str(), mode);
	if (m_oracle == NULL) {
		Log(LOG_ERROR, "MyOracle::execute - connect oracle database: %s fail.", dbConnStr.preview().c_str());
		return false;
	}

	Log(LOG_DEBUG, "MyOracle::execute - ++++++ Connect database: %s success ++++++", dbConnStr.preview().c_str());
	Log(LOG_DEBUG, "MyOracle::execute - ++++++ Client library version: %d   ++++++", OCI_GetVersionConnection(m_oracle));
//	Log(LOG_DEBUG, "MyOracle::execute - ++++++ DataBase version: %s         ++++++", OCI_GetVersionServer(m_oracle));

	return true;
}

QueryResult* MyOracle::query(const char *sql)
{
	if (m_oracle == NULL) return NULL;
	
	OCI_Statement *stmt = OCI_StatementCreate(m_oracle);
	if (stmt == NULL) {
		return NULL;
	}

	bool bSucced = OCI_ExecuteStmt(stmt, sql);
	if (!bSucced) {
		OCI_StatementFree(stmt);
		Log(LOG_ERROR, "MyOracle::query - excute: %s error.", sql);
		return NULL;
	}

	OCI_Resultset *result = OCI_GetResultset(stmt);
	if (!result) return NULL;

	uint32 fieldCount = OCI_GetColumnCount(result);
	uint64 rowCount = OCI_GetRowCount(result);
	
	QueryResult *queryResult = new QueryResultOracle(stmt, result, rowCount, fieldCount);
	queryResult->nextRow();

	return queryResult;
}

bool MyOracle::execute(const char *sql)
{
	if (m_oracle == NULL) return false;
	
	OCI_Statement *stmt = OCI_StatementCreate(m_oracle);
	if (stmt == NULL) {
		return false;
	}

	bool bSucced = OCI_ExecuteStmt(stmt, sql);
	if (!bSucced) {
		OCI_StatementFree(stmt);
		Log(LOG_ERROR, "MyOracle::execute - excute: %s error.", sql);
		Log(LOG_ERROR, "MyOracle::execute - SQL ERROE %s.", OCI_ErrorGetString(OCI_GetLastError()));
		return false;
	}
	OCI_StatementFree(stmt);

	return true;
}

OCI_Statement *MyOracle::prepare(std::string sql)
{
	OCI_Statement *stmt = OCI_StatementCreate(m_oracle);
	if (stmt == NULL) {
		return NULL;
	}

	if (!OCI_Prepare(stmt, sql.c_str())) {
		return NULL;
	}

	return stmt;
}

bool MyOracle::execute(OCI_Statement *stmt)
{
	return OCI_Execute(stmt);
}

bool MyOracle::commit()
{
	if (!m_oracle) return false;
	
	return OCI_Commit(m_oracle);
}