/***************************************************************************
 *   Copyright (C) 2016 by going
 *   Created on: 2016年7月20日
 *   Author: going                                           *
 *   going369@163.com                                                   *                                                                  *
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
/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016��2��23��
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   test.cpp                                                                      *
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
/*********************************************/
#include "../../module/oracle/myoracle.h"
#include "ocilib.h"
#include "stdio.h"
#include "../../module/log/log.h"
int main()
{
	DefaultLog::initialize(LOG_DEBUG,"out.log");
	Oracle oracle("172.31.108.194:1521/SICSDB", "si01", "jp2011");
	OCI_Resultset * rs = NULL;
	//OCI_Connection * cn =NULL;
	//cn = oracle.get_cn();
	//oracle.execute("insert into carinfo_tab (XXBH)values(440400140000000012);");
		oracle.execute("select * from car_tab_temp");
		while((rs = oracle.result()) != NULL){
			LOG(LOG_INFO,"%s - %s - %s - %s- %s - %s - %s\n", OCI_GetString(rs, 1),
									OCI_GetString(rs,2), OCI_GetString(rs,5), OCI_GetString(rs,6),
									OCI_GetString(rs,10), OCI_GetString(rs,11), OCI_GetString(rs,12)
									);


			}


}









