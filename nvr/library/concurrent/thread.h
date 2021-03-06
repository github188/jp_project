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
#ifndef __LIBRARY_CONCURRENT_THREAD_H__
#define __LIBRARY_CONCURRENT_THREAD_H__

#include <string>

#include "../../common/basetype.h"
#include "../common/runnable.h"

namespace library {

class ThreadProperties;
class Thread : public Runnable
{
	private:
		
		Thread(const Thread&);
		
        Thread& operator=(const Thread&);
		
	public:

		/** The minimum priority that a thread can have. */
        static const int MIN_PRIORITY = 1;

        /** The default priority that a thread is given at create time. */
        static const int NORM_PRIORITY = 5;

        /** The maximum priority that a thread can have. */
        static const int MAX_PRIORITY = 10;

		enum State {

			/** Before a Thread is started or a Thread has been joined it exists in this State. */
			IDLE = 0,

			/** While a Thread is running it is in this State. */
			RUNNING = 1,

			/** A Thread whose run method has exited is in this state. */
			FINISHED = 2
		};

	public:

		/**
         * Constructs a new Thread. This constructor has the same effect as
         * Thread( NULL, NULL, GIVEN_NAME ), where GIVEN_NAME is a newly generated name.
         * When no name is given the name is automatically generated and are of the form
         * "Thread-"+n, where n is an integer.
         */
		Thread();

		/**
         * Constructs a new Thread with the given target Runnable task. This constructor
         * has the same effect as Thread( NULL, task, GIVEN_NAME ), where GIVEN_NAME is a
         * newly generated name.  When no name is given the name is automatically generated
         * and are of the form  "Thread-"+n, where n is an integer.
         *
         * @param task the Runnable that this thread manages, if the task is NULL the Thread's
         *        run method is used instead.
         */
		Thread(Runnable *task);

		/**
         * Constructs a new Thread with the given name. This constructor has the same effect
         * as Thread( NULL, NULL, GIVEN_NAME ), where GIVEN_NAME is a newly generated name.
         * When no name is given the name is automatically generated and are of the form
         * "Thread-"+n, where n is an integer.
         *
         * @param name the name to assign to this Thread.
         */
        Thread(const std::string& name);

		/**
         * Constructs a new Thread with the given target Runnable task and name. This constructor
         * has the same effect as Thread( NULL, task, GIVEN_NAME ), where GIVEN_NAME is a
         * newly generated name.  When no name is given the name is automatically generated
         * and are of the form  "Thread-"+n, where n is an integer.
         *
         * @param task the Runnable that this thread manages, if the task is NULL the Thread's
         *        run method is used instead.
         * @param name the name to assign to this Thread.
         */
        Thread(Runnable* task, const std::string& name);

		/**
         * Constructs a new Thread with the given target Runnable task and name. This constructor
         * has the same effect as Thread( NULL, task, GIVEN_NAME ), where GIVEN_NAME is a
         * newly generated name.  When no name is given the name is automatically generated
         * and are of the form  "Thread-"+n, where n is an integer.
         *
         * The stack size option is platform independent and may have no effect on the newly
         * created thread on some systems.  If the value given is invalid on the system a
         * RuntimeException is thrown, the stack size can be invalid if it is outside the
         * allowed range or doesn't match the size of the system page size on some system.
         *
         * @param task
         *      The Runnable that this thread manages, if the task is NULL the Thread's
         *      run method is used instead.
         * @param name
         *      The name to assign to this Thread.
         * @param stackSize
         *      The size of the newly allocated thread's stack.
         */
        Thread(Runnable* task, const std::string& name, long long stackSize);

		~Thread();

	public:

		/**
         * Creates a system thread and starts it in a joinable mode.  Upon creation, the
         * run() method of either this object or the provided Runnable object will be
         * invoked in the context of this thread.
         *
         * @throws IllegalThreadStateException if the thread has already been started.
         * @throws RuntimeException if the Thread cannot be created for some reason.
         */
        void start();

		/**
		 * Waits for this thread to die.
		 */
        void join();

		/**
         * Obtains the Thread Id of the current thread, this value is OS specific but is
         * guaranteed not to change for the lifetime of this thread.
         *
         * @return Thread Id of this Thread instance.
         */
        uint64 getid() const;

		/**
         * Returns the Thread's assigned name.
         * @returns the Name of the Thread.
         */
        std::string getname() const;

		/**
         * Sets the name of the Thread to the new Name given by the argument <em>name</em>
         *
         * @paran name the new name of the Thread.
         */
        void setname(const std::string& name);

		/**
         * Gets the currently set priority for this Thread.
         *
         * @return an int value representing the Thread's current priority.
         */
        int getpriority() const;

        /**
         * Sets the current Thread's priority to the newly specified value.  The given value
         * must be within the range Thread::MIN_PRIORITY and Thread::MAX_PRIORITY.
         *
         * @param value the new priority value to assign to this Thread.
         *
         * @throws IllegalArgumentException if the value is out of range.
         */
        void setpriority(int value);

		/**
         * Returns a string that describes the Thread.
         *
         * @return string describing the Thread.
         */
        std::string tostring() const;

		/**
         * Returns true if the Thread is alive, meaning it has been started and has not yet
         * died.
         *
         * @return true if the thread is alive.
         */
        bool isalive() const;

		/**
         * Returns the currently set State of this Thread.
         *
         * @return the Thread's current state.
         */
        Thread::State getstate() const;

	public:

		virtual void run();

	public:

		/**
		 * Causes the currently executing thread to sleep (temporarily 
		 * cease execution) for the specified number of milliseconds.
		 *
		 * @param millis
		 *    number of milliseconds to sleep.
		 */
		static  void      sleep(long millis);

		/**
		 * Causes the currently executing thread to sleep (cease 
		 * execution) for the specified number of milliseconds plus 
		 * the specified number of nanoseconds.
		 *
		 * @param millis
		 *    the length of time to sleep in milliseconds.
		 * @param nanos
		 *    0-999999 additional nanoseconds to sleep.
		 */
		static  void      sleep(long millis, int nanos);

		/**
		 * Clone this thread. This method must not be called on
		 * running threads.
		 */
		Thread* clone();

	private:

		void initializeself(Runnable* task, const std::string& name, uint64 stacksize);
		
	private:

		friend void* threadstarter(void*);
		friend class ThreadProperties;
		
		/**
         * The internal data necessary to manage a Thread instance.
         */
        ThreadProperties* properties;
};

}

#endif /* end of file */
