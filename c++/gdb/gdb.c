/***************************************************************************
 *   Copyright (C) 2016 by going
 *   Created on: 2016年7月25日
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
#include <stdio.h>
int func(int n)
{
        int sum=0,i;
        for(i=0; i<n; i++){
        		sum+=i;
          }
				return sum;
}

int main()
   {
      int i;
      long result = 0;
      for(i=1; i<=100; i++){
           result += i;
         }

       printf("result[1-100] = %d \n", result );
       printf("result[1-250] = %d \n", func(250) );
  }




