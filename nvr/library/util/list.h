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
#ifndef __LIBRARY_UTIL_LIST_H__
#define __LIBRARY_UTIL_LIST_H__

#include "../../common/basetype.h"

namespace library {

template <class T> class List;
template <class T> class ListCursor;
template <class T> class OrderedList;
template <class T> class OrderedListCursor;

/**
 * list element define.
 */
template <class T> class ListItem
{
	friend class List<T>;
	friend class ListCursor<T>;
	friend class OrderedList<T>;
	
	public:

		/**
		 * return the payload of the listitem.
		 */
		T* getItem() {return item;};

	private:
		
		ListItem(ListItem *p, ListItem *n, T *e) : prev(p), next(n), item(e) {};
		
		ListItem *prev, *next;
		
		T *item;
};

/**
 * The list collection class implements a double linked template.
 */
template <class T> class List
{
	friend class ListCursor<T>;
	public:

		List() : head(NULL), tail(NULL) {};

		virtual ~List() {clearAll();};

	public:

		/**
		 * Add an element at the list's beginning.
		 *
		 * @param t
		 *    a pointer to an instance to be added.
		 * @return 
		 *    the same pointer t
		 */
   		T* addFirst(T *t)
   		{
   			head = new ListItem<T>(NULL, head, t);
			if (head->next) head->next->prev = head;
			if (tail == NULL) tail = head;

			return head->item;
   		}

		/**
		 * Add an element at the list's end.
		 *
		 * @param t
		 *    a pointer to an instance to be added.
		 * @return 
		 *    the same pointer t
		 */
		T* addLast(T *t)
		{
			if (head == NULL)
				return addFirst(t);
			else
			{
				ListItem<T> *p = tail;
				p->next = new ListItem<T>(tail, NULL, t);
				tail = p->next;

				return tail->item;
			}
		}

		/**
		 * Add an element at the list's end.
		 * @param t
		 *    a pointer to an instance to be added.
		 * @return
		 *    the same pointer t
		 */
		T* add(T *t) {return addLast(t);};

		/**
		 * Insert an element before a given element.
		 *
		 * @param newElement 'item'
		 *    the element to add.
		 * @param element 't'
		 *    a pointer to an element of the list. If the receiver does
		 *    not contain such an element, then item is appended to the 
		 *    list.
		 */
		T* insertBefore(T* item, T* t)
		{
			if ((head == NULL) || (head->item == t))
				return addFirst(item);
			else
			{
				ListItem<T> *tmp = NULL, *h = NULL;

				tmp = head;
				while ((tmp->next != NULL) && (t != tmp->next->item))
					tmp = tmp->next;
				h = tmp->next;
				if (h != NULL)
				{
					tmp->next = new ListItem<T>(tmp, h, item);
					h->prev = tmp->next;
				}
				else
				{
					tmp->next = new ListItem<T>(tmp, NULL, item);
					tail = tmp->next;
				}

				return tmp->next->item;
			}
		}

		/**
		 * Insert an element after a given element.
		 *
		 * @param newElement 'item'
		 *    the element to add.
		 * @param element 't'
		 *    a pointer to an element of the list. If the receiver does
		 *    not contain such an element, then item is appended to the 
		 *    list.
		 */
		T* insertAfter(T* item, T* t)
		{
			if ((tail == NULL) || (tail->item == t))
				return addLast(item);
			else
			{
				ListItem<T> *tmp = NULL, *h = NULL;

				tmp = tail;
				while ((tmp->prev != NULL) && (tmp->prev->item != t))
					tmp = tmp->prev;
				
				h = tmp->prev;
				if (h != NULL)
				{
					tmp->prev = new ListItem<T>(h, tmp, item);
					h->next = tmp->prev;
				}
				else
				{
					tmp->prev = new ListItem<T>(NULL, tmp, item);
					head = tmp->prev;
				}

				return tmp->prev->item;
			}
		}

		/**
		 * Remove the first element.
		 *
		 * @return 
		 *    a pointer to the removed element or NULL if the list is empty.
		 */
		T* removeFirst()
		{
			if (head != NULL)
			{
				T *retval = head->item;
				ListItem<T> *tmp = head;
				head = head->next;
				if (!head)
					tail = NULL;
				else
					head->prev = NULL;

				delete tmp;
				return retval;
			}

			return NULL;
		}

		/**
		 * Remove the last element.
		 *
		 * @return 
		 *    a pointer to the removed item or 0 if the list is empty.
		 */
		T* removeLast()
		{
			if (tail)
			{
				T *retval = tail->item;
				ListItem<T> *tmp = tail;

				tail = tail->prev;
				if (!tail)
					head = NULL;
				else
					tail->next = NULL;

				delete tmp;
				return retval;
			}

			return NULL;
		}

		/**
		 * Remove and delete all elements from the receiver.
		 */
		void clearAll()
		{
			ListItem<T> *tmp = head;
			ListItem<T> *del = NULL;

			while (tmp)
			{
				del = tmp;
				tmp = tmp->next;
				
				delete del->item;
				delete del;
				del = NULL;
			}

			tail = head = NULL;
		}

		/**
		 * Empty the receiver list without freeing the memory of the
		 * list elements.
		 */
		void clear()
		{
			ListItem<T>* tmp = head;
			ListItem<T>* del = NULL;

			while (tmp)
			{
				del = tmp;
				tmp = tmp->next;

				delete del;
				del = NULL;
			}

			head = tail = NULL;
		}

		/**
		 * Remove an element.
		 * 
		 * @param item
		 *    a pointer to a list item.
		 * @return
		 *    the pointer to the given item, or 0 if the receiver does not
		 *    contain item.
		 */
		T* remove(T *t)
		{
			ListItem<T> *tmp;

			if (!head) return NULL;
			
			tmp = head;
			do
			{
				if (tmp->item == t)
					return remove(tmp);
				
			} while ((tmp = tmp->next) != NULL);

			return 0;
		}

		/**
		 * Remove an element using a cursor.
		 * Note: The cursor is invalid after this operation!
		 * 
		 * @param item
		 *    a pointer to a list cursor.
		 * @return
		 *    the pointer to the removed item.
		 */
		T* remove(ListItem<T>* victim)
		{
			T* i = victim->item;

			if ((victim->prev) && (victim->next))
			{
				victim->prev->next = victim->next;
				victim->next->prev = victim->prev;
			}
			else
			{
				if (victim->next)
				{
					victim->next->prev = NULL;
					head = victim->next;
				}
				else
				{
					if (victim->prev)
					{
						victim->prev->next = NULL;
						tail = victim->prev;
					}
					else
					{
						head = NULL;
						tail = NULL;
					}
				}
			}

			delete victim;
			return i;
		}

		/**
		 * Return the nth element of the list.
		 *
		 * @param n
		 *    points to the elements (starting from 0) to return.
		 * @return 
		 *    a pointer to a list item, or 0 if the list does contain 
		 *   less than n elements.
		 */
		T* getNth(int32 n) const
		{
			if (!head) return NULL;

			ListItem<T> *p = head;

			while ((n > 0) && (p->next))
			{
				n--;
				p = p->next;
			}
			
			if (n != 0) return NULL;
			return p->item;
		}

		/**
		 * Return the position of a given element.
		 *
		 * @param element
		 *    a pointer to a list element.
		 * @return 
		 *    the list item holding the given element, or 0 if element
		 *    is not part of the list.
		 */
		ListItem<T>* position(T* t) const
		{
			ListItem<T> *p = head;

			while ((p) && (p->item != t))
				p = p->next;

			return p;
		}

		/**
		 * Return the index of a given element.
		 *
		 * @param element
		 *    a pointer to a list element.
		 * @return
		 *    the index (counted from 0) of element within the receiver.
		 *    If the receiver does not contain element, then -1 is returned.
		 */
		int32 index(T* t) const
		{
			ListItem<T> *p = head; 
			int32 i=0;

			for (; (p) && (p->item != t); i++)
				p = p->next;

			return (p) ? i : -1;
		}

		/**
		 * Return the first element.
		 *
		 * @return
		 *    the pointer to the first element, or 0 if the receiver is empty.
		 */
		T* first() const
		{
			return (head) ? head->item : NULL;
		}

		/**
		 * Return the last element.
		 *
		 * @return
		 *    the pointer to the last element, or 0 if the receiver is empty.
		 */
		T* last()
		{
			return (tail) ? tail->item : NULL;
		}

		/**
		 * Replace the nth element with a given one.
		 *
		 * @param n
		 *    the index of the element to replace (counted from 0).
		 * @param element
		 *    a pointer to an element.
		 * @return 
		 *    the pointer to the new element at index n, or 0 if the 
		 *    receiver contains less than n elements.
		 */
		T* overwriteNth(int n, T* t)
		{
			if (!head) return NULL;

			ListItem<T> *p = head;
			while ((n>0) && (p->next))
			{
				n--; p = p->next;
			}

			if (n == 0)
			{
				if (p->item) delete p->item;
				p->item = t;
			}

			return p->item;
		}

		/**
		 * Get the element count of the list.
		 *
		 * @return
		 *    the receiver's size.
		 */
		int32 size() const
		{
			ListItem<T> *p = head;
			int32 r = 0;

			while (p)
			{
				r++;
				p = p->next;
			}

			return r;
		}

		/**
		 * Check whether the list is empty.
		 *
		 * @return
		 *    a value != 0, if the list is empty. A value of 0, if the
		 *    list is not empty.
		 */
		int32 empty() const
		{
			return (head == NULL);
		}

	protected:

		ListItem<T> *head;
		ListItem<T> *tail;
};

////////////////////////////////////////////////////////////////////////////////
template <class T> class ListCursor
{
	public:

		ListCursor() : cursor(NULL) {};

		ListCursor(const List<T> *l) : cursor(l->head) {};

		ListCursor(const ListCursor<T> &c) : cursor(c.cursor) {};

		~ListCursor() {};

	public:

		void init(const List<T> *l) {cursor = l->head;};

		void initLast(const List<T>* l) { cursor = l->tail; }

		void init(ListItem<T>* t) { cursor = t; }

		void init(const OrderedList<T>* l) { cursor = l->content.head; }
		
		void initLast(const OrderedList<T>* l) { cursor = l->content.tail; }

	public:

		T* get()
		{
			if (cursor)
				return cursor->item;
			else
				return NULL;
		}

		int32 next()
		{
			if ((cursor) && (cursor->next))
			{
				cursor = cursor->next;

				return 1;
			}
			else
			{
				if (cursor) cursor = NULL;
			}

			return 0;
		}

		int32 prev()
		{
			if ((cursor) && (cursor->prev))
			{
				cursor = cursor->prev;

				return 1;
			}
			else
			{
				if (cursor) cursor = NULL;
			}

			return 0;
		}

		int32 isNext()
		{
			if ((cursor) && (cursor->next))
				return 1;
			else
				return 0;
		}

		int32 isPrev()
		{
			if ((cursor) && (cursor->prev))
				return 1;
			else
				return 0;
		}

		ListItem<T>* get_cursor()
		{
			return cursor;
		}

	protected:

		ListItem<T> *cursor;
};

////////////////////////////////////////////////////////////////////////////////

//
// OrderedList is implemented as a wrapper of List to avoid corrupting
// the order by using directly methods of List
// from little to big
//
template <class T> class OrderedList
{
	friend class ListCursor<T>;

	public:

		T* add(T* item)
		{
			if (!content.empty())
			{
				ListCursor<T> cur;
				for (cur.init(&content); cur.get(); cur.next())
				{
					if (*item < *cur.get())
						return content.insertBefore(item, cur.get());
				}

				return content.add(item);
			}

			return content.add(item);
		}

		T* addLast(T* item)
		{
			if (!content.empty())
			{
				ListCursor<T> cur;
				for (cur.initLast(&content); cur.get(); cur.prev())
				{
					if (*item > *cur.get())
						return content.insertAfter(item, cur.get());
				}
				
				return content.addFirst(item);
			}
			
			return content.addFirst(item);
		}

		// add a new item, but if an equal item already exists:
		// do not change the list, delete the new item and return 0 
		T* addUnique(T* item)
		{
			if (!content.empty())
			{
				ListCursor<T> cur;
				for (cur.init(&content); cur.get(); cur.next())
				{
					if (*item == *cur.get())
					{
						delete item;
						return 0;
					}
					
					if (*item < *cur.get()) 
						return content.insertBefore(item, cur.get());
				}
				
				return content.add(item);
			}
			
			return content.add(item);
		}

		T* remove(T* item) { return content.remove(item); }
		
		T* remove(ListItem<T>* item) { return content.remove(item); }

		T* overwriteNth(int n, T* t)
		{
			return content.overwriteNth(n, t);
		}

		int32 size() const  {return content.size();}
		
		T* first() const {return content.first();}
		
		T* last() const  {return content.last();}
		
		T* getNth(int32 i) const  {return content.getNth(i);}

		ListItem<T>* position(T *t) const {return content.position(t);}
		
		int32 index(T *t) const {return content.index(t);}

		int	empty() const {return content.empty();}

		void clearAll()	{content.clearAll();}

		void clear() {content.clear();}

	protected:

		List<T>	content;
};

////////////////////////////////////////////////////////////////////////////////
template <class T> class OrderedListCursor
{
	public:
		
		void init(const OrderedList<T>* l) {cursor.init(l);};
		
		void initLast(const OrderedList<T>* l) {cursor.initLast(l);};
		
		void init(ListItem<T>* t) {cursor.init(t);};
		
		T* get() { return cursor.get(); };
		
		int	next() { return cursor.next(); };
		
		int	isNext() { return cursor.isNext(); };
		
		int	prev() { return cursor.prev(); };
		
		int	isPrev() { return cursor.isPrev(); };
		
		ListItem<T>* get_cursor() { return cursor.get_cursor(); }

	public:
		
		OrderedListCursor() {};
		
		OrderedListCursor(const OrderedList<T>* l) : cursor((const List<T>*)l) {}
		
		OrderedListCursor(const OrderedListCursor<T>& c): cursor(c.cursor) {}
		
	protected:
		
		ListCursor<T> cursor;
};

}

#endif /* end of file */