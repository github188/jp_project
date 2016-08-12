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
#ifndef __CORELIB_DATABASE_DATABASEORACLE_ORACLE_COM_H__
#define __CORELIB_DATABASE_DATABASEORACLE_ORACLE_COM_H__

#include "ocilib.h"

namespace corelib {
namespace database {
	
enum enum_field_types
{
	ORACLE_OCI_CDT_NUMERIC,
	ORACLE_OCI_CDT_DATETIME,
	ORACLE_OCI_CDT_TEXT,
	ORACLE_OCI_CDT_LONG,
	ORACLE_OCI_CDT_CURSOR,
	ORACLE_OCI_CDT_LOB,
	ORACLE_OCI_CDT_FILE,
	ORACLE_OCI_CDT_TIMESTAMP,
	ORACLE_OCI_CDT_INTERVAL,
	ORACLE_OCI_CDT_RAW,
	ORACLE_OCI_CDT_OBJECT,
	ORACLE_OCI_CDT_COLLECTION,
	ORACLE_OCI_CDT_REF
};

}
}

#endif /* end of file */
