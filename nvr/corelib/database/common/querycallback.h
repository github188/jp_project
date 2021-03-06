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
#ifndef __APP_COMMON_QUERYCALLBACK_H__
#define __APP_COMMON_QUERYCALLBACK_H__

#include "../../common/callback.h"

namespace corelib {
namespace database {

using corelib::_Callback;
class QueryResult;
class IQueryCallback
{
    public:
        virtual void Execute() = 0;
        virtual ~IQueryCallback() {}
        virtual void SetResult(QueryResult* result) = 0;
        virtual QueryResult* GetResult() = 0;
};

template < class CB >
class _IQueryCallback : public CB, public IQueryCallback
{
	public:
	    _IQueryCallback(CB const& cb) : CB(cb) {}
	    void Execute() { CB::_Execute(); }
	    void SetResult(QueryResult* result) { CB::m_param1 = result; }
	    QueryResult* GetResult() { return CB::m_param1; }
};

template < class Class, typename ParamType1 = void, typename ParamType2 = void, typename ParamType3 = void >
class QueryCallback : 
    public _IQueryCallback< _Callback < Class, QueryResult*, ParamType1, ParamType2, ParamType3 > >
{
    private:
        typedef _Callback < Class, QueryResult*, ParamType1, ParamType2, ParamType3 > QC3;
    public:
        QueryCallback(Class *object, typename QC3::Method method, QueryResult* result, ParamType1 param1, ParamType2 param2, ParamType3 param3)
            : _IQueryCallback< QC3 >(QC3(object, method, result, param1, param2, param3)) {}
};

template < class Class, typename ParamType1, typename ParamType2 >
class QueryCallback < Class, ParamType1, ParamType2 > : 
    public _IQueryCallback< _Callback < Class, QueryResult*, ParamType1, ParamType2 > >
{
    private:
        typedef _Callback < Class, QueryResult*, ParamType1, ParamType2 > QC2;
    public:
        QueryCallback(Class *object, typename QC2::Method method, QueryResult* result, ParamType1 param1, ParamType2 param2)
            : _IQueryCallback< QC2 >(QC2(object, method, result, param1, param2)) {}
};

template < class Class, typename ParamType1 >
class QueryCallback < Class, ParamType1 > : 
    public _IQueryCallback< _Callback < Class, QueryResult*, ParamType1 > >
{
    private:
        typedef _Callback < Class, QueryResult*, ParamType1 > QC1;
    public:
        QueryCallback(Class *object, typename QC1::Method method, QueryResult* result, ParamType1 param1)
            : _IQueryCallback< QC1 >(QC1(object, method, result, param1)) {}
};

template < class Class >
class QueryCallback < Class > : public _IQueryCallback< _Callback < Class, QueryResult* > >
{
    private:
        typedef _Callback < Class, QueryResult* > QC0;
    public:
        QueryCallback(Class *object, typename QC0::Method method, QueryResult* result)
            : _IQueryCallback< QC0 >(QC0(object, method, result)) {}
};

}
}
#endif /* end of file */