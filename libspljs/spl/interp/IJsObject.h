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
#ifndef _ijsobject_h
#define _ijsobject_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/IVariant.h>
#include <spl/Memory.h>
#include <spl/RefCountPtrCast.h>
#include <spl/String.h>
#include <spl/Undefined.h>

namespace spl
{
	#define JSOBJECT_MAJIC 787		//< Majic number for ASSERT's in Compare and Convert
	#define JSNULL_MAJIC 788		//< Majic number for ASSERT's in Compare and Convert
	#define JSUNDEFINED_MAJIC 789	//< Majic number for ASSERT's in Compare and Convert
	#define JSARRAY_MAJIC 790		//< Majic number for ASSERT's in Compare and Convert
	#define JSMETHOD_MAJIC 791		//< Majic number for ASSERT's in Compare and Convert

	class Variant;
	class IJsObject;
	typedef RefCountPtr<IJsObject> IJsObjectPtr;

	class IJsObject : public IMemoryValidate, public IComparable
	{
	public:

		inline IJsObject()
		{
		}
		
		virtual ~IJsObject();

		virtual IJsObjectPtr New() = 0;

		virtual RefCountPtr<Variant> GetProperty(const String& idx) = 0;
		virtual bool HasProperty(const String& idx) const = 0;
		virtual void SetProperty(const String& idx, RefCountPtr<Variant> obj) = 0;

		virtual bool Equals( const IComparable& a ) const = 0;
		virtual int Compare( const IComparable& a ) const = 0;

		///@brief Class instances with the same majic number are of the same type (can be casted).
		/// Majic numbers above 0xFFFF are available for user applications.
		virtual int32 MajicNumber() const = 0;
		virtual uint32 HashCode() const = 0;

		virtual StringPtr ToString() const = 0;
		virtual StringPtr TypeName() const = 0;
		
	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const = 0;
		void ValidateMem() const = 0;
	#endif
	};

}

#endif
