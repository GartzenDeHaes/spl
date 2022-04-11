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
#include <spl/interp/JsMethod.h>

using namespace spl;

JsMethod::JsMethod()
: m_src("function anonymous( ) { [native code] }"), m_prog(new Program())
{
}

JsMethod::JsMethod(ProgramPtr prog)
: m_src("function anonymous( ) { [native code] }"), m_prog(prog)
{
}

JsMethod::JsMethod(const JsMethod& obj)
: JsObject(obj), m_src(obj.m_src), m_prog(obj.m_prog)
{
}

JsMethod::~JsMethod()
{
}

IJsObjectPtr JsMethod::New()
{
	JsMethod *obj = new JsMethod(*this);
	if (NULL != m_properties && m_properties->ContainsKey("super"))
	{
		SetProperty("super", m_properties->Get("super")->Clone());		
	}
	return IJsObjectPtr(obj);
}

bool JsMethod::IsNative() const
{
	return false;
}

int JsMethod::ArgumentCount()
{
	return m_prog->ArgumentCount();
}

VariantPtr JsMethod::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	throw new Exception("Call of non native method");
}

VariantPtr JsMethod::GetProperty(const String& idx)
{
	if ("arguments" == idx)
	{
		return VariantPtr(new Variant(m_prog->ArgumentCount()));
	}
	return JsObject::GetProperty(idx);
}

bool JsMethod::HasProperty(const String& idx) const
{
	return "arguments" == idx || JsObject::HasProperty(idx);
}

void JsMethod::SetProperty(const String& idx, VariantPtr obj)
{
	JsObject::SetProperty(idx, obj);
}

bool JsMethod::Equals( const IComparable& a ) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	
	return false;
}

int JsMethod::Compare( const IComparable& a ) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return -1;
	}
	
	return 0;
}

int32 JsMethod::MajicNumber() const
{
	return JSMETHOD_MAJIC;
}

uint32 JsMethod::HashCode() const
{
	uint32 hc = *(uint32 *)(void *)this;
	return hc;
}

StringPtr JsMethod::ToString() const
{
	return m_src.ToString();
}

StringPtr JsMethod::TypeName() const
{
	return StringPtr(new String("Function"));
}

#if defined(DEBUG) || defined(_DEBUG)
void JsMethod::CheckMem() const
{
	JsObject::CheckMem();
	m_src.CheckMem();
	m_prog.CheckMem();
}

void JsMethod::ValidateMem() const
{
	JsObject::ValidateMem();
	m_src.ValidateMem();
	m_prog.ValidateMem();
}
#endif

