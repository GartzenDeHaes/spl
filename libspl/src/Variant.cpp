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
#include <spl/Double.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
#include <spl/Variant.h>
#include <spl/math/Math.h>
//#include <spl/interp/JsArray.h>
//#include <spl/interp/JsMethod.h>
//#include <spl/interp/JsObject.h>

using namespace spl;

IVariant::IVariant()
{
}

IVariant::~IVariant()
{
}

IVariantData::IVariantData()
{
}

IVariantData::~IVariantData()
{
}

Variant::Variant()
: m_data(new _VariantData<Undefined>(Undefined()))
{
}

Variant::Variant(IVariantData *m_data)
: m_data(m_data)
{
}

Variant::Variant(bool flag)
: m_data(new _VariantData<bool>(flag))
{
}

Variant::Variant(int8 i8)
: m_data(new _VariantData<int8>(i8))
{
}

Variant::Variant(int16 i16)
: m_data(new _VariantData<int16>(i16))
{
}

Variant::Variant(int32 i32)
: m_data(new _VariantData<int32>(i32))
{
}

Variant::Variant(int64 i64)
: m_data(new _VariantData<int64>(i64))
{
}

Variant::Variant(float32 f32)
: m_data(new _VariantData<float32>(f32))
{
}

Variant::Variant(float64 f64)
: m_data(new _VariantData<float64>(f64))
{
}

Variant::Variant(const DateTime &dtm)
: m_data(new _VariantData<DateTime>(dtm))
{
}

Variant::Variant(const Date& dt)
: m_data(new _VariantData<Date>(dt))
{
}

Variant::Variant(const String& str)
: m_data(new _VariantData<String>(str))
{
}

Variant::Variant(const String *str)
: m_data(new _VariantData<String>(*str))
{
}

Variant::Variant(const char * str)
: m_data(new _VariantData<String>(String(str)))
{
}

Variant::Variant(const Decimal& dec)
: m_data(new _VariantData<Decimal>(dec))
{
}

Variant::Variant(const Variant& v)
: m_data(v.m_data->Copy())
{
}

Variant::Variant(const Undefined& u)
: m_data(new _VariantData<Undefined>(u))
{
}

Variant::Variant(const Null& n)
: m_data(new _VariantData<Null>(n))
{
}

//Variant::Variant(RefCountPtr<IJsObject> jso)
//: m_data(new _VariantData<RefCountPtr<IJsObject> >(jso))
//{
//}

Variant::~Variant()
{
	delete m_data;
}

RefCountPtr<Variant> Variant::ParseInt() const
{
	if (Convert::CanConvertToInt(m_data))
	{
		int64 a;
		Convert::ToInt64(m_data, a);
		return VariantPtr(new Variant(a));
	}
	StringPtr val = ToString();
	if (Int64::IsInt(*val))
	{
		return VariantPtr(new Variant(Int64::Parse(*val)));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ParseFloat() const
{
	if (Convert::CanConvertToFloat(m_data))
	{
		float64 a;
		Convert::ToFloat64(m_data, a);
		return VariantPtr(new Variant(a));
	}
	StringPtr val = ToString();
	if (Double::IsDouble(*val))
	{
		return VariantPtr(new Variant(Double::Parse(*val)));
	}
	return VariantPtr(new Variant());
}

bool Variant::IsObject() const
{
	return m_data->IsObject();
}

String Variant::TypeName() const
{
	return "Variant";
}

Variant &Variant::operator =(const Variant &v)
{
	ASSERT(m_data != v.m_data);
	ValidateMem();
	delete m_data;
	m_data = v.m_data->Copy();
	return *this;
}

Variant& Variant::operator =(const int32 i)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<int32>(i);
	return *this;
}

Variant& Variant::operator =(const int64 i)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<int64>(i);
	return *this;
}

Variant& Variant::operator =(const float64 d)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<float64>(d);
	return *this;	
}

Variant& Variant::operator =(const String& s)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<String>(s);
	return *this;	
}

Variant& Variant::operator =(const char *cp)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<String>(cp);
	return *this;	
}

Variant& Variant::operator =(const DateTime& s)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<DateTime>(s);
	return *this;	
}

Variant& Variant::operator =(const Date& s)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<Date>(s);
	return *this;	
}

Variant& Variant::operator =(const Decimal& s)
{
	ValidateMem();
	delete m_data;
	m_data = new _VariantData<Decimal>(s);
	return *this;	
}

//Variant& Variant::operator =(RefCountPtr<IJsObject> s)
//{
//	ValidateMem();
//	delete m_data;
//	m_data = new _VariantData<RefCountPtr<IJsObject> >(s);
//	return *this;	
//}

