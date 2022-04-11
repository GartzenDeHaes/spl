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
#ifndef _jsarray_h
#define _jsarray_h

#include <spl/interp/JsMethod.h>
#include <spl/Variant.h>
#include <spl/collection/Vector.h>

namespace spl
{
	class JsArray;
	typedef RefCountPtrCast<JsArray, JsObject, JsObjectPtr> JsArrayPtr;

	class JsArray : public JsMethod
	{
	protected:
		Vector<VariantPtr> m_array;
		
	public:
		JsArray();
		JsArray(const JsArray& obj);
		virtual ~JsArray();

		virtual IJsObjectPtr New();
		
		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;
		virtual void SetProperty(const String& idx, VariantPtr obj);

		virtual bool Equals( const IComparable& a ) const;
		virtual int Compare( const IComparable& a ) const;

		virtual int32 MajicNumber() const;
		virtual uint32 HashCode() const;

		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;

		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		virtual int ArgumentCount();

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}
#endif
