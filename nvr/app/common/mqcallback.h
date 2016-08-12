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
#ifndef __APP_COMMON_MQCALLBACK_H__
#define __APP_COMMON_MQCALLBACK_H__

#include <string>
#include "../../corelib/common/callback.h"

namespace app {

using corelib::_Callback;

class IMQCallback
{
	public:
	virtual void Execute(std::string &) = 0;
	virtual ~IMQCallback() {};
};

template < class CB >
class _IMQCallback : public CB, public IMQCallback
{
	public:
	_IMQCallback(CB const& cb) : CB(cb) {};
	
	virtual void Execute(std::string &cmd) {
		CB::m_param1 = cmd;
		CB::_Execute();
	};
	
	virtual ~_IMQCallback() {};
};

#if 0
template < class Class, typename ParamType1 = void, typename ParamType2 = void, typename ParamType3 = void >
class MQCallback :
	public _IMQCallback< _Callback < Class, std::string, ParamType1, ParamType2, ParamType3 > >
{
	private:
		typedef _Callback < Class, std::string, ParamType1, ParamType2, ParamType3 > MQC3;

	public:
		MQCallback(Class *object, typename MQC3::Method method, std::string cmd, ParamType1 param1, ParamType2 param2, ParamType3 param3):
				_IMQCallback< MQC3 >(MQC3(object, method, cmd, param1, param2, param3)) {};
};

template < class Class, typename ParamType1, typename ParamType2 >
class MQCallback<Class, ParamType1, ParamType2> :
	public _IMQCallback< _Callback < Class, std::string, ParamType1, ParamType2 > >
{
	private:
		typedef _Callback < Class, std::string, ParamType1, ParamType2 > MQC2;

	public:
		MQCallback(Class *object, typename MQC2::Method method, std::string cmd, ParamType1 param1, ParamType2 param2):
				_IMQCallback< MQC2 >(MQC2(object, method, cmd, param1, param2)) {};
};

template < class Class, typename ParamType1 >
class MQCallback<Class, ParamType1> :
	public _IMQCallback< _Callback < Class, std::string, ParamType1 > >
{
	private:
		typedef _Callback < Class, std::string, ParamType1 > MQC1;

	public:
		MQCallback(Class *object, typename MQC1::Method method, std::string cmd, ParamType1 param1):
				_IMQCallback< MQC1 >(MQC1(object, method, cmd, param1)) {};
};

#endif
template < class Class >
class MQCallback : public _IMQCallback< _Callback < Class, std::string > >
{
	private:

		typedef _Callback < Class, std::string > MQC0;

	public:

		MQCallback(Class *object, typename MQC0::Method method, std::string cmd):
			_IMQCallback< MQC0 >(MQC0(object, method, cmd)) {};
};

}

#endif /* end of file */