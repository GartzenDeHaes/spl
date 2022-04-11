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
#ifndef _createonaccessptr_h
#define _createonaccessptr_h

#include <spl/Memory.h>

namespace spl
{
/** 
 * @defgroup smrtptr Smart Pointers
* @ingroup core
* @{
*/

template <class T> class LazyLoadPtr : public IMemoryValidate
{
private:
	T* m_ptr;

	void Create()
	{
		if (NULL == m_ptr) 
		{
			m_ptr = new T();
		}
	}

	LazyLoadPtr(const LazyLoadPtr&) 
	{
	}

	LazyLoadPtr& operator =(const LazyLoadPtr&) 
	{
	}

public:
	LazyLoadPtr()
	: m_ptr(NULL) 
	{
	}

	~LazyLoadPtr()
	{
		if (NULL != m_ptr)
		{
			delete m_ptr;
		}
	}

	T& operator*()
	{
		Create(); 
		return *m_ptr;
	}

	T* operator->()
	{
		Create(); 
		return m_ptr;
	}

	T* Get()
	{
		Create(); 
		return m_ptr;
	}
	
#ifdef DEBUG
	virtual void ValidateMem() const
	{
		if (NULL != m_ptr)
		{
			TypeValidate(m_ptr);
		}
	}

	virtual void CheckMem() const
	{
		if (NULL != m_ptr)
		{
			TypeCheckMem(m_ptr);
		}
	}
#endif
};

/** @} */
}
#endif
