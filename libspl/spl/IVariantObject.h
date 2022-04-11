
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
#ifndef _ivariantobject_h
#define _ivariantobject_h

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/RefCountPtrCast.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

	class IVariantObject;
	typedef RefCountPtr<IVariantObject> IVariantObjectPtr;

	/** @brief Object that a variant can hold.  
	 *	
	*	@ref Variant
	*/
	class IVariantObject : public IMemoryValidate, public IComparable
	{
		public:

		inline IVariantObject()
		{
		}

		virtual ~IVariantObject();

		virtual bool Equals( const IComparable& a ) const = 0;
		virtual int Compare( const IComparable& a ) const = 0;

		///@brief Class instances with the same majic number are of the same type (can be casted).
		virtual int32 MajicNumber() const = 0;
		virtual uint32 HashCode() const = 0;

		virtual StringPtr ToString() const = 0;
		virtual StringPtr TypeName() const = 0;

		#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const = 0;
		void ValidateMem() const = 0;
		#endif
	};

/** @} */
};

#endif
