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
#include <spl/interp/JsArray.h>
#include <spl/interp/JsObject.h>
#include <spl/Variant.h>

using namespace spl;

IJsObject::~IJsObject()
{
}

//const char *JsObject::CONTAINER_PROPERKTY_NAME = "$___container____$";

JsObject::JsObject(const JsObject& obj)
: m_properties(NULL), m_container(NULL)
{
	if (NULL != obj.m_properties)
	{
		foreach(v,(*obj.m_properties))
		{
			SetProperty(v.CurrentKey(), v.Current()->Clone());
		}
	}
}

JsObject::~JsObject()
{
	if (NULL != m_properties)
	{
		delete m_properties;
	}
}

IJsObjectPtr JsObject::New()
{
	JsObject *obj = new JsObject(*this);
	if (NULL != m_properties && m_properties->ContainsKey("super"))
	{
		obj->SetProperty("super", m_properties->Get("super")->Clone());
	}
	return IJsObjectPtr(obj);
}

VariantPtr JsObject::GetProperty(const String& idx)
{
	if (NULL == m_properties)
	{
		m_properties = new Hashtable<String, VariantPtr>();
	}
	if (! m_properties->ContainsKey(idx))
	{
		if (m_properties->ContainsKey("super"))
		{
			if (m_properties->Get("super")->ToObject()->HasProperty(idx))
			{
				return m_properties->Get("super")->ToObject()->GetProperty(idx);
			}
		}
		if (NULL != m_container)
		{
			if (m_container->HasProperty(idx))
			{
				return m_container->GetProperty(idx);
			}
		}
		m_properties->Set(idx, VariantPtr(new Variant(Undefined::Instance())));
	}
	return m_properties->Get(idx);
}

bool JsObject::HasProperty(const String& idx) const
{
	if (NULL != m_properties)
	{
		if( m_properties->ContainsKey(idx) )
		{
			return true;
		}
		if (m_properties->ContainsKey("super"))
		{
			if (m_properties->Get("super")->IsObject())
			{
				return m_properties->Get("super")->ToObject()->HasProperty(idx);
			}
		}
	}
	return false;
}

void JsObject::SetProperty(const String& idx, VariantPtr obj)
{
	if (NULL == m_properties)
	{
		m_properties = new Hashtable<String, VariantPtr>();
	}
	if (m_properties->ContainsKey("super"))
	{
		if (m_properties->Get("super")->ToObject()->HasProperty(idx))
		{
			m_properties->Get("super")->ToObject()->SetProperty(idx, obj);
			return;
		}
	}
	if (!m_properties->ContainsKey(idx) && NULL != m_container)
	{
		if (m_container->HasProperty(idx))
		{
			m_container->SetProperty(idx, obj);
			return;
		}
	}
	m_properties->Set(idx, obj);
}

StringPtr JsObject::ToString() const
{
	return StringPtr(new String("[object Object]"));
}

StringPtr JsObject::TypeName() const
{
	return StringPtr(new String("Object"));
}

bool JsObject::Equals( const IComparable& a ) const
{
	///TODO doesn't consider properties WHY?
	if (a.MajicNumber() == MajicNumber())
	{
		const JsObject &j2 = static_cast<const JsObject &>(a);
		j2.ValidateMem();
		return (void *)this == (void *)&j2;
	}
	return false;
}

int JsObject::Compare( const IComparable& a ) const
{
	///TODO doesn't consider properties WHY?
	if (a.MajicNumber() == MajicNumber())
	{
		const JsObject &j2 = static_cast<const JsObject &>(a);
		j2.ValidateMem();
		return ToString()->Compare(*j2.ToString());
	}
	return -1;
}

int32 JsObject::MajicNumber() const
{
	return JSOBJECT_MAJIC;
}

uint32 JsObject::HashCode() const
{
	return *static_cast<const uint32 *>(static_cast<const void *>(this));
}

