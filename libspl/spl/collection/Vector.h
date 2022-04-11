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
/** @file */ 
#ifndef _tvector_h
#define _tvector_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/collection/Array.h>
#include <spl/Exception.h>
#include <spl/collection/IEnumerable.h>
#include <spl/IIterator.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

/** @brief A dynamically sized vector template.
  *
  * Both native and class types (with copy constructors) can be used. 
  */
template<typename T>
class Vector : public IEnumerable<T>
{
protected:
	T *m_data;
	int m_pos;		/**< Next write position */
	int m_size;		/**< Number of elements in m_data */

public:
	class Iterator : public IIterator<T>
	{
	private:
		const Vector<T> *m_ap;
		int m_pos;

	public:
		Iterator(const Vector<T> *ap)
		: m_ap(ap), m_pos(-1)
		{
		}

		Iterator(const Iterator& vi)
		: m_ap(vi.m_ap), m_pos(vi.m_pos)
		{
		}

		virtual bool Next()
		{
			if (++m_pos >= m_ap->Count())
			{
				return false;
			}
			return true;
		}

		virtual bool Prev()
		{
			if (--m_pos < 0)
			{
				return false;
			}
			return true;
		}

		virtual T Current()
		{
			return m_ap->ElementAt(m_pos);
		}

		virtual T& CurrentRef()
		{
			return m_ap->ElementAtRef(m_pos);
		}	
	};

	/** @brief constructor.
	  *
	  */
	Vector(int count = 20) 
	: m_pos(0), m_size(count)
	{
		if ( NULL == (m_data = new T[m_size]) )
		{
			throw OutOfMemoryException();
		}
		//memset(m_data, 0, m_size * sizeof(T));
#ifdef DEBUG
		// The debug memory checks need this zero'ed for native types of T
		for ( int x = 0; x < m_size; x++ )
		{
			m_data[x] = T();
		}
#endif
	}

	Vector(const Vector& v) : m_pos(v.m_pos), m_size(v.m_size)
	{
		m_data = new T[m_size];

		for ( int x = 0; x < m_size; x++ )
		{
			m_data[x] = v.m_data[x];
		}
	}

	virtual ~Vector()
	{
		ASSERT_MEM( m_data, sizeof(T) * m_size );
		delete[] m_data;
	}

	Vector& operator =(const Vector& v)
	{
		Vector<T>::ValidateMem();

		delete[] m_data;

		m_pos = v.m_pos;
		m_size = v.m_size;
		m_data = new T[m_size];

		for ( int x = 0; x < m_size; x++ )
		{
			m_data[x] = v.m_data[x];
		}

		return *this;
	}

	virtual RefCountPtr<IIterator<T> > _Begin()
	{
		return RefCountPtr<IIterator<T> >(new Iterator(this));
	}

	inline Iterator Begin() const
	{
		return Iterator(this);
	}

	/** @brief Add an item
	  *
	  * @param item[in] of type T
	  * @exception OutOfMemoryException
	  */
	inline void Add( T item )
	{
		if ( m_pos == m_size )
		{
			Extend(m_size*2);
		}
		m_data[m_pos++] = item;
	}

	void AddRange(const Array<T>& a, const int offset, const int len)
	{
		for ( int x = 0; x + offset < a.Length() && x < len; x++ )
		{
			Add(a[x+offset]);
		}
	}

	inline void AddRange(const Array<T>& a, const int len)
	{
		AddRange(a, 0, len);
	}

	inline void AddRange(const Array<T>& a)
	{
		AddRange(a, 0, a.Length());
	}

	void AddRange(const Vector<T>& v)
	{
		for (int x = 0; x < v.Count(); x++)
		{
			Add(v.ElementAt(x));
		}
	}

	void AddRangeWithPad(const Array<T>& a, int count)
	{
		for (int x = 0; x < count; x++)
		{
			if (x < a.Length())
			{
				Add(a[x]);
			}
			else
			{
				Add(T());
			}
		}
	}

	void AddRangeWithLeadingPad(Array<T>& a, int count)
	{
		int diff = count - a.Length();
		ASSERT(diff >= 0);

		for (int x = 0; x < count; x++)
		{
			if (x >= diff)
			{
				Add(a[x - diff]);
			}
			else
			{
				Add(T());
			}
		}
	}

	/** @brief Set an element of the array.
	  * 
	  * If necessary, the vector is automatically extended.
	  * @param item[in] of type T.
	  * @param pos[in] The index to set.
	  * @exception OutOfMemoryException
	  */
	void SetElementAt( T item, int pos )
	{
		if ( pos >= m_size )
		{
			ExtendTo(pos+1);
		}
		if ( pos >= m_pos )
		{
			m_pos = pos + 1;
		}
		m_data[pos] = item;
	}

	/** @brief Insert an item into the array.
	  *
	  * If necessary, the vector is automatically extended.  All existing items are preserved.
	  * @param item[in] of type T.
	  * @param pos[in] Index in the vector.
	  * @exception OutOfMemoryException
	  */
	void Insert( T item, int pos )
	{
		if ( m_pos+1 >= m_size )
		{
			ExtendTo(m_size+10);
		}
		memcpy(&m_data[pos+1], &m_data[pos], (m_pos - pos)*sizeof(T));
		m_data[pos] = item;
		m_pos++;
		ASSERT_PTR( &m_data[m_pos] );
		ASSERT_MEM( m_data, sizeof(T) * m_size );
	}

	/** @brief Get an element
	  *
	  * @param pos[in] Index in the vector.
	  * @return T
	  */
	inline T ElementAt( const int pos ) const
	{
		ASSERT( pos >= 0 );
		ASSERT( pos < m_pos );
		ASSERT_MEM( m_data, sizeof(T) * m_size );
		if ( pos >= m_pos || 0 > pos )
		{
			throw IndexOutOfBoundsException();
		}
		return m_data[pos];
	}

	/** @brief Get a reference to an element
	  *
	  * @param pos[in] Index in the vector.
	  * @return T
	  */
	inline T& ElementAtRef( const int pos ) const
	{
		ASSERT( pos >= 0 );
		ASSERT( pos < m_pos );
		ASSERT_MEM( m_data, sizeof(T) * m_size );
		if ( pos >= m_pos || 0 > pos )
		{
			throw IndexOutOfBoundsException();
		}
		return m_data[pos];
	}

	/** @brief Remove the element at the index
	  *
	  * No error if pos is greater than the vector length.
	  * @param pos[in] Vector index.
	  */
	T RemoveAt( const int pos )
	{
		if ( pos >= m_pos )
		{
			return T();
		}
		T element = ElementAt(pos);
		ASSERT_MEM( m_data, sizeof(T) * m_size );
		ASSERT_PTR( &m_data[pos] );
		memcpy(&m_data[pos], &m_data[pos+1], (m_pos - pos)*sizeof(T));
		m_pos--;
		ASSERT_PTR( &m_data[m_pos] );
		ASSERT_MEM( m_data, sizeof(T) * m_size );
		return element;
	}

	inline T& operator[] (const int idx) const
	{
		return ElementAtRef(idx);
	}

	/** @brief Remove the specified item.
	  *
	  * Search for item and remove it.  No error if item is not in the vector.
	  * @param item[in] The item to remove.
	  */
	void Remove( const T& item )
	{
		int count = Count();
		for ( int x = 0; x < count; x++ )
		{
			if ( item ==  ElementAt(x) )
			{
				RemoveAt( x );
				return;
			}
		}
	}

	/** @brief Removes that last (highest index) element.
	  *
	  */
	inline T Pop(  )
	{
		ASSERT( m_pos > 0 );
		if (m_pos == 0)
		{
			return T();
		}
		return m_data[--m_pos];
	}

	/** @brief Returns that last (highest index) element.
	  *
	  */
	inline T Peek(  ) const
	{
		ASSERT( m_pos > 0 );
		if (m_pos == 0)
		{
			throw Exception("Stack empty");
		}
		return m_data[m_pos-1];
	}

	/** @brief Returns that last (highest index) element.
	  *
	  */
	inline T& PeekRef(  ) const
	{
		ASSERT( m_pos > 0 );
		if (m_pos == 0)
		{
			throw Exception("Stack empty");
		}
		return m_data[m_pos-1];
	}

	/** @brief Resets the vector count to zero.
	  *
	  * Does not deallocate or clear any vector elements.
	  */
	inline void Clear()
	{
		m_pos = 0;
	}

	/** @brief Number of elements in the vector.
	  *
	  */
	inline int Count() const
	{
		return m_pos;
	}

	/** @brief Shallow copy of the vector.
	  *
	  * @exception OutOfMemoryException
	  */
	Vector<T> *Copy() const
	{
		Vector<T> *vec = new Vector<T>();
		int count = Count();
		for ( int x = 0; x < count; x++ )
		{
			vec->Add( ElementAt(x) );
		}
		return vec;
	}

	/** @brief Internal data pointer.
	  *
	  */
	inline T *Data() { return m_data; }

	/** @brief Ensure the vector has a minimum number of elements.
	  *
	  * @param size[in] The minimum number of elements.
	  */
	void EnsureElementTo(int size)
	{
		if ( size <= Count() )
		{
			return;
		}
		ExtendTo(size);
	}

	void SetCount(int cnt)
	{
		ASSERT( cnt < m_size );
		m_pos = cnt;
	}

	inline RefCountPtr<Array<T> > ToArray() const
	{
		return ToArray(Count());
	}

	inline RefCountPtr<Array<T> > ToArray(int count) const
	{
		return RefCountPtr<Array<T> >(new Array<T>(m_data, count));
	}

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const
	{
		DEBUG_NOTE_MEM_ALLOCATION( m_data );
		for ( int x = 0; x < m_size; x++ )
		{
			TypeCheckMem( m_data[x] );
		}
	}

	void ValidateMem() const
	{
		ASSERT_MEM(m_data, sizeof(T) * m_size);
	}
#endif

protected:
	void ExtendTo(int index)
	{
		ASSERT_MEM(m_data, sizeof(T) * m_size);

		if (index > m_size)
		{
			Extend(index+1);
			ASSERT_MEM(m_data, sizeof(T) * m_size);
		}
		ASSERT_MEM(m_data, sizeof(T) * m_size);
	}

	void Extend(int newlen)
	{
		ASSERT_MEM(m_data, sizeof(T) * m_size);

		int x;
		T *newbuf;

		if ((newbuf = new T[newlen]) == NULL)
		{
			throw OutOfMemoryException();
		}
		for ( x = 0; x < m_size; x++ )
		{
			newbuf[x] = m_data[x];
		}
		//memset(&newbuf[m_size], 0, (newlen - m_size) * sizeof(T));
#ifdef DEBUG
		// The debug memory checks need this zero'ed for native types of T
		for ( x = m_size; x < newlen; x++ )
		{
			newbuf[x] = T();
		}
#endif
		m_size = newlen;
		delete[] m_data;
		m_data = newbuf;
		ASSERT_MEM(m_data, sizeof(T) * m_size);
	}
};

/** @} */
}
#endif
