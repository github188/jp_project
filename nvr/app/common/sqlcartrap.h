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
#ifndef __APP_COMMON_SQLCARTRAP_H__
#define __APP_COMMON_SQLCARTRAP_H__

#include <string>

#include "eprotocol.h"
#include "../../corelib/database/common/sqloperation.h"
#include "../../corelib/database/databaseoracle/ocilib.h"

namespace app {

namespace CD = corelib::database;
class SqlCarTrap : public CD::SqlOperation
{
	private:

		SqlCarTrap(const SqlCarTrap&);
		SqlCarTrap& operator= (const SqlCarTrap&);

	public:

		SqlCarTrap(ST_CAR_TRAP_INFO &trap);
		~SqlCarTrap();

	public:

		virtual void Execute(CD::Database *db);

	private:

		std::string formatCarTrap();
		void addBindingCarTrap(OCI_Statement *stmt);
		void update();

	private:

		ST_CAR_TRAP_INFO m_trap;

		std::string clxxbh, bkxxbh, kkbh, fxbh, hphm, hpys, cwhphm, cwhpys;
		std::string hpyz, clpp, clwx, csys, hpzl, txmc1, txmc2, bjlx;
		OCI_Date *jgsk, *bjsk;
		int32 txsl;
		float clsd;
};

}

#endif /* end of file */