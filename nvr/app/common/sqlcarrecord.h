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
#ifndef __APP_COMMON_SQLCARRECORD_H__
#define __APP_COMMON_SQLCARRECORD_H__

#include <set>
#include <list>
#include <string>

#include "eprotocol.h"
#include "../../corelib/database/common/sqloperation.h"
#include "../../corelib/database/databaseoracle/ocilib.h"

namespace app {

namespace CD = corelib::database;
//class CD::Database;
class SqlCarRecord : public CD::SqlOperation
{
	private:

		SqlCarRecord(const SqlCarRecord&);
		SqlCarRecord& operator= (const SqlCarRecord&);

	public:

		SqlCarRecord(std::list<ST_CAR_RECORD_INFO> &dl);
		~SqlCarRecord();

	public:

		virtual void Execute(CD::Database *db);

	public:

		std::string formatNewCarRecord();
		std::string formatHeadCarRecord();
		std::string formatBackCarRecord();
		std::string formatFullCarRecord();
		std::string formatSpecialBlockNewCarRecord();
		std::string formatSpecialBlockHeadCarRecord();
		std::string formatSpecialBlockBackCarRecord();
		void addBindingNewRecord(OCI_Statement *st);
		void addBindingHeadRecord(OCI_Statement *stmt);
		void addBindingBackRecord(OCI_Statement *stmt);
		void addBindingFullRecord(OCI_Statement *stmt);
		void addBindingSpecialBlockNewRecord(OCI_Statement *stmt);
		void addBindingSpecialBlockHeadRecord(OCI_Statement *stmt);
		void addBindingSpecialBlockBackRecord(OCI_Statement *stmt);

	private:

		void initBindingVars();
		void update(ST_CAR_DESCRIPTION_INFO *record);
		std::string toText(ST_CAR_DESCRIPTION_INFO *record);

	private:

		std::list<ST_CAR_RECORD_INFO> m_dl;
		std::set<std::string> specialblocks;
		
		std::string xxbh, kkbh, cdbh, hphm, hpys, cwhphm, cwhpys, hpyz, tx1, tx2, sbcd, qydm, clbs;
		std::string tx3, tx4, tx5, xszt, clpp, clwx, csys, cllx, hpzl, sbbh, cj, kkmc, dwmc, sbfx, sbmc;
		OCI_Date *jgsj, *date, *predate;
		float clsd, clcd, clxs;
		int txsl;
};

}

#endif