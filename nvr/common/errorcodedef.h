/***************************************************************************
 *   Copyright (C) 2015 by Y.S.G   										   *
 *   ysgen0217@163.com   												   *
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
#ifndef __LIBRARY_COMMON_ERRORCODEDEF_H__
#define __LIBRARY_COMMON_ERRORCODEDEF_H__

/////////////////////////////////error code define//////////////////////////////

/* mutex */
#define SYS_MUTEX_INIT_ERROR_CODE					50

#define SYS_MUTEX_LOCK_ERROR_CODE					51

#define SYS_MUTEX_UNLOCK_ERROR_CODE					52

#define SYS_RWLOCK_INIT_ERROR_CODE					53

#define SYS_RWLOCK_READLOCK_ERROR_CODE				54

#define SYS_RWLOCK_WRITELOCK_ERROR_CODE				55

#define SYS_RWLOCK_UNLOCK_ERROR_CODE				56

#define SYS_COND_INIT_ERROR_CODE					57

#define SYS_THREAD_GET_STACK_ERROR_CODE				58

#define SYS_THREAD_SET_STACK_ERROR_CODE				59

#define SYS_THREAD_ATTR_INIT_ERROR_CODE				60

#define SYS_THREAD_GET_POLICY_ERROR_CODE			61

#define SYS_THREAD_SET_SCHED_PARAM_ERROR_CODE		62

#define SYS_THREAD_CREATE_NEW_THREAD_ERROR_CODE		63

/////////////////////////////////error indication///////////////////////////////

/* mutex */
#define SYS_MUTEX_INIT_ERROR_CODE_STRING		"mutex init error"

#endif

