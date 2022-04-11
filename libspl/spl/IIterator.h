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
#ifndef _iiterator_h
#define _iiterator_h

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

/** @brief Each collection type defines a specialized iterator for its contents.
 *
 */
template<typename T>
class IIterator
{
public:
	virtual bool Next( ) = 0;
	virtual bool Prev( ) = 0;
	virtual T Current() = 0;
	virtual T& CurrentRef() = 0;
};

/** @} */
}
#endif