#if defined(DEBUG) || defined(_DEBUG)
void JsObject::CheckMem() const
{
	if (NULL != m_properties)
	{
		Debug::NoteMem(m_properties);
		m_properties->CheckMem();
	}
}

void JsObject::ValidateMem() const
{
	if (NULL != m_properties)
	{
		Debug::AssertMem(m_properties, sizeof(Hashtable<String, VariantPtr>));
		m_properties->ValidateMem();
	}
	if (NULL != m_container)
	{
		Debug::AssertPtr(m_container);
	}
}
#endif

//JsNull::JsNull()
//{
//	m_val = new Variant(Undefined());
//}
//
//JsNull::JsNull(const JsNull& obj)
//{
//	m_val = new Variant(Undefined());
//}
//
//JsNull::~JsNull()
//{
//}
//
//VariantPtr JsNull::GetProperty(const String& idx)
//{
//	return VariantPtr(new Variant(*(Variant *)m_val));
//}
//
//void JsNull::SetProperty(const String& idx, VariantPtr obj)
//{
//}
//
//bool JsNull::HasProperty(const String& idx) const
//{
//	return false;
//}
//
//
//StringPtr JsNull::ToString() const
//{
//	return StringPtr(new String("null"));
//}
//
//bool JsNull::Equals( const IComparable& a ) const
//{
//	// doesn't consider properties
//	return a.MajicNumber() == MajicNumber();
//}
//
//int JsNull::Compare( const IComparable& a ) const
//{
//	// doesn't consider properties
//	return (a.MajicNumber() == MajicNumber()) ? 0 : -1;
//}
//
//int32 JsNull::MajicNumber() const
//{
//	return JSNULL_MAJIC;
//}
//
//int32 JsNull::HashCode() const
//{
//	return 0;
//}
//
//#if defined(DEBUG) || defined(_DEBUG)
//void JsNull::CheckMem() const
//{
//	DEBUG_NOTE_MEM(m_val);
//	m_val->CheckMem();
//}
//
//void JsNull::ValidateMem() const
//{
//	ASSERT_MEM(m_val, sizeof(Variant));
//	m_val->ValidateMem();
//}
//#endif
//
//JsUndefined::JsUndefined()
//{
//	m_val = new Variant(Undefined());
//}
//
//JsUndefined::JsUndefined(const JsUndefined& obj)
//{
//	m_val = new Variant(Undefined());
//}
//
//JsUndefined::~JsUndefined()
//{
//	delete m_val;
//}
//
//VariantPtr JsUndefined::GetProperty(const String& idx)
//{
//	return VariantPtr(new Variant(*(Variant *)m_val));
//}
//
//bool JsUndefined::HasProperty(const String& idx) const
//{
//	return false;
//}
//
//void JsUndefined::SetProperty(const String& idx, VariantPtr obj)
//{
//}
//
//StringPtr JsUndefined::ToString() const
//{
//	return StringPtr(new String("undefined"));
//}
//
//bool JsUndefined::Equals( const IComparable& a ) const
//{
//	// doesn't consider properties
//	return a.MajicNumber() == MajicNumber();
//}
//
//int JsUndefined::Compare( const IComparable& a ) const
//{
//	// doesn't consider properties
//	return (a.MajicNumber() == MajicNumber()) ? 0 : -1;
//}
//
//int32 JsUndefined::MajicNumber() const
//{
//	return JSUNDEFINED_MAJIC;
//}
//
//int32 JsUndefined::HashCode() const
//{
//	return -1;
//}
//
//#if defined(DEBUG) || defined(_DEBUG)
//void JsUndefined::CheckMem() const
//{
//	DEBUG_NOTE_MEM(m_val);
//	m_val->CheckMem();
//}
//
//void JsUndefined::ValidateMem() const
//{
//	ASSERT_MEM(m_val, sizeof(Variant));
//	m_val->ValidateMem();
//}
//#endif
