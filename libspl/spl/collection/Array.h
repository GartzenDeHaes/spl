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
#ifndef _array_h
#define _array_h

#include <spl/types.h>
#include <spl/Debug.h>
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

/**	@brief Fixed length array with DEBUG build bounds checking.
 *	@todo There are probably places in the library that should use this instead of raw pointers.
 */
template<typename T>
class Array : public IEnumerable<T>
{
private:
	T *m_data;
	int m_len;

public:
	class Iterator : public IIterator<T>
	{
	private:
		const Array<T> *m_ap;
		int m_pos;

	public:
		Iterator()
		: m_ap(NULL), m_pos(-1)
		{
		}

		Iterator(const Array<T> *ap)
		: m_ap(ap), m_pos(-1)
		{
		}

		Iterator(const Iterator& ai)
		: m_ap(ai.m_ap), m_pos(ai.m_pos)
		{
		}

		virtual bool Next()
		{
			if (NULL == m_ap || ++m_pos >= m_ap->Count())
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

public:
	/** @brief constructor.
	  *
	  */
	Array(int count) 
	: m_len(count)
	{
		ASSERT(m_len > -1);
		if (0 == m_len)
		{
			m_data = NULL;
			return;
		}
		m_data = new T[m_len];
		Clear();
	}

	Array()
	: m_len(0), m_data(NULL)
	{
	}

	Array(const Array& v) 
	: m_len(v.m_len)
	{
		ASSERT(m_len > -1);
		if (0 == m_len)
		{
			m_data = NULL;
			return;
		}

		m_data = new T[m_len];

		for ( int x = 0; x < m_len; x++ )
		{
			m_data[x] = v.m_data[x];
		}
	}

	Array(const T* v, int len)
	: m_len(len)
	{
		if (0 == m_len)
		{
			m_data = NULL;
			return;
		}

		m_data = new T[m_len];

		for ( int x = 0; x < m_len; x++ )
		{
			m_data[x] = v[x];
		}
	}

	virtual ~Array()
	{
		if (NULL != m_data)
		{
			ASSERT_MEM( m_data, sizeof(T) * m_len );
			delete[] m_data;
		}
	}

	Array& operator =(const Array& v)
	{
		Array<T>::ValidateMem();

		if (m_len > 0)
		{
			delete[] m_data;
		}
		else
		{
			ASSERT(NULL == m_data);
		}

		if (0 == (m_len = v.m_len))
		{
			m_data = NULL;
		}
		else
		{
			m_data = new T[m_len];
		}

		for ( int x = 0; x < m_len; x++ )
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

	/** @brief Get an element
	  *
	  * @param pos[in] Index in the vector.
	  * @return T
	  */
	inline T ElementAt( const int pos ) const
	{
		if (pos >= m_len || 0 > pos)
		{
			throw new IndexOutOfBoundsException();
		}
		ASSERT_MEM( m_data, sizeof(T) * m_len);

		return m_data[pos];
	}

	/** @brief Get a reference to an element
	  *
	  * @param pos[in] Index in the vector.
	  * @return T
	  */
	inline T& ElementAtRef( const int pos ) const
	{
		ASSERT_MEM( m_data, sizeof(T) * m_len );
		if ( pos >= m_len || 0 > pos )
		{
			throw IndexOutOfBoundsException();
		}
		return m_data[pos];
	}

	inline void SetElementAt( const int pos, T val )
	{
		if ( pos >= m_len || 0 > pos )
		{
			throw IndexOutOfBoundsException();
		}
		m_data[pos] = val;
	}

	inline T& operator[] (const int idx)
	{
		return ElementAtRef(idx);
	}

	inline const T& operator[] (const int idx) const
	{
		return ElementAtRef(idx);
	}

	inline operator T*() const
	{
		return m_data;
	}

	inline T* Data() const
	{
		return m_data;
	}

	/** @brief Resets the vector count to zero.
	  *
	  * Does not deallocate or clear any vector elements.
	  */
	inline void Clear()
	{
		Clear(0, Length());
	}

	void Clear(int start, int len)
	{
		ASSERT(len - start <= m_len);
		for ( int x = start; x < m_len && x < len; x++ )
		{
			m_data[x + start] = T();
		}
	}

	inline void ClearBinary()
	{
		ClearBinary(0, Length());
	}

	void ClearBinary(int start, int len)
	{
		ASSERT(len - start <= m_len);
		memset(m_data, 0, sizeof(T) * (len - start));
		for ( int x = start; x < m_len && x < len; x++ )
		{
			m_data[x + start] = T();
		}
	}

	/** @brief Number of elements in the vector.
	  *
	  */
	inline int Count() const
	{
		return m_len;
	}

	inline int Length() const
	{
		return m_len;
	}

	bool Equals(const Array<T>& a) const
	{
		if (m_len != a.m_len)
		{
			return false;
		}
		for (int x = 0; x < m_len; x++)
		{
			if (m_data[x] != a.m_data[x])
			{
				return false;
			}
		}
		return true;
	}

	bool AreElementsEqualTo(const T& val) const
	{
		for (int x = 0; x < m_len; x++)
		{
			if (m_data[x] != val)
			{
				return false;
			}
		}
		return true;
	}

	inline RefCountPtr<Array<T> > Clone() const
	{
		return RefCountPtr<Array<T> >(new Array<T>(*this));
	}

	void CopyTo(Array<T>& to, int sourceStart) const
	{
		if (sourceStart >= m_len)
		{
			return;
		}
		int pos = 0;
		for (int x = sourceStart; x < m_len && pos < to.m_len; x++)
		{
			to.m_data[pos++] = m_data[x];
		}
	}

	inline void CopyTo(Array<T>& to) const
	{
		CopyTo(to, 0);
	}

	void CopyToBinary(Array<T>& to, int sourceStart) const
	{
		if (sourceStart >= m_len)
		{
			return;
		}
		int len = m_len - sourceStart;
		if (len >= to.Length())
		{
			len = to.Length();
		}
		memcpy(to.m_data, &m_data[sourceStart], sizeof(T) * len);
	}

	inline void CopyToBinary(Array<T>& to) const
	{
		CopyToBinary(to, 0);
	}

	inline static bool Equals(const Array<T>& a, const Array<T>& b)
	{
		return Equals(a, 0, b, 0, a.Length());
	}

	static bool Equals(const Array<T>& a, int astart, const Array<T>& b, int bstart, int len)
	{
		int diff = (a.Length() - astart) - (b.Length() - bstart);
		if (0 != diff)
		{			
			return false;
		}

		int length = a.Length() - astart;
		if (length < len)
		{
			return false;
		}
		for ( int x = 0; x < length; x++)
		{
			if (!(a[x + astart] == b[x + bstart]))
			{
				return false;
			}
		}
		return true;
	}

	static void Copy(const Array<T>& from, int fromStart, Array<T>& to, int toStart, int length)
	{
		for ( int x = 0; x < length; x++)
		{
			to[toStart + x] = from[fromStart + x];
		}
	}

	inline static void CopyBinary(const Array<T>& from, int fromStart, Array<T>& to, int toStart, int length)
	{
		memmove(&to.m_data[toStart], &from.m_data[fromStart], sizeof(T) * length);
	}

	///@TODO: Use a real sort algorithm.
	void SortAsc()
	{
		T tmp;
		for (int x = 0; x < m_len-1; x++ )
		{
			for ( int y = x + 1; y < m_len; y++ )
			{
				if (m_data[x] < m_data[y])
				{
					tmp = m_data[x];
					m_data[x] = m_data[y];
					m_data[y] = tmp;
				}
			}
		}
	}

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const
	{
		if (NULL != m_data)
		{
			DEBUG_NOTE_MEM_ALLOCATION( m_data );
			for ( int x = 0; x < m_len; x++ )
			{
				TypeCheckMem( m_data[x] );
			}
		}
	}

	void ValidateMem() const
	{
		if (NULL != m_data)
		{
			ASSERT_MEM(m_data, sizeof(T) * m_len);
			for ( int x = 0; x < m_len; x++ )
			{
				TypeValidate( m_data[x] );
			}
		}
	}
#endif
};

/** @} */
}
#endif
