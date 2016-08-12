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
#ifndef __CORELIB_DATABASE_DATABASEORACLE_QUERYRESULTORACLE_H__
#define __CORELIB_DATABASE_DATABASEORACLE_QUERYRESULTORACLE_H__

#include "../common/queryresult.h"

namespace corelib {
namespace database {

class QueryResultOracle : public QueryResult
{
	private:

		QueryResultOracle(const QueryResultOracle&);
		QueryResultOracle& operator= (const QueryResultOracle&);

	public:

		QueryResultOracle(OCI_Statement *stmt, OCI_Resultset *result, uint64 rowCount, uint32 fieldCount);
		~QueryResultOracle();

	public:

		virtual bool nextRow();

	private:

		enum Field::DataTypes convertNativeType(unsigned int oracleType) const;
		void endQuery();

	private:

		OCI_Statement *m_stmt;
		OCI_Resultset *m_result;
};

}
}

#endif /* end of file */