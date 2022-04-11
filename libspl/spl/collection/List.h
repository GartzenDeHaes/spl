/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SPL is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SPL.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _tlist_h
#define _tlist_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/collection/Array.h>
#include <spl/Compare.h>
#include <spl/Exception.h>
#include <spl/collection/IEnumerable.h>
#include <spl/IIterator.h>
#include <spl/math/Math.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */
 
template<typename T>
class _listnode
{
public:
	_listnode *next;
	_listnode *prev;
	T data;
};

/** @brief A doublely linked list. */
template<typename T, typename LISTNODE = _listnode<T> >
class List : public IEnumerable<T>
{
private:
	inline void _Init()
	{
		m_head.data = T();
		m_head.next = &m_tail;
		m_head.prev = NULL;
		m_tail.data = T();
		m_tail.next = NULL;
		m_tail.prev = &m_head;
		m_count = 0;
	}

protected:
	LISTNODE m_head;
	LISTNODE m_tail;
	int m_count;

public:
	/** @brief Iterate the nodes of the list. */
	class Iterator : public IIterator<T>
	{
		friend class List<T>;

	private:
		LISTNODE* m_current;

	public:
		Iterator()
		: m_current(NULL)
		{
		}

		Iterator(LISTNODE* head)
		: m_current(head)
		{
		}

		Iterator(const Iterator& li)
		: m_current(li.m_current)
		{
		}

		virtual bool Next( )
		{
			if (NULL == m_current || NULL == m_current->next)
			{
				// m_current is the tail node
				return false;
			}
			m_current = (LISTNODE*)m_current->next;
			if (NULL == m_current->next)
			{
				// m_current is the tail node
				return false;
			}

			ASSERT_MEM( m_current, sizeof( LISTNODE ) );
			return true;
		}

		virtual bool Prev( )
		{
			if (NULL == m_current || NULL == m_current->prev)
			{
				// Head
				return false;
			}
			m_current = (LISTNODE*)m_current->prev;
			if (NULL == m_current->prev)
			{
				// Head
				return false;
			}
			return true;
		}

		virtual T Current()
		{
			ASSERT_MEM( m_current, sizeof( LISTNODE ) );
			return m_current->data;
		}

		virtual T& CurrentRef()
		{
			ASSERT_MEM( m_current, sizeof( LISTNODE ) );
			return m_current->data;
		}
	};
	
	List()
	{
		_Init();
	}

	List( const List<T>& list )
	{
		_Init();

		LISTNODE* node = list.m_head.next;
		while ( node != &list.m_tail )
		{
			ASSERT_MEM( node, sizeof( LISTNODE ) );
			Add( node->data );
			node = node->next;
		}
	}

	virtual ~List()
	{
		Clear();
	}

	List<T>& operator =( const List<T>& list )
	{
		Clear();

		LISTNODE* node = list.m_head.next;
		while ( node != &list.m_tail )
		{
			Add( node->data );
			node = node->next;
		}
		return *this;
	}

	bool IsEmpty(  ) const
	{
		return m_head.next == &m_tail;
	}

	void Clear(  )
	{
		LISTNODE* np;
		LISTNODE* npnext;

		np = (LISTNODE*)m_head.next;

		while ( np != &m_tail )
		{
			ASSERT_MEM( np, sizeof( LISTNODE ) );
			npnext = (LISTNODE*)np->next;
			delete np;
			np = npnext;
		}
		m_head.next = &m_tail;
		m_count = 0;
		ASSERT( IsEmpty(  ) );
	}

	void Add( T data )
	{
		LISTNODE *np = new LISTNODE();
		np->data = data;
		np->next = m_head.next;
		np->prev = &m_head;
		m_head.next->prev = np;
		m_head.next = np;
		m_count++;
	}

	inline Iterator Begin()
	{
		return Iterator(&m_head);
	}

	T& ElementAt( const int index ) const
	{
		int count = 0;
		LISTNODE* np = (LISTNODE*)m_head.next;
		
		while ( np != &m_tail && count < index )
		{
			np = (LISTNODE*)np->next;
			count++;
		}
		if ( np == &m_tail )
		{
			throw IndexOutOfBoundsException();
		}
		return np->data;
	}

