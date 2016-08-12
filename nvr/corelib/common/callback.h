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
#ifndef __CORELIB_COMMON_CALLBACK_H__
#define __CORELIB_COMMON_CALLBACK_H__

namespace corelib {

template < class Class, typename ParamType1 = void, typename ParamType2 = void, typename ParamType3 = void, typename ParamType4 = void >
class _Callback
{
	protected:
	    typedef void (Class::*Method)(ParamType1, ParamType2, ParamType3, ParamType4);
	    Class *m_object;
	    Method m_method;
	    ParamType1 m_param1;
	    ParamType2 m_param2;
	    ParamType3 m_param3;
	    ParamType4 m_param4;
	    void _Execute() { (m_object->*m_method)(m_param1, m_param2, m_param3, m_param4); }
    public:
        _Callback(Class *object, Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3, ParamType4 param4)
            : m_object(object), m_method(method), m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4) {}
        _Callback(_Callback < Class, ParamType1, ParamType2, ParamType3, ParamType4> const& cb)
            : m_object(cb.object), m_method(cb.m_method), m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3), m_param4(cb.m_param4) {}
};

template < class Class, typename ParamType1, typename ParamType2, typename ParamType3 >
class _Callback < Class, ParamType1, ParamType2, ParamType3 >
{
    protected:
        typedef void (Class::*Method)(ParamType1, ParamType2, ParamType3);
        Class *m_object;
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        ParamType3 m_param3;
        void _Execute() { (m_object->*m_method)(m_param1, m_param2, m_param3); }
    public:
        _Callback(Class *object, Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3)
            : m_object(object), m_method(method), m_param1(param1), m_param2(param2) {}
        _Callback(_Callback < Class, ParamType1, ParamType2, ParamType3 > const& cb)
            : m_object(cb.object), m_method(cb.m_method), m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3) {}
};

template < class Class, typename ParamType1, typename ParamType2 >
class _Callback < Class, ParamType1, ParamType2 >
{
    protected:
        typedef void (Class::*Method)(ParamType1, ParamType2);
        Class *m_object;
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        void _Execute() { (m_object->*m_method)(m_param1, m_param2); }
    public:
        _Callback(Class *object, Method method, ParamType1 param1, ParamType2 param2)
            : m_object(object), m_method(method), m_param1(param1), m_param2(param2) {}
        _Callback(_Callback < Class, ParamType1, ParamType2 > const& cb)
            : m_object(cb.m_object), m_method(cb.m_method), m_param1(cb.m_param1), m_param2(cb.m_param2) {}
};

template < class Class, typename ParamType1 >
class _Callback < Class, ParamType1 >
{
    protected:
        typedef void (Class::*Method)(ParamType1);
        Class *m_object;
        Method m_method;
        ParamType1 m_param1;
        void _Execute() { (m_object->*m_method)(m_param1); }
    public:
        _Callback(Class *object, Method method, ParamType1 param1)
            : m_object(object), m_method(method), m_param1(param1) {}
        _Callback(_Callback < Class, ParamType1 > const& cb)
            : m_object(cb.m_object), m_method(cb.m_method), m_param1(cb.m_param1) {}
};

template < class Class >
class _Callback < Class >
{
    protected:
        typedef void (Class::*Method)();
        Class *m_object;
        Method m_method;
        void _Execute() { (m_object->*m_method)(); }
    public:
        _Callback(Class *object, Method method)
            : m_object(object), m_method(method) {}
        _Callback(_Callback < Class > const& cb)
            : m_object(cb.m_object), m_method(cb.m_method) {}
};

}

#endif /* end of file */