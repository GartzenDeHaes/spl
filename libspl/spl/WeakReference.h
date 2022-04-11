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
#ifndef _weakreference_h
#define _weakreference_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

/** @brief Use weak references in place of RefCountPtr's to break circular pointer dependancies.
 *
 */
template<class BASE, class BASEPTR>
class WeakReference : public IMemoryValidate
{
private:
    _PtrHolder<BASE> *m_ptr;

public:
	inline WeakReference()
	: m_ptr(NULL)
	{
	}

	inline WeakReference(BASEPTR& rptr)
	: m_ptr(rptr.GetHolder())
	{
	}

	virtual ~WeakReference()
	{
	}

	inline operator BASEPTR() const
	{
		return BASEPTR(m_ptr);
	}

	inline WeakReference& operator =(BASEPTR& rptr)
	{
		m_ptr = rptr.GetHolder();
		return *this;
	}

    // value access
    inline BASE* Get() const
    {
		if (NULL == m_ptr)
		{
			return NULL;
		}
		ASSERT(m_ptr->Count() > 0);
        return m_ptr->ptr;
    }

    inline BASE* operator->() const
    {
		ASSERT(NULL != m_ptr);
		ASSERT(m_ptr->Count() > 0);
        return m_ptr->ptr;
    }

	inline operator BASE&() const
	{
		if (NULL == m_ptr)
		{
			throw new Exception("NULL pointer");
		}
		ASSERT(m_ptr->Count() > 0);
        return *m_ptr->ptr;
	}

	inline void Clear()
	{
		m_ptr = NULL;
	}

	inline bool IsNull() const
	{
		return m_ptr == NULL;
	}

	inline bool IsNotNull() const
	{
		return ! IsNull();
	}

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const
	{
	}

	void ValidateMem() const
	{
		if (NULL != m_ptr)
		{
			ASSERT_MEM(m_ptr, sizeof(_PtrHolder<BASE>));
			ASSERT_MEM(m_ptr->ptr, sizeof(BASE));
			TypeValidate( m_ptr->ptr );
		}
	}
#endif
};

/** @} */
}
#endif
