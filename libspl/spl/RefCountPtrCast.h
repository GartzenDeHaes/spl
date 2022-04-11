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
#ifndef _refcountptrcast_h
#define _refcountptrcast_h

#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup smrtptr Smart Pointers
 * @ingroup core
 * @{
 */

template<class BASE, class CAST, class CASTPTR>
class RefCountPtrCast : public RefCountPtr<BASE>
{
public:
	inline RefCountPtrCast()
	: RefCountPtr<BASE>()
	{
	}

	inline RefCountPtrCast(BASE *s)
	: RefCountPtr<BASE>(s)
	{
	}

	inline RefCountPtrCast(const RefCountPtrCast& s)
	: RefCountPtr<BASE>((_PtrHolder<BASE> *)s.GetHolder())
	{
	}

	inline RefCountPtrCast(const CASTPTR& s)
	: RefCountPtr<BASE>((_PtrHolder<BASE> *)s.GetHolder())
	{
	}

	inline RefCountPtrCast(_PtrHolder<BASE> *holder)
	: RefCountPtr<BASE>(holder)
	{
	}

	~RefCountPtrCast()
	{
	}

	operator CASTPTR()
	{
		if (! RefCountPtr<BASE>::IsNull())
		{
			return CASTPTR((_PtrHolder<CAST> *)RefCountPtr<BASE>::GetHolder());
		}
		return CASTPTR();
	}
};

/** @} */
}
#endif
