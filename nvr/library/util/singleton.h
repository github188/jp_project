/***************************************************************************
 *   Copyright (C) 2015 by root   				   *
 *   ysgen0217@163.com   							   *
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
#ifndef __LIBRARY_UTIL_SINGLETON_H__
#define __LIBRARY_UTIL_SINGLETON_H__

#include "../concurrent/lock.h"

namespace library {

/**
 * this class keeps only a instance of type 'T'.
 * Note: this kind of object should be initialized in main.
 */
template <typename T> class Singleton
{
    protected:
		
        Singleton(){};

    private:

		static T *_instance;
		
        static Lock *_pLock;

    public:
		
        virtual ~Singleton();
        
        static T *getInstance();
};

////////////////////////////////////////////////////////////////////////////////
template <typename T>  Lock* Singleton<T>::_pLock = new Lock();

template <typename T>  T* Singleton<T>::_instance = NULL;

////////////////////////////////////////////////////////////////////////////////
template <typename T> Singleton<T>::~Singleton()
{
    if (_instance) delete _instance;
    _instance = NULL;

    if (_pLock) delete _pLock;
    _pLock = NULL;
}

////////////////////////////////////////////////////////////////////////////////
template <typename T> T *Singleton<T>::getInstance()
{
    if (_instance == NULL)
    {
        _pLock->lock();
        if (_instance == NULL)
        {
            _instance = new T();
        }
        _pLock->unlock();
    }

    return _instance;
}

}

#endif /* end of file */