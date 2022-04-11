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
#ifndef _jsobject_h
#define _jsobject_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Hashtable.h>
#include <spl/interp/IJsObject.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/Variant.h>

namespace spl
{
	class JsObject;
	typedef RefCountPtrCast<JsObject, IJsObject, IJsObjectPtr> JsObjectPtr;

	class JsObject : public IJsObject
	{
	protected:
		Hashtable<String, VariantPtr> *m_properties;
		JsObject *m_container;
		
	public:

		inline JsObject()
		: m_properties(NULL), m_container(NULL)
		{
		}
		
		JsObject(const JsObject& obj);
		virtual ~JsObject();

		virtual IJsObjectPtr New();
		
		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;
		virtual void SetProperty(const String& idx, VariantPtr obj);

		virtual bool Equals( const IComparable& a ) const;
		virtual int Compare( const IComparable& a ) const;

		///@brief Class instances with the same majic number are of the same type (can be casted).
		/// Majic numbers above 0xFFFF are available for user applications.
		virtual int32 MajicNumber() const;
		virtual uint32 HashCode() const;

		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;
		
		inline void SetOuterContext(VariantPtr& vp) { m_container = (JsObject *)vp->ToObject().Get(); }
		//static const char *CONTAINER_PROPERKTY_NAME;

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};

	REGISTER_TYPEOF2(699,Hashtable<String, VariantPtr>);
}
#endif
