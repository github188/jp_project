/***************************************************************************
 *   Copyright (C) 2016 by going
 *   Created on: 2016年4月26日
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
#include "log.h"
#include <unistd.h>
#define Log(level, format, ...) DefaultLog::log(level, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
int main()
{

	//DefaultLog::initialize(LOG_DEBUG,"log.log");
	while(1){
		Log(LOG_DEBUG,"hello Iam going %d ", 19);
		sleep(1);
	}
}