	inline T& operator[] (const int idx) const
	{
		return ElementAt(idx);
	}

	void RemoveCurrent( Iterator& iter )
	{
		LISTNODE* newcur = (LISTNODE*)iter.m_current->prev;
		ASSERT( iter.m_current != &m_head && iter.m_current != &m_tail && iter.m_current != NULL );
		iter.m_current->prev->next = iter.m_current->next;
		iter.m_current->next->prev = iter.m_current->prev;
		ASSERT_MEM( iter.m_current, sizeof( LISTNODE ) );
		delete iter.m_current;
		iter.m_current = newcur;
		m_count--;
	}

	T Pop()
	{
		LISTNODE* cur = (LISTNODE*)m_head.next;
		ASSERT( cur != &m_tail );
		m_head.next = cur->next;
		cur->next->prev = &m_head;
		T data = cur->data;
		m_count--;
		delete cur;
		return data;
	}

	T Peek() const
	{
		LISTNODE* cur = (LISTNODE*)m_head.next;
		if( cur == &m_tail )
		{
			return T();
		}
		return cur->data;
	}

	T& PeekRef() const
	{
		LISTNODE* cur = (LISTNODE*)m_head.next;
		if( cur == &m_tail )
		{
			throw new IndexOutOfBoundsException("List is empty.");
		}
		return cur->data;
	}

	T Tail() const
	{
		LISTNODE* cur = (LISTNODE*)m_tail.prev;
		if( cur == &m_head )
		{
			return T();
		}
		return cur->data;
	}

	T& TailRef() const
	{
		LISTNODE* cur = (LISTNODE*)m_tail.prev;
		if( cur == &m_head )
		{
			return T();
		}
		return cur->data;
	}

	void RemoveTail(  )
	{
		LISTNODE* cur = (LISTNODE*)m_tail.prev;
		ASSERT( cur != &m_head && cur != &m_tail && cur != NULL );
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		ASSERT_MEM( cur, sizeof( LISTNODE ) );
		delete cur;
		m_count--;
	}

	void Remove( const T& data )
	{
		int count = 0;
		LISTNODE* np = (LISTNODE*)m_head.next;
		
		while ( np != &m_tail )
		{
			if ( Compare::Equals(data, np->data) )
			{
				np->prev->next = np->next;
				np->next->prev = np->prev;
				ASSERT_MEM( np, sizeof( LISTNODE ) );
				delete np;
				m_count--;
				return;
			}
			np = (LISTNODE*)np->next;
			count++;
		}
	}

	inline int Count(  ) const
	{
		return m_count;
	}

	RefCountPtr<Array<T> > ToArray() const
	{
		RefCountPtr<Array<T> > ap(new Array<T>(m_count));

		int pos = 0;
		LISTNODE* np = (LISTNODE *)m_head.next;
		while ( np != &m_tail )
		{
			ap->SetElementAt(pos++, np->data);
		}
		return ap;
	}

#ifdef DEBUG
	void ValidateMem() const
	{
		LISTNODE* np = (LISTNODE *)m_head.next;
		while ( np != &m_tail )
		{
			ASSERT_MEM( np, sizeof(LISTNODE) );
			TypeValidate( np->data );
			np = (LISTNODE *)np->next;
		}
		TypeValidate(m_head.data);
		TypeValidate(m_tail.data);
	}

	void CheckMem() const
	{
		LISTNODE* np = (LISTNODE *)m_head.next;
		while ( np != &m_tail )
		{
			DEBUG_NOTE_MEM_ALLOCATION( np );
			TypeCheckMem( np->data );
			np = (LISTNODE *)np->next;
		}
		TypeCheckMem(m_head.data);
		TypeCheckMem(m_tail.data);
	}
#endif

	virtual RefCountPtr<IIterator<T> > _Begin()
	{
		return RefCountPtr<IIterator<T> >(new Iterator(&m_head));
	}
};

/** @} */
}
#endif
