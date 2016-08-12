/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016��2��18��
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   myoracle.h                                                                      *
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


#ifndef MYORACLE_H_
#define MYORACLE_H_
#include "ocilib.h"
class Oracle{
public:
	Oracle(const char * ip, const char *user, const char *passwd);
	~Oracle();
	void execute(const char * sql);
	OCI_Connection* get_cn();
	OCI_Resultset* result();
	void clear_result(){
		OCI_ReleaseResultsets(st);
	}
public:



private:
	OCI_Connection* cn ;
	OCI_Statement* st;
	OCI_Resultset* rs;
	int nRet;

};




#endif /* MYORACLE_H_ */
