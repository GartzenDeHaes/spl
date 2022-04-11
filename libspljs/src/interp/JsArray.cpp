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
#include <spl/Int32.h>
#include <spl/interp/JsArray.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

JsArray::JsArray()
: m_array()
{
}

JsArray::JsArray(const JsArray& obj)
: JsMethod(obj), m_array(obj.m_array)
{
}

JsArray::~JsArray()
{
}

IJsObjectPtr JsArray::New()
{
	JsArray *obj = new JsArray();
	foreach(a,m_array)
	{
		obj->m_array.Add(a.CurrentRef()->Clone());
	}	
	return IJsObjectPtr(obj);
}

VariantPtr JsArray::GetProperty(const String& idx)
{
	if ("length" == idx)
	{
		return VariantPtr(new Variant(m_array.Count()));
	}
	if (Int32::IsInt(idx))
	{
		int iidx = Int32::Parse(idx);
		m_array.EnsureElementTo(iidx);

		if (iidx >= m_array.Count())
		{
			m_array.SetElementAt(VariantPtr(new Variant()), iidx);
		}
		
		VariantPtr var = m_array.ElementAt(iidx);
		if (var.IsNull())
		{
			var = VariantPtr(new Variant());
		}
		return var;
	}
	else
	{
		return JsMethod::GetProperty(idx);
	}
}

bool JsArray::HasProperty(const String& idx) const
{
	return "length" == idx || Int32::IsInt(idx) || JsObject::HasProperty(idx);
}

void JsArray::SetProperty(const String& idx, VariantPtr obj)
{
	if ("length" == idx)
	{
		return;
	}
	if (Int32::IsInt(idx))
	{
		int iidx = Int32::Parse(idx);
		m_array.EnsureElementTo(iidx);		
		m_array.SetElementAt(obj, iidx);
		return;
	}
	
	JsMethod::SetProperty(idx, obj);
}

bool JsArray::IsNative() const
{
	return true;
}

VariantPtr JsArray::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	int len;
	if (args.Count() == 1 && args.ElementAtRef(0)->ToInt32(len))
	{
		Debug::Assert(!args.ElementAt(0)->IsUndefined());
		m_array.EnsureElementTo(len);
		for (int x = 0; x < len; x++)
		{
			m_array.Add(VariantPtr(new Variant()));
		}
	}
	else
	{
		for (int x = 0; x < args.Count(); x++ )
		{
			Debug::Assert(!args.ElementAt(x)->IsUndefined());
			SetProperty(Int32::ToString(x), args.ElementAt(x));
		}
	}
	return VariantPtr(new Variant());
}

int JsArray::ArgumentCount()
{
	return 0;
}

bool JsArray::Equals( const IComparable& a ) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	
	const JsArray &arr = (const JsArray&)a;
	if (arr.m_array.Count() != m_array.Count())
	{
		return false;
	}
	
	for (int x = 0; x < m_array.Count(); x++)
	{
		VariantPtr& v = m_array.ElementAtRef(x);
		if (! v->Equals(*arr.m_array.ElementAtRef(x)))
		{
			return false;
		}
	}
	return true;
}

int JsArray::Compare( const IComparable& a ) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return -1;
	}
	
	const JsArray &arr = (const JsArray&)a;
	if (arr.m_array.Count() != m_array.Count())
	{
		return arr.m_array.Count() - m_array.Count();
	}
	
	for (int x = 0; x < m_array.Count(); x++)
	{
		VariantPtr& v = m_array.ElementAtRef(x);
		int cmp = v->Compare(*arr.m_array.ElementAtRef(x));
		if (0 != cmp)
		{
			return cmp;
		}
	}
	return 0;
}

int32 JsArray::MajicNumber() const
{
	return JSARRAY_MAJIC;
}

uint32 JsArray::HashCode() const
{
	uint32 hc = 0;
	for(Vector<VariantPtr>::Iterator v = m_array.Begin(); v.Next();)
	{
		hc ^= v.CurrentRef()->HashCode();
	}
	return hc;
}

StringPtr JsArray::ToString() const
{
	StringBuffer buf;
	
	for(Vector<VariantPtr>::Iterator v = m_array.Begin(); v.Next();)
	{
		if (buf.Length() > 0)
		{
			buf.Append(',');
		}
		buf.Append(v.CurrentRef()->ToString());
	}
	return buf.ToString();
}

StringPtr JsArray::TypeName() const
{
	return StringPtr(new String("Array"));
}

#if defined(DEBUG) || defined(_DEBUG)
void JsArray::CheckMem() const
{
	JsMethod::CheckMem();
	m_array.CheckMem();
}

void JsArray::ValidateMem() const
{
	JsMethod::ValidateMem();
	m_array.ValidateMem();
}
#endif

