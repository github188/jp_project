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
#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;
void err_handler(OCI_Error *err)
{
    int   err_type = OCI_ErrorGetType(err);
    char *err_msg  = (char*)OCI_ErrorGetString(err);

    printf("%s - %s\n", err_type == OCI_ERR_WARNING ? "warning" : "error", err_msg);
}

Oracle::Oracle(const char * ip, const char *user, const char *passwd)
{
		cn = NULL;
		st = NULL;
		rs = NULL;
		nRet = OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);
		if(!nRet)
		{	perror("fail to oracle");
			err_handler(OCI_GetLastError());
			std::cout << "OCI_Initialize fail" << std::endl;
		}
		cn = OCI_ConnectionCreate(ip, user, passwd,  OCI_SESSION_DEFAULT);
		OCI_SetAutoCommit(cn, TRUE);
		printf("Server major    version : %i\n",   OCI_GetServerMajorVersion(cn));
		printf("Server minor    version : %i\n",   OCI_GetServerMinorVersion(cn));
		printf("Server revision version : %i\n\n", OCI_GetServerRevisionVersion(cn));
		printf("Connection      version : %i\n\n", OCI_GetVersionConnection(cn));

		//��������
		st = OCI_StatementCreate(cn);
		if(NULL == st)
		{
			std::cout << "OCI_StatementCreate fail" << std::endl;
			err_handler(OCI_GetLastError());
			nRet = -3;

		}

}
Oracle::~Oracle()
{
	if(rs)
		OCI_ReleaseResultsets (st);


	if(st)
		OCI_StatementFree(st);

	//
	if(cn)
		OCI_ConnectionFree (cn);

	//���OCI��
	OCI_Cleanup();
}
void Oracle::execute(const char * sql)
{	rs = NULL;
	if(!OCI_Prepare(st, sql))
			{
				std::cout << "OCI_Prepare fail" << std::endl;
				err_handler(OCI_GetLastError());
				nRet = -4;

			}
			//�󶨱������ñ����滻ռλ��
		  /*  if(!OCI_BindString (st, ":value", value, strlen(value)))
		    {
		    	std::cout << "OCI_BindString fail" << std::endl;
		    	err_handler(OCI_GetLastError());
		    	nRet = -5;
		    	break;
		    }
		    */
		    //ִ������
			if(!OCI_Execute(st))
			{
				std::cout << "OCI_Execute fail" << std::endl;
		    	err_handler(OCI_GetLastError());
		    	nRet = -6;

			}

}
OCI_Resultset* Oracle::result()
{
	if(rs==NULL){
		rs = OCI_GetResultset(st);
			if(NULL == rs)
			{
				std::cout << "OCI_GetResultset fail" << std::endl;
				err_handler(OCI_GetLastError());
				nRet = -5;
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

