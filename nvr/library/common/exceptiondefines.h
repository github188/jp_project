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
#ifndef __LIBRARY_EXCEPTIONDEFINES_H__
#define __LIBRARY_EXCEPTIONDEFINES_H__

/**
 * Macro for catching and rethrowing an exception of
 * a given type.
 * @param type The type of the exception to throw
 * (e.g. Exception ).
 */
#define EXCEPTION_CATCH_RETHROW( type ) \
    catch( type& ex ){ \
        ex.setmark( __FILE__, __LINE__ ); \
        throw; \
    }

/**
 * A catch-all that throws a known exception.
 * @param type the type of exception to be thrown.
 */
#define EXCEPTION_CATCHALL_THROW( type ) \
    catch( ... ){ \
        type ex( __FILE__, __LINE__, \
            "caught unknown exception" ); \
        throw ex; \
    }

/**
 * A catch-all that does not throw an exception, one use would
 * be to catch any exception in a destructor and mark it, but not
 * throw so that cleanup would continue as normal.
 */
#define EXCEPTION_CATCHALL_NOTHROW( ) \
    catch( ... ){ \
        library::Exception ex( __FILE__, __LINE__, \
            "caught unknown exception, not rethrowing" ); \
    }

/**
 * Macro for catching and rethrowing an exception of
 * a given type.
 * @param type The type of the exception to throw
 * (e.g. Exception ).
 */
#define EXCEPTION_CATCH_NOTHROW( type ) \
    catch( type& ex ){ \
        ex.setmark( __FILE__, __LINE__ ); \
    }

#endif /* end of file */