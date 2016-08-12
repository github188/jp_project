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
#ifndef __LIBRARY_UTIL_ARRAY_H__
#define __LIBRARY_UTIL_ARRAY_H__

#include "indexoutofboundexception.h"
#include "../../common/basetype.h"
#include "system.h"

namespace library {

template <class T> class Array
{
	public:

		Array() : elements(NULL), capacity(0), head(0), cursize(0)
		{
			this->ensureCapacity(10);
		};

		Array(int32 initCapacity) : elements(NULL), capacity(initCapacity), head(0), cursize(0)
		{
			this->elements = new T[this->capacity];
		};

		Array(const Array<T> &source) : elements(NULL), capacity(0), head(0), cursize(0)
		{
			this->capacity = source.size() + (source.size()/10);
			this->elements = new T[this->capacity];

			for (int32 i = 0; i < source.size(); ++i)
			{
				this->elements[i] = source.elements[i];
				this->cursize++;
			}
		};

		virtual ~Array()
		{
			delete [] elements;
		}

	public:

		Array<T>& operator= (const Array<T> &right)
		{
			this->clear();
			this->addAll(right);

			return *this;
		}

	public:

		/**
         * Increases the capacity of this ArrayList instance, if necessary, to ensure that it can
         * hold at least the number of elements specified by the minimum capacity argument.  If the
         * capacity is already greater than or equal to the minimum capacity argument then the array
         * is left unchanged.
         *
         * @param minimumCapacity
         *      The desired minimum capacity for this ArrayList.
         */
		void ensureCapacity(int32 minimumCapacity)
		{
			if( minimumCapacity < 0 || this->capacity >= minimumCapacity ) {
                return;
            }

			int newCapacity = minimumCapacity == 0 ? 10 : minimumCapacity;

			T* newElements = new T[newCapacity];
           	for (int32 i = 0; i < this->cursize; ++i)
           	{
           		newElements[i] = this->elements[i];
           	}
			
            delete [] this->elements;
            this->elements = newElements;
            this->capacity = newCapacity;
		}

		/**
         * Trims the internal array to match the current size of the ArrayList.  This compacts
         * the internal array to save storage space used by this ArrayList.
         */
        void trimToSize()
		{
			if (this->cursize < this->capacity)
			{
				int32 newCapacity = this->cursize == 0 ? 10 : this->cursize;

				T newElements = new T[newCapacity];
				for (int32 i = 0; i < this->cursize; ++i)
	           	{
	           		newElements[i] = this->elements[i];
	           	}

				delete [] this->elements;
                this->elements = newElements;
                this->capacity = newCapacity;
			}
		}

		void clear()
		{
			delete [] this->elements;
            this->curSize = 0;
            this->capacity = 10;
            this->elements = new T[this->capacity];
		}

		int32 empty() const
		{
			return this->cursize == 0;
		}

		int32 size() const
		{
			return this->cursize;
		}

		T set(int32 index, const T& e)
		{
			if (index < 0 || index > this->cursize)
			{
				throw IndexOutOfBoundException(__FILE__, __LINE__, "array::set index out of bound.");
			}

			T oldValue = this->elements[index];
			this->elements[index] = e;

			return oldValue;
		}

		T get(int32 index) const
		{
			if (index < 0 || index > this->cursize)
			{
				throw IndexOutOfBoundException(__FILE__, __LINE__, "array::get index out of bound.");
			}

			return this->elements[index];
		}

		int32 add(const T& e)
		{
			this->expandEnd(1);
			this->elements[this->cursize++] = e;

			return 1;
		}

		void add(int32 index, const T& e)
		{
			if (index < 0 || index > this->cursize)
			{
				throw IndexOutOfBoundException(__FILE__, __LINE__, "array::add index out of bound.");
			}

			if (index == 0)
				this->expandFront(1);
			else if (index == this->cursize)
				this->expandEnd(1);
			else
				this->expandMiddle(index, 1);

			this->elements[index] = e;
            this->curSize++;
		}

		int32 addAll(const Array<T>& source)
		{
			int32 size = source.size();

			if (size == 0) return 0;

			this->expandEnd(size);
			std::vector<T> array = source.toArray();
			for (int32 i = 0; i < size; ++i)
			{
				this->elements[this->cursize++] = array[i];
			}

			return 1;
		}

		int32 addAll(int32 index, const Array<T>& source)
		{
			if (index < 0 || index > this->cursize)
			{
				throw IndexOutOfBoundException(__FILE__, __LINE__, "array::addAll index out of bound.");
			}

			int32 size = source.size();
			if (size == 0) return 0;

			std::vector<T> array = source.toArray();

			if (index == 0)
				this->expandFront(size);
			else if (index == this->cursize)
				this->expandEnd(size);
			else
				this->expandMiddle(index, size);

			for( int i = 0; i < size; ++i, ++this->curSize )
			{
                this->elements[index++] = array[i];
            }

			return 1;
		}

		int32 remove(const T& e)
		{
			int32 result = index(e);
			if (result != -1)
			{
				this->remove(result);

				return 1;
			}

			return 0;
		}

		T remove(int32 ind)
		{
			if (ind < 0 || ind > this->cursize)
			{
				throw IndexOutOfBoundException(__FILE__, __LINE__, "array::remove index out of bound.");
			}

			T old = this->elements[ind];
			System::arraycopy(this->elements, 0, this->elements, 0, ind);

			if (this->cursize > ind)
			{
				System::arraycopy(this->elements, ind + 1, this->elements, ind, this->cursize - ind - 1);
			}

			/* fill a new one? */
			this->elements[--this->cursize] = T();

			return old;
		}

		int32 contains(const T& e) const
		{
			return this->index(e) != -1;
		}

		/* get the index of the node equal to the element */
		int32 index(const T& e) const
		{
			for (int32 i = 0; i < this->cursize; ++i)
			{
				if (this->elements[i] == e)
					return i;
			}

			return -1;
		}

		int32 indexLast(const T& e) const
		{
			for (int32 i = this->cursize - 1; i >= 0; --i)
			{
				if (this->elements[i] == e) return i;
			}

			return -1;
		}

		std::vector<T> toArray() const
		{
			std::vector<T> result;

			for (int32 i = 0; i < this->cursize; ++i)
			{
				result.push_back(this->elements[i]);
			}

			return result;
		}

	private:

		void expandFront(int32 amount)
		{
			if (amount == 0) return;

			T *previous = this->elements;
			if (amount > this->capacity - this->cursize)
			{
				this->capacity = this->capacity + amount + 11;
				this->elements = new T[this->capacity];
			}

			if (this->cursize > 0)
			{
				System::arraycopy(previous, 0, this->elements, amount, this->cursize );
			}

			if (previous != this->elements)
			{
				delete [] previous;
			}
		}

		void expandEnd(int32 amount)
		{
			if (amount == 0) return;

			T *previous = this->elements;

			if (amount > this->capacity - this->cursize)
			{
				this->capacity = this->capacity + amount + 11;
				this->elements = new T[this->capacity];
				
				System::arraycopy(previous, 0, this->elements, 0, this->cursize);
			}

			if (previous != this->elements)
			{
				delete [] previous;
			}
		}

		void expandMiddle(int32 index, int32 amount)
		{
			if (amount == 0) return;

			T *previous = this->elements;
			if (amount > this->capacity - this->cursize)
			{
				this->capacity = this->capacity + amount + 11;
				this->elements = new T[this->capacity];
			}

			if (this->cursize > 0)
			{
				System::arraycopy(previous, 0, this->elements, 0, index);
			}

			if (index < this->cursize)
			{
				System::arraycopy(previous, index, this->elements, index + amount, this->cursize - index);
			}

			if (previous != this->elements)
				delete [] previous;
		}
		
	private:

		T *elements;

		int32 capacity;

		int32 head;

		int32 cursize;
};

////////////////////////////////////////////////////////////////////////////////
template <class T> class ArrayCursor
{
	public:

		void init(const Array<T> *l)
		{
			cursor = 0;
			_list = l;
		}

		void initLast(const Array<T> *l)
		{
			cursor = l->size() - 1;
			_list = l;
		}

		T get()
		{
			if ((cursor < 0) || (cursor >= _list->size()))
			{
				throw IndexOutOfBoundException(__FILE__, __LINE__, "ArrayCursor::get out of bound.");
			}

			return _list->get(cursor);
		}

		int32 next()
		{
			if (++cursor >= _list->size())
				return 0;

			return 1;
		}

		int32 prev()
		{
			if (--cursor < 0) return 0;
			return 1;
		}

		int32 isNext()
		{
			if (cursor + 1 >= _list->size()) return 0;
			return 1;
		}

		int32 isPrev()
		{
			if (cursor -1 < 0) return 0;
			return 1;
		}

		int32 getCursor()
		{
			return cursor;
		}

		ArrayCursor() : cursor(0), _list(NULL) {};

		ArrayCursor(const Array<T> *l) : cursor(0), _list(l) {};

		ArrayCursor(const ArrayCursor<T> &c) : cursor(c.cursor), _list(c._list) {};

	protected:

		const Array<T> *_list;
		int32 cursor;
};

}

#endif /* end of file */