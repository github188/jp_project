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
#include "queryresultoracle.h"
#include "../../../library/util/log.h"

using namespace library;
using namespace corelib;
using namespace corelib::database;

QueryResultOracle::QueryResultOracle(OCI_Statement *stmt, OCI_Resultset *result, uint64 rowCount, uint32 fieldCount) : 
QueryResult(rowCount, fieldCount), m_stmt(stmt), m_result(result)
{
	mCurrentRow = new Field[mFieldCount];
	assert(mCurrentRow != NULL);

	for (uint32 i = 0; i < fieldCount; ++i) {
		mCurrentRow[i].setName(OCI_GetColumnName(OCI_GetColumn(m_result, i + 1)));
		mCurrentRow[i].setType(convertNativeType(OCI_ColumnGetType(OCI_GetColumn(m_result, i + 1))));
	}
}

QueryResultOracle::~QueryResultOracle()
{
	endQuery();
}

bool QueryResultOracle::nextRow()
{
	if (m_result != NULL) {
		if (OCI_FetchNext(m_result)) {
			for (uint32 i = 0; i < mFieldCount; ++i) {

				uint32 columType = OCI_ColumnGetType(OCI_GetColumn(m_result, i + 1));
				if (columType == OCI_CDT_DATETIME) {
					int32 year, month, day, hour, min, sec;
					char dateStr[64] = {0};
					
					OCI_DateGetDateTime(OCI_GetDate(m_result, i + 1), &year, &month, &day, &hour, &min, &sec);
					sprintf(dateStr, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
					mCurrentRow[i].setValue(dateStr);
					continue;
				}
				mCurrentRow[i].setValue(OCI_GetString(m_result, i + 1));
			}
			return true;
		}
	}

	return false;
}

enum Field::DataTypes QueryResultOracle::convertNativeType(unsigned int ocilibType) const
{
	switch (ocilibType)
	{
	case OCI_CDT_NUMERIC:
		return Field::DB_TYPE_INTEGER;
	case OCI_CDT_TIMESTAMP:
	case OCI_CDT_DATETIME:
	case OCI_CDT_TEXT:
	case OCI_CDT_LONG:
	case OCI_CDT_LOB:
		return Field::DB_TYPE_STRING;
	default:
		return Field::DB_TYPE_UNKNOWN;
	}
}

void QueryResultOracle::endQuery()
{
	if (mCurrentRow != NULL) {
		delete []mCurrentRow;
		mCurrentRow = NULL;
	}

	OCI_StatementFree(m_stmt);
	if (m_result) {
		///!result will be released when statement is destroyed or reused.
		m_result = NULL;
	}
}