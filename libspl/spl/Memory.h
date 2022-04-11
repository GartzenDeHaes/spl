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
#ifndef _memory_h
#define _memory_h

#include <spl/types.h>
#include <spl/Debug.h>

namespace spl
{
/** 
 * @defgroup debugging Debugging
 * @ingroup core
 * @{
 */

/** @brief Allows instances in collections be to automatically validated.
 *
 */
class IMemoryValidate
{
public:
	IMemoryValidate() {}
	virtual ~IMemoryValidate();

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

inline void TypeValidate( const IMemoryValidate *p )
{
	if ( NULL != p )
	{
		ASSERT_PTR( p );
		p->ValidateMem();
	}
}

inline void TypeValidate( const IMemoryValidate& p )
{
	p.ValidateMem();
}

inline void TypeCheckMem( const IMemoryValidate *p )
{
	if ( NULL != p )
	{
		DEBUG_NOTE_MEM(p);
		p->CheckMem();
	}
}

inline void TypeCheckMem( const IMemoryValidate& p )
{
	p.CheckMem();
}

inline void TypeCheckMem( const void *p )
{
	Debug::NoteMem(p);
}

inline void TypeValidate( const void *p )
{
	Debug::AssertPtr(p);
}

/** @} */
}
#endif
