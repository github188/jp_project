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
#include "sqlcommit.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../corelib/database/databaseoracle/myoracle.h"
#include "../../corelib/database/databaseoracle/databaseoracle.h"

using namespace library;
using namespace corelib::database;
using namespace app;

SqlCommit::SqlCommit()
{
}

SqlCommit::~SqlCommit()
{
}

void SqlCommit::Execute(Database *db)
{
	if (db == NULL) return;

	uint64 tb = System::currentTimeMillis();

	DatatbaseOracle *mydb = static_cast<DatatbaseOracle*>(db);
	MyOracle* orcl = mydb->getDatabaseHandle();
	if (orcl == NULL) return;

	bool bCommited = orcl->commit();
	Log(LOG_DEBUG, "SqlCommit::Execute - committed: [%llu] %s", System::currentTimeMillis() - tb, bCommited ? "SUCCESS" : "FAIL!!!");
}