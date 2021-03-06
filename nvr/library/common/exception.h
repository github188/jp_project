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
#ifndef __LIBRARY_EXCEPTION_H__
#define __LIBRARY_EXCEPTION_H__

#include "../../common/basetype.h"

#include <exception>
#include <stdarg.h>
#include <sstream>
#include <stdio.h>
#include <utility>
#include <string>
#include <vector>

namespace library {

/*
 * base exception defines. all other exception must be inherited it.
*/

class Exception : public std::exception
{
	public:

		/* Default Constructor */
		Exception();

		/**
		 * Copy Constructor
		 *
		 * @param ex
		 *		The <code>Exception</code> instance to copy
		 */
		Exception(const Exception &ex);

		/**
		 * Constructor - code id to indicate the exception.
		 *
		 * @param file
		 *		The file name where the exception occurs.
		 *
		 * @param linenumber
		 *		The line number where the exception occurs.
		 *
		 * @param codeid
		 *		exception indication.
		 */
		 Exception(const char * file, const int32 linenumber, int32 codeid);

		/**
		 * Constructor - Initializes the file name and line number where this
		 * message occured. Sets the message to report, using an optional list 
		 * of arguments to parse into the <code>message</code>
		 *
		 * @param file
		 *		The file name where the exception occurs.
		 *
		 * @param linenumber
		 *		The line number where the exception occurs.
		 *
		 * @param msg
		 *		The message to report.
		 *
		 * @param ...
		 *		list of primitives that are formatted into the message.
		 */
		Exception(const char *file, const int32 linenumber, const char *msg, ...);

		/* Destructor - throw no exception when instance destroyed */
		virtual ~Exception() throw();

	public:

		/**
         * Implement method from std::exception.
         *
         * @return the const char* of <code>getMessage()</code>.
         */
        virtual const char* what() const throw ();

	public:
		/**
         * Sets the cause for this exception.
         *
         * @param msg
         *      The format string for the msg.
         * @param ...
         *      The params to format into the string.
         */
		virtual void setmessage(const char *msg, ...);

		/**
         * Adds a file/line number to the stack trace.
         *
         * @param file
         *      The name of the file calling this method (use __FILE__).
         * @param lineNumber
         *      The line number in the calling file (use __LINE__).
         */
		virtual void setmark(const char *file, const int32 linenumber);

		/**
		 * Get the message string for this exception.
		 * @return string formatted error message.
		 */
		std::string getmessage()const;

		/**
		 * Provides the stack trace for every point where this exception was caught,
		 * marked, and rethrown. The first item in the returned vector is the first
		 * point where the mark was set (e.g. where the exception was created).
		 *
		 * @return the stack trace.
		 */
		std::vector<std::pair<std::string, int> >getstacktrace()const;

		/**
         * Gets the stack trace as one contiguous string.
         *
         * @return string with formatted stack trace data.
         */
		std::string getstacktracestring()const;

		/**
         * Prints the stack trace to the given output stream.
         */
		void printstackstrace(std::ostream& stream) const;

		/**
         * Assignment operator, copies one Exception to another.
         *
         * @param ex
         *      const reference to another Exception
         */
		Exception &operator=(const Exception &ex);

		/**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         *
         * @return Copy of this Exception object
         */
		virtual Exception* clone() const;

	protected:
		void setstacktrace(const std::vector<std::pair<std::string, int> > &trace);

		virtual void buildmessage(const char *format, va_list &vargs);

	private:
		std::string message;
		std::vector<std::pair<std::string, int> > stackstrace;
};

}

#endif /* end of file */