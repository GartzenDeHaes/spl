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
#ifndef _tcirculararray_h
#define _tcirculararray_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Exception.h>
#include <spl/Memory.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

class CircularArrayException : public Exception
{
public:
	CircularArrayException(const char *msg);
};

/** @brief An array that wraps back to the first element when elements are written past the end. */
template<typename T>
class CircularArray: public IMemoryValidate
{
protected:
	int m_size;
	T *m_array;
	int m_writepos;
	int m_readpos;

public:
	CircularArray( int size ) : m_writepos(0), m_readpos(0)
	{
		m_size = size;
		m_array = new T[size];
	}

	CircularArray( const CircularArray<T>& ar )
	: m_writepos(ar.m_writepos), m_readpos(ar.m_readpos), m_size(ar.m_size)
	{
		m_array = new T[m_size];
		for ( int x = 0; x < m_size; x++ )
		{
			m_array[x] = ar.m_array[x];
		}
	}

	virtual ~CircularArray()
	{
		delete m_array;
	}

	void Write( T data )
	{
		ASSERT_MEM( m_array, m_size*sizeof(T) );
		if ( m_size == m_writepos )
		{
			m_writepos = 0;
		}
		m_array[m_writepos++] = data;
		if ( m_writepos == m_readpos )
		{
			throw new CircularArrayException("insert overwrote read pos -- make array larger");
		}
	}

	void Write( T *data, int start, int len )
	{
		int end = len + start;
		for ( int x = start; x < end; x++ )
		{
			write( data[x] );
		}
	}

	void Read( T *data )
	{
		ASSERT_MEM( m_array, m_size*sizeof(T) );
		if ( m_readpos == m_writepos )
		{
			CircularArrayException("EOF");
		}
		if ( m_readpos == m_size )
		{
			m_readpos = 0;
		}
		*data = m_array[m_readpos++];
	}

	void Read( T *data, int start, int len )
	{
		int end = len + start;
		for ( int x = start; x < end; x++ )
		{
			Read( &data[x] );
		}
	}

	int Count() const
	{
		if ( m_writepos >= m_readpos )
		{
			return m_writepos - m_readpos;
		}
		return m_size - m_readpos + m_writepos;
	}

	T Peek( int offset ) const
	{
		ASSERT_MEM( m_array, m_size*sizeof(T) );
		int pos = m_readpos + offset;
		if ( offset == m_writepos )
		{
			CircularArrayException("EOF");
		}
		if ( pos >= m_size )
		{
			 pos -= m_size;
		}
		return m_array[pos];
	}

	int LocateElement( T item ) const
	{
		int cnt = Count();
		for ( int x = 0; x < cnt; x++ )
		{
			if ( Peek(x) == item )
			{
				return x;
			}
		}
		return -1;
	}

	CircularArray<T>& operator =( const CircularArray<T>& ar )
	{
		delete[] m_array;

		m_writepos = ar.m_writepos;
		m_readpos = ar.m_readpos;
		m_size = ar.m_size;

		m_array = new T[m_size];
		for ( int x = 0; x < m_size; x++ )
		{
			m_array[x] = ar.m_array[x];
		}

		return *this;
	}

#ifdef DEBUG
	virtual void ValidateMem() const
	{
		ASSERT_MEM(m_array, m_size*sizeof(T));
	}

	virtual void CheckMem() const
	{
		DEBUG_NOTE_MEM_ALLOCATION(m_array);
		for ( int x = 0; x < m_size; x++ )
		{
			TypeCheckMem(m_array[x]);
		}
	}
#endif
};

/** @} */
}
#endif
