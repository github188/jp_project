/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016��2��23��
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   myoracle.cpp                                                                      *
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
#include "../log/log.h"
#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;

void error_handler(OCI_Error *err)
{
	int err_type = OCI_ErrorGetType(err);
	const char *err_msg = OCI_ErrorGetString(err);
	LOG(LOG_ERROR,"** %s - %s\n", err_type == OCI_ERR_WARNING ? "Warning" : "Error", err_msg);
}

Oracle::Oracle(const char * ip, const char *user, const char *passwd)
{
		cn = NULL;
		st = NULL;
		rs = NULL;
		nRet = OCI_Initialize(error_handler, "oracle", OCI_ENV_EVENTS);
		if(!nRet)
		{
			LOG(LOG_ERROR,"OCI_Initialize fail");


		}
		cn = OCI_ConnectionCreate(ip, user, passwd,  OCI_SESSION_DEFAULT);
		if( cn == NULL ){
			exit(-1);
		}
		OCI_SetAutoCommit(cn, TRUE);
		LOG(LOG_INFO,"Server major    version : %i\n",   OCI_GetServerMajorVersion(cn));
		LOG(LOG_INFO,"Server minor    version : %i\n",   OCI_GetServerMinorVersion(cn));
		LOG(LOG_INFO,"Server revision version : %i\n\n", OCI_GetServerRevisionVersion(cn));
		LOG(LOG_INFO,"Connection      version : %i\n\n", OCI_GetVersionConnection(cn));


		st = OCI_StatementCreate(cn);
		if(NULL == st)
		{
			LOG(LOG_ERROR,"OCI_StatementCreate fail" );
			exit(-1);


		}

}
Oracle::~Oracle()
{
	if(rs)
		OCI_ReleaseResultsets (st);


	if(st)
		OCI_StatementFree(st);


	if(cn)
		OCI_ConnectionFree (cn);

	OCI_Cleanup();
}
void Oracle::execute(const char * sql)
{	rs = NULL;
	if(!OCI_Prepare(st, sql))
			{
			LOG(LOG_ERROR,"OCI_Prepare fail" );



			}

			if(!OCI_Execute(st))
			{
				LOG(LOG_ERROR,"error sql:: %s",sql);


			}

}
OCI_Resultset* Oracle::result()
{
	if(rs==NULL){
		rs = OCI_GetResultset(st);
			if(NULL == rs)
			{
				LOG(LOG_ERROR,"OCI_GetResultset fail, the result is NULL" );
			}
	}
	if(OCI_FetchNext(rs))
		return rs;
	else
		return NULL;
}

OCI_Connection* Oracle::get_cn(){
	return cn;
}

