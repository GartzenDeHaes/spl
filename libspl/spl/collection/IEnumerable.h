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
#ifndef _ienumerable_h
#define _ienumerable_h

#include <spl/Debug.h>
#include <spl/Delegate.h>
#include <spl/Memory.h>
#include <spl/IIterator.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

/** @brief Inteface for collections that can be iterated.
 *	@ref IIterator
 */
template<typename T>
class IEnumerable : public IMemoryValidate
{
public:
	virtual ~IEnumerable() {}
	
	virtual RefCountPtr<IIterator<T> > _Begin() = 0;

	inline void ForEach(IDelegateOneParameter<T&>& func)
	{
		RefCountPtr<IIterator<T> > iter = _Begin();
		while(iter->Next())
		{
			func.Call(iter->Current(iter->CurrentRef()));
		}
	}

#if defined(DEBUG)
	virtual void CheckMem() const {}
	virtual void ValidateMem() const {}
#endif
};

/** @} */
}
#endif
