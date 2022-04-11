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
#ifndef _refcountptr_h
#define _refcountptr_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Exception.h>
#include <spl/threading/InterlockCounter.h>
#include <spl/Memory.h>

namespace spl
{
	/**
	 * @defgroup smrtptr Smart Pointers
	 * @ingroup core
	 * @{
	 */

	/** @brief Private, for internal use */
	template<class Y>
	class _PtrHolder
	{
	private:
		mutable short m_icount;

	public:
		mutable Y* ptr;

		inline _PtrHolder (Y* rhs)
		: ptr(rhs), m_icount(1)
		{
		}

		virtual int Count()
		{
			return m_icount;
		}

		virtual void Inc()
		{
			m_icount++;
		}

		virtual int Dec()
		{
			return --m_icount;
		}

		virtual bool IsThreadSafe() const
		{
			return false;
		}
	};

	/** @brief Private, for internal use */
	template<class Y>
	class _PtrHolderThreadSafe : public _PtrHolder<Y>
	{
	private:
		mutable InterlockCounter m_tcount;

	public:
		inline _PtrHolderThreadSafe (Y* rhs)
		: _PtrHolder<Y>(rhs), m_tcount(1)
		{
		}

		virtual int Count()
		{
			return m_tcount.Get();
		}

		virtual void Inc()
		{
			m_tcount++;
		}

		virtual int Dec()
		{
			m_tcount--;
			return m_tcount.Get();
		}

		virtual bool IsThreadSafe() const
		{
			return true;
		}
	};

	template<class T>
	class RefCountPtr : public IMemoryValidate
	{
	private:
		mutable _PtrHolder<T> *m_ptr;

		inline void _Dec()
		{
			if (NULL != m_ptr)
			{
				ASSERT(m_ptr->Count() > 0);
				if (0 == m_ptr->Dec())
				{
					delete m_ptr->ptr;
					delete m_ptr;
				}
			}
		}

	public:
		typedef T element_type;

		// constructor
		explicit RefCountPtr (T* ptr, bool threadSafe = false)
		{
			if (NULL == ptr)
			{
				m_ptr = NULL;
				return;
			}
			ASSERT_PTR(ptr);

			if (threadSafe)
			{
				m_ptr = new _PtrHolderThreadSafe<T>(ptr);
			}
			else
			{
				m_ptr = new _PtrHolder<T>(ptr);
			}
		}

		// copy constructors (with implicit conversion)
		// - note: nonconstant parameter
		RefCountPtr (const RefCountPtr<T>& rhs)
		: m_ptr(rhs.m_ptr)
		{
			if (NULL != m_ptr)
			{
#ifdef DEBUG
				ASSERT(m_ptr->Count() > 0);
				if (m_ptr->IsThreadSafe())
				{
					ASSERT_MEM(m_ptr, sizeof(_PtrHolderThreadSafe<T>));
				}
				else
				{
					ASSERT_MEM(m_ptr, sizeof(_PtrHolder<T>));
				}
#endif
				m_ptr->Inc();
			}
		}

		RefCountPtr ()
		: m_ptr(NULL)
		{
		}

		RefCountPtr(_PtrHolder<T> *ptr)
		: m_ptr(ptr)
		{
			if(NULL != ptr)
			{
				m_ptr->Inc();
			}
		}

	  //  // assignments (with implicit conversion)
	  //  // - note: nonconstant parameter
		RefCountPtr<T>& operator =(const RefCountPtr<T>& rhs)
		{
			_Dec();

			if (NULL != (m_ptr = rhs.m_ptr))
			{
				if (m_ptr->IsThreadSafe())
				{
					ASSERT_MEM(m_ptr, sizeof(_PtrHolderThreadSafe<T>));
				}
				else
				{
					ASSERT_MEM(m_ptr, sizeof(_PtrHolder<T>));
				}
				m_ptr->Inc();
			}

			return *this;
		}

		RefCountPtr<T>& operator =(T *p)
		{
			_Dec();
			if (NULL == p)
			{
				m_ptr = NULL;
				return *this;
			}
			ASSERT_PTR(p);

			if (NULL == m_ptr || !m_ptr->IsThreadSafe())
			{
				m_ptr = new _PtrHolder<T>(p);
			}
			else
			{
				m_ptr = new _PtrHolderThreadSafe<T>(p);
			}

			return *this;
		}

	#ifndef _WINDOWS
		// gnu has stricter definition of const.
		// assignments (with implicit conversion)
		// - note: nonconstant parameter
		RefCountPtr<T>& operator= (RefCountPtr<T>& rhs)
		{
			return *this = (const RefCountPtr<T>&)rhs;
		}
	#endif

		// destructor
		virtual ~RefCountPtr()
		{
			Release();
		}

		inline int ReferenceCount() const
		{
			return NULL == m_ptr ? 0 : m_ptr->Count();
		}

		inline bool IsNull() const
		{
			return NULL == m_ptr || NULL == m_ptr->ptr;
		}

		inline bool IsNotNull() const
		{
			return !IsNull();
		}

		void Release()
		{
			if (NULL != m_ptr)
			{
				ASSERT(m_ptr->Count() > 0);

				if (0 == m_ptr->Dec())
				{
					delete m_ptr->ptr;
					delete m_ptr;
				}
				m_ptr = NULL;
			}
		}

		T* Detach()
		{
			if(m_ptr == NULL)
			{
				return NULL;
			}
			ASSERT(m_ptr->Count() == 1);
			T* ret = m_ptr->ptr;
			delete m_ptr;
			m_ptr = NULL;
			return ret;
		}

		// value access
		inline T* Get() const
		{
			if (NULL == m_ptr)
			{
				return NULL;
			}
			return m_ptr->ptr;
		}

		// This is for casting of pointers
		inline _PtrHolder<T> *GetHolder() const
		{
			return m_ptr;
		}

		inline T& operator*() const
		{
			ASSERT(NULL != m_ptr);
			return *m_ptr->ptr;
		}

		inline T* operator->() const
		{
			ASSERT(NULL != m_ptr);
			return m_ptr->ptr;
		}

		inline operator T&() const
		{
			if (NULL == m_ptr)
			{
				throw new Exception("NULL pointer");
			}
			return *m_ptr->ptr;
		}

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const
		{
			if (NULL != m_ptr)
			{
				DEBUG_NOTE_MEM(m_ptr);
				DEBUG_NOTE_MEM(m_ptr->ptr);
				TypeCheckMem( m_ptr->ptr );
			}
		}

		void ValidateMem() const
		{
			if (NULL != m_ptr)
			{
				if (m_ptr->IsThreadSafe())
				{
					ASSERT_MEM(m_ptr, sizeof(_PtrHolderThreadSafe<T>));
				}
				else
				{
					ASSERT_MEM(m_ptr, sizeof(_PtrHolder<T>));
				}

				ASSERT_MEM(m_ptr->ptr, sizeof(T));
				TypeValidate( m_ptr->ptr );
			}
		}
	#endif
	};

	/** @} */
}
#endif