bool Variant::ToBool() const
{
	bool b;

	if (m_data->ToBool(b))
	{
		return b;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to bool.");
}

byte Variant::ToByte() const
{
	byte b;
	if (m_data->ToByte(b))
	{
		return b;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to byte.");
}

int16 Variant::ToInt16() const
{
	int16 i;
	if(m_data->ToInt16(i))
	{
		return i;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to int16.");
}

int32 Variant::ToInt32() const
{
	int32 i;
	if (m_data->ToInt32(i))
	{
		return i;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to int32.");
}

int64 Variant::ToInt64() const
{
	int64 i;
	if (m_data->ToInt64(i))
	{
		return i;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to int64.");
}

float32 Variant::ToFloat32() const
{
	float32 d;
	if (m_data->ToFloat32(d))
	{
		return d;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to float32.");
}

float64 Variant::ToFloat64() const
{
	float64 d;
	if (m_data->ToFloat64(d))
	{
		return d;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to float64.");
}

DateTime Variant::ToDateTime() const
{
	DateTime dtm;
	if(m_data->ToDateTime(dtm))
	{
		return dtm;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to DateTime.");
}

Date Variant::ToDate() const
{
	Date dt;
	if(m_data->ToDate(dt))
	{
		return dt;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to Date.");
}

Decimal Variant::ToDecimal() const
{
	Decimal d;
	if(m_data->ToDecimal(d))
	{
		return d;
	}
	throw InvalidTypeConversionException("Cannot convert " + m_data->TypeName() + " to Decimal.");
}

StringPtr Variant::ToString() const
{
	String s;
	m_data->ToString(s);
	return s.Clone();
}

/*RefCountPtr<IJsObject> Variant::ToObject() const
{
	ASSERT(IsObject());

	RefCountPtr<IJsObject> obj = static_cast<_VariantData<RefCountPtr<IJsObject> > *>(m_data)->GetRef();
	obj.ValidateMem();

	if (m_data->MajicNumber() == JSOBJECT_MAJIC)
	{
		ASSERT_MEM(obj.Get(), sizeof(JsObject));
		return obj;
	}
	if (m_data->MajicNumber() == JSARRAY_MAJIC)
	{
		ASSERT_MEM(obj.Get(), sizeof(JsArray));
		return obj;
	}
	if (m_data->MajicNumber() == JSMETHOD_MAJIC)
	{
		ASSERT_MEM(obj.Get(), sizeof(JsMethod));
		return obj;
	}
	throw new Exception("Internal error Variant::ToObject");
}
*/

bool Variant::ToChar(char& c2) const
{
	return false;
}

bool Variant::ToBool(bool& c2) const
{
	c2 = ToBool();
	return true;
}

bool Variant::ToByte(byte& c2) const
{
	c2 = ToByte();
	return true;
}

bool Variant::ToInt16(int16& c2) const
{
	return m_data->ToInt16(c2);
}

bool Variant::ToInt32(int32& c2) const
{
	return m_data->ToInt32(c2);
}

bool Variant::ToInt64(int64& c2) const
{
	return m_data->ToInt64(c2);
}

bool Variant::ToUInt32(uint32& c2) const
{
	return false;
}

bool Variant::ToUInt64(uint64& c2) const
{
	return false;
}

//bool Variant::ToBigInteger(const BigInteger& c2) const
bool Variant::ToFloat32(float32& c2) const
{
	return m_data->ToFloat32(c2);
}

bool Variant::ToFloat64(float64& c2) const
{
	return m_data->ToFloat64(c2);
}

bool Variant::ToDateTime(DateTime& c2) const
{
	c2 = ToDateTime();
	return true;
}

bool Variant::ToDate(Date& c2) const
{
	c2 = ToDate();
	return true;
}

bool Variant::ToDecimal(Decimal& c2) const
{
	c2 = ToDecimal(c2);
	return true;
}

bool Variant::ToString(String& c2) const
{
	c2 = *ToString();
	return true;
}

bool Variant::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Variant& vp = static_cast<const Variant&>(a);
	vp.ValidateMem();
	return m_data->Equals(*vp.m_data);
}

bool Variant::Equals( const Variant& v ) const
{
	return m_data->Equals(*v.m_data);
}

int Variant::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	return m_data->Compare(a);
}

int32 Variant::MajicNumber() const
{
	return 0x1001;
}

uint32 Variant::HashCode() const
{
	return m_data->HashCode();
}

bool Variant::BinOp(const Variant& v, enum CompareOperator op) const
{
	if (Convert::IsFloat(m_data) || Convert::IsFloat(v.m_data))
	{
		float64 a, b;
		if (m_data->ToFloat64(a) && v.m_data->ToFloat64(b))
		{
			if (op == Variant::VOP_EQ)
			{
				return a == b;
			}
			if (op == Variant::VOP_GT)
			{
				return a > b;
			}
			if (op == Variant::VOP_LT)
			{
				return a < b;
			}
		}
	}
	if (Convert::IsInt(m_data) || Convert::IsInt(v.m_data))
	{
		int64 a, b;
		if (m_data->ToInt64(a) && v.m_data->ToInt64(b))
		{
			if (op == Variant::VOP_EQ)
			{
				return a == b;
			}
			if (op == Variant::VOP_GT)
			{
				return a > b;
			}
			if (op == Variant::VOP_LT)
			{
				return a < b;
			}
		}
	}
	if (Compare::IsBoolMajic(m_data->MajicNumber()) || Compare::IsBoolMajic(v.m_data->MajicNumber()))
	{
		bool a, b;
		if (m_data->ToBool(a) && v.m_data->ToBool(b))
		{
			if (op == Variant::VOP_EQ)
			{
				return a == b;
			}
			if (op == Variant::VOP_GT)
			{
				return a > b;
			}
			if (op == Variant::VOP_LT)
			{
				return a < b;
			}
		}
	}
	if (Compare::IsDateMajic(m_data->MajicNumber()) || Compare::IsDateMajic(v.m_data->MajicNumber()))
	{
		Date a, b;
		if (m_data->ToDate(a) && v.m_data->ToDate(b))
		{
			if (op == Variant::VOP_EQ)
			{
				return a == b;
			}
			if (op == Variant::VOP_GT)
			{
				return a > b;
			}
			if (op == Variant::VOP_LT)
			{
				return a < b;
			}
		}
	}
	if (Compare::IsDateTimeMajic(m_data->MajicNumber()) || Compare::IsDateTimeMajic(v.m_data->MajicNumber()))
	{
		DateTime a, b;
		if (m_data->ToDateTime(a) && v.m_data->ToDateTime(b))
		{
			if (op == Variant::VOP_EQ)
			{
				return a == b;
			}
			if (op == Variant::VOP_GT)
			{
				return a > b;
			}
			if (op == Variant::VOP_LT)
			{
				return a < b;
			}
		}
	}
	String a, b;
	if (!m_data->ToString(a) || !v.m_data->ToString(b))
	{
		return false;
	}
	if (op == Variant::VOP_EQ)
	{
		return a == b;
	}
	if (op == Variant::VOP_GT)
	{
		return a > b;
	}
	if (op == Variant::VOP_LT)
	{
		return a < b;
	}
	return false;
}

bool Variant::operator ==(const Variant& v) const 
{
	return BinOp(v, Variant::VOP_EQ);
}

bool Variant::operator !=(const Variant& v) const 
{ 
	return !(*this == v);
}

bool Variant::operator >(const Variant& v) const 
{ 
	return BinOp(v, Variant::VOP_GT);
}

bool Variant::operator <(const Variant& v) const 
{ 
	return BinOp(v, Variant::VOP_LT);
}

bool Variant::operator >=(const Variant& v) const 
{ 
	return BinOp(v, Variant::VOP_GT) || BinOp(v, Variant::VOP_EQ);
}

bool Variant::operator <=(const Variant& v) const 
{ 
	return BinOp(v, Variant::VOP_LT) || BinOp(v, Variant::VOP_EQ);
}

RefCountPtr<Variant> Variant::ToVarBool() const
{
	bool z;
	if (m_data->ToBool(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarByte() const
{
	byte z;
	if (m_data->ToByte(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarInt16() const
{
	int16 z;
	if (m_data->ToInt16(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarInt32() const
{
	int32 z;
	if (m_data->ToInt32(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarInt64() const
{
	int64 z;
	if (m_data->ToInt64(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarFloat32() const
{
	float32 z;
	if (m_data->ToFloat32(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarFloat64() const
{
	float64 z;
	if (m_data->ToFloat64(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarDateTime() const
{
	DateTime z;
	if (m_data->ToDateTime(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarDate() const
{
	Date z;
	if (m_data->ToDate(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarDecimal() const
{
	Decimal z;
	if (m_data->ToDecimal(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ToVarString() const
{
	String z;
	if (m_data->ToString(z))
	{
		return VariantPtr(new Variant(z));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Add(const Variant& v) const
{
	if (m_data->IsFloat() || v.m_data->IsFloat())
	{
		float64 a, b;
		if (Convert::ToFloat64(m_data, a) && Convert::ToFloat64(v.m_data, b))
		{
			return VariantPtr(new Variant(a + b));
		}
	}
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a + b));
	}
	
	String a, b;
	Convert::ToString(m_data, a);
	Convert::ToString(v.m_data, b);

	return VariantPtr(new Variant( a + b ));
}

RefCountPtr<Variant> Variant::Sub(const Variant& v) const
{
	if (m_data->IsFloat() || v.m_data->IsFloat())
	{
		float64 a, b;
		if (Convert::ToFloat64(m_data, a) && Convert::ToFloat64(v.m_data, b))
		{
			return VariantPtr(new Variant(a - b));
		}
	}
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a - b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Div(const Variant& v) const
{
	if (m_data->IsFloat() || v.m_data->IsFloat())
	{
		float64 a, b;
		if (Convert::ToFloat64(m_data, a) && Convert::ToFloat64(v.m_data, b))
		{
			return VariantPtr(new Variant(a / b));
		}
	}
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a / b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Mul(const Variant& v) const
{
	if (m_data->IsFloat() || v.m_data->IsFloat())
	{
		float64 a, b;
		if (Convert::ToFloat64(m_data, a) && Convert::ToFloat64(v.m_data, b))
		{
			return VariantPtr(new Variant(a * b));
		}
	}
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a * b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Mod(const Variant& v) const
{
	if (m_data->IsFloat() || v.m_data->IsFloat())
	{
		float64 a, b;
		if (Convert::ToFloat64(m_data, a) && Convert::ToFloat64(v.m_data, b))
		{
			return VariantPtr(new Variant(Math::Remainder(a, b)));
		}
	}
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a % b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Exp(const Variant& v) const
{
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a ^ b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ShiftLeft(const Variant& v) const
{
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a << b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::ShiftRigth(const Variant& v) const
{
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a >> b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::OrBin(const Variant& v) const
{
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a | b));
	}
	bool ba, bb;
	if (Convert::ToBool(m_data, ba) && Convert::ToBool(v.m_data, bb))
	{
		return VariantPtr(new Variant(ba | bb));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::OrLog(const Variant& v) const
{
	bool ba, bb;
	if (Convert::ToBool(m_data, ba) && Convert::ToBool(v.m_data, bb))
	{
		return VariantPtr(new Variant(ba || bb));
	}
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a || b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::AndBin(const Variant& v) const
{
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a & b));
	}
	bool ba, bb;
	if (Convert::ToBool(m_data, ba) && Convert::ToBool(v.m_data, bb))
	{
		return VariantPtr(new Variant(ba & bb));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::AndLog(const Variant& v) const
{
	bool ba, bb;
	if (Convert::ToBool(m_data, ba) && Convert::ToBool(v.m_data, bb))
	{
		return VariantPtr(new Variant(ba && bb));
	}
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a && b));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Xor(const Variant& v) const
{
	if (Convert::CanConvertToInt(m_data) && Convert::CanConvertToInt(v.m_data))
	{
		int64 a, b;
		Convert::ToInt64(m_data, a);
		Convert::ToInt64(v.m_data, b);
		return VariantPtr(new Variant(a ^ b));
	}
	bool ba, bb;
	if (Convert::ToBool(m_data, ba) && Convert::ToBool(v.m_data, bb))
	{
		return VariantPtr(new Variant(ba ^ bb));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Not() const
{
	bool b;
	if (Convert::ToBool(m_data, b))
	{
		return VariantPtr(new Variant(!b));
	}
	if (Convert::CanConvertToInt(m_data))
	{
		int64 a;
		Convert::ToInt64(m_data, a);
		return VariantPtr(new Variant(!a));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Comp() const
{
	if (Convert::CanConvertToInt(m_data))
	{
		int64 a;
		Convert::ToInt64(m_data, a);
		return VariantPtr(new Variant(~a));
	}
	return VariantPtr(new Variant());
}

RefCountPtr<Variant> Variant::Neg() const
{
	if (Convert::CanConvertToInt(m_data))
	{
		int64 a;
		Convert::ToInt64(m_data, a);
		return VariantPtr(new Variant(-a));
	}
	if (Convert::CanConvertToFloat(m_data))
	{
		float64 a;
		Convert::ToFloat64(m_data, a);
		return VariantPtr(new Variant(-a));
	}
	return VariantPtr(new Variant());
}

VariantPtr Variant::Clone() const
{
	VariantPtr v(new Variant(m_data->Clone()));
	return v;
}

#ifdef DEBUG
void Variant::ValidateMem() const
{
	ASSERT_PTR(m_data);
	m_data->ValidateMem();
}

void Variant::CheckMem() const
{
	DEBUG_NOTE_MEM(m_data);
	m_data->CheckMem();
}
#endif
