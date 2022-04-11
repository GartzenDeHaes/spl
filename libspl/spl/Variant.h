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
#ifndef _Variant_h
#define _Variant_h

#include <spl/Convert.h>
#include <spl/IVariant.h>
//#include <spl/interp/IJsObject.h>
#include <spl/Null.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

class IVariantData : public IConvertable
{
public:
	IVariantData();
	virtual ~IVariantData();

	virtual uint32 HashCode() const = 0;
	virtual bool Equals( const IComparable& a ) const = 0;
	virtual int Compare( const IComparable& a ) const = 0;

	virtual bool IsObject() const = 0;
	virtual bool IsFloat() const = 0;

	virtual int32 MajicNumber() const = 0;

	virtual bool ToChar(char& c2) const = 0;
	virtual bool ToBool(bool& c2) const = 0;
	virtual bool ToByte(byte& c2) const = 0;
	virtual bool ToInt16(int16& c2) const = 0;
	virtual bool ToInt32(int32& c2) const = 0;
	virtual bool ToInt64(int64& c2) const = 0;
	virtual bool ToUInt32(uint32& c2) const = 0;
	virtual bool ToUInt64(uint64& c2) const = 0;
	//virtual bool ToBigInteger(const BigInteger& c2) const = 0;
	virtual bool ToFloat32(float32& c2) const = 0;
	virtual bool ToFloat64(float64& c2) const = 0;
	virtual bool ToDateTime(DateTime& c2) const = 0;
	virtual bool ToDate(Date& c2) const = 0;
	virtual bool ToDecimal(Decimal& c2) const = 0;
	virtual bool ToString(String& c2) const = 0;
//	virtual bool ToObject(IJsObject& c2) const = 0;
	
	virtual String TypeName() const = 0;

	virtual IVariantData *Clone() = 0;
	virtual IVariantData *Copy() = 0;
	virtual void *Data() = 0;
};

template<typename T>
class _VariantData : public IVariantData
{
private:
	T m_data;

public:
	_VariantData()
	{
	}

	_VariantData(T data)
	: m_data(data)
	{
	}

	_VariantData(const _VariantData<T>& vd)
	: m_data()
	{
		throw new NotImplementedException("Do not copy construct _VariantData");
	}

	virtual ~_VariantData()
	{
	}

	inline _VariantData<T>& operator =(const _VariantData<T> &v)
	{
		throw new NotImplementedException("Do not assign _VariantData");
	}

	virtual bool Equals( const IComparable& a ) const
	{
		return Compare::Equals(a, m_data);
	}

	virtual int Compare( const IComparable& a ) const
	{
		return static_cast<int>(Compare::Cmp(m_data, a));
	}

	virtual int32 MajicNumber() const
	{
		return Compare::Majic(m_data);
		//return 911 + Compare::Majic(m_data);
	}

	virtual uint32 HashCode() const
	{
		return Math::Hash(m_data);
	}

	virtual bool ToChar(char& c2) const
	{
		return Convert::ToChar(m_data, c2);
	}

	virtual bool ToBool(bool& c2) const
	{
		return Convert::ToBool(m_data, c2);
	}

	virtual bool ToByte(byte& c2) const
	{
		return Convert::ToByte(m_data, c2);
	}

	virtual bool ToInt16(int16& c2) const
	{
		return Convert::ToInt16(m_data, c2);
	}

	virtual bool ToInt32(int32& c2) const
	{
		return Convert::ToInt32(m_data, c2);
	}

	inline int32 ToInt32() const
	{
		int32 i;
		bool ok = ToInt32(i);
		Debug::Assert(ok);
		return i;
	}

	virtual bool ToInt64(int64& c2) const
	{
		return Convert::ToInt64(m_data, c2);
	}

	virtual bool ToUInt32(uint32& c2) const
	{
		return Convert::ToUInt32(m_data, c2);
	}

	virtual bool ToUInt64(uint64& c2) const
	{
		return Convert::ToUInt64(m_data, c2);
	}

	//virtual bool ToBigInteger(const BigInteger& c2) const
	//{
	//}

	virtual bool ToFloat32(float32& c2) const
	{
		return Convert::ToFloat32(m_data, c2);
	}

	virtual bool ToFloat64(float64& c2) const
	{
		return Convert::ToFloat64(m_data, c2);
	}

	virtual bool ToDateTime(DateTime& c2) const
	{
		return Convert::ToDateTime(m_data, c2);
	}

	virtual bool ToDate(Date& c2) const
	{
		return Convert::ToDate(m_data, c2);
	}

	virtual bool ToDecimal(Decimal& c2) const
	{
		return Convert::ToDecimal(m_data, c2);
	}

	virtual bool ToString(String& c2) const
	{
		return Convert::ToString(m_data, c2);
	}

	//virtual bool ToObject(IJsObject& c2) const
	//{
	//	return Convert::ToObject(m_data, c2);
	//}
	
	inline String ToString() const
	{
		String s;
		ToString(s);
		return s;
	}

	virtual bool IsObject() const
	{
		return Convert::IsObject(m_data);
	}

	virtual String TypeName() const
	{
		return Convert::TypeName(m_data);
	}

	virtual IVariantData *Clone()
	{
		//if (IsObject())
		//{
		//	RefCountPtr<IJsObject> *obj = (RefCountPtr<IJsObject> *)&m_data;
		//	return new _VariantData<RefCountPtr<IJsObject> >(obj->Get()->New());
		//}
		//else
		//{
			return new _VariantData<T>(m_data);
		//}
	}
	
	virtual IVariantData *Copy()
	{
		return new _VariantData<T>(m_data);
	}

	inline T Get()
	{
		return m_data;
	}

	inline T& GetRef()
	{
		return m_data;
	}

	virtual void *Data()
	{
		return &m_data;
	}

	virtual bool IsFloat() const
	{
		return Convert::IsFloat(m_data);
	}

#ifdef DEBUG
	void ValidateMem() const
	{
		TypeValidate(m_data);
	}

	void CheckMem() const
	{
		TypeCheckMem(m_data);
	}
#endif
};

class Variant;
typedef RefCountPtr<Variant> VariantPtr;
typedef WeakReference<Variant, VariantPtr> VariantRef;

REGISTER_TYPEOF( 584, VariantPtr );
REGISTER_TYPEOF( 586, VariantRef );
REGISTER_TYPEOF( 606, Array<VariantPtr> );
REGISTER_TYPEOF( 610, Vector<VariantPtr> );

/** @brief A variant type mostly for use in interpreters for variant languages.
 *
 */
class Variant : public IVariant
{
private:
	IVariantData *m_data;

	enum CompareOperator
	{
		VOP_EQ,
		VOP_GT,
		VOP_LT
	};

	bool BinOp(const Variant& b, enum CompareOperator op) const;
	Variant(IVariantData *m_data);
	
public:
	Variant();
	Variant(bool flag);
	Variant(int8 i8);
	Variant(int16 i16);
	Variant(int32 i32);
	Variant(int64 i64);
	Variant(float32 f32);
	Variant(float64 f64);
	Variant(const DateTime& dtm);
	Variant(const Date& dt);
	Variant(const String& str);
	Variant(const String *str);
	Variant(const char *str);
	Variant(const Decimal& dec);
	Variant(const Variant& v);
	Variant(const Undefined& v);
	Variant(const Null& v);
	//Variant(RefCountPtr<IJsObject> jso);
	virtual ~Variant();

	VariantPtr ToVarBool() const;
	VariantPtr ToVarByte() const;
	VariantPtr ToVarInt16() const;
	VariantPtr ToVarInt32() const;
	VariantPtr ToVarInt64() const;
	VariantPtr ToVarFloat32() const;
	VariantPtr ToVarFloat64() const;
	VariantPtr ToVarDateTime() const;
	VariantPtr ToVarDate() const;
	VariantPtr ToVarDecimal() const;
	VariantPtr ToVarString() const;

	virtual bool ToBool() const;
	virtual byte ToByte() const;
	virtual int16 ToInt16() const;
	virtual int32 ToInt32() const;
	virtual int64 ToInt64() const;
	virtual float32 ToFloat32() const;
	virtual float64 ToFloat64() const;
	virtual DateTime ToDateTime() const;
	virtual Date ToDate() const;
	virtual Decimal ToDecimal() const;
	virtual StringPtr ToString() const;
	//virtual RefCountPtr<IJsObject> ToObject() const;

	virtual bool ToChar(char& c2) const;
	virtual bool ToBool(bool& c2) const;
	virtual bool ToByte(byte& c2) const;
	virtual bool ToInt16(int16& c2) const;
	virtual bool ToInt32(int32& c2) const;
	virtual bool ToInt64(int64& c2) const;
	virtual bool ToUInt32(uint32& c2) const;
	virtual bool ToUInt64(uint64& c2) const;
	//virtual bool ToBigInteger(const BigInteger& c2) const;
	virtual bool ToFloat32(float32& c2) const;
	virtual bool ToFloat64(float64& c2) const;
	virtual bool ToDateTime(DateTime& c2) const;
	virtual bool ToDate(Date& c2) const;
	virtual bool ToDecimal(Decimal& c2) const;
	virtual bool ToString(String& c2) const;

	virtual bool Equals( const IComparable& a ) const;
	bool Equals( const Variant& v ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;
	virtual uint32 HashCode() const;

	bool IsObject() const;

	VariantPtr ParseInt() const;
	VariantPtr ParseFloat() const;

	String TypeName() const;

	inline String TypeNameInner() const
	{
		return m_data->TypeName();
	}
	
	inline VariantPtr ToRValue()
	{
		return RefCountPtr<Variant>(new Variant(*this));
	}

	inline void Clear()
	{
		delete m_data;
		m_data = new _VariantData<Undefined>(Undefined());
	}

	Variant& operator =(const Variant& v);
	Variant& operator =(const int32 i);
	Variant& operator =(const int64 i);
	Variant& operator =(const float64 d);
	Variant& operator =(const String& s);
	Variant& operator =(const char *s);
	Variant& operator =(const DateTime& s);
	Variant& operator =(const Date& s);
	Variant& operator =(const Decimal& s);
	//Variant& operator =(RefCountPtr<IJsObject> s);

	bool operator ==(const Variant& v) const;
	bool operator !=(const Variant& v) const;
	bool operator >(const Variant& v) const;
	bool operator <(const Variant& v) const;
	bool operator >=(const Variant& v) const;
	bool operator <=(const Variant& v) const;

	VariantPtr Add(const Variant& v) const;
	VariantPtr Sub(const Variant& v) const;
	VariantPtr Div(const Variant& v) const;
	VariantPtr Mul(const Variant& v) const;
	VariantPtr Mod(const Variant& v) const;
	VariantPtr Exp(const Variant& v) const;
	VariantPtr ShiftLeft(const Variant& v) const;
	VariantPtr ShiftRigth(const Variant& v) const;
	VariantPtr OrBin(const Variant& v) const;
	VariantPtr OrLog(const Variant& v) const;
	VariantPtr AndBin(const Variant& v) const;
	VariantPtr AndLog(const Variant& v) const;
	VariantPtr Xor(const Variant& v) const;
	VariantPtr Not() const;
	VariantPtr Comp() const;
	VariantPtr Neg() const;

	inline Variant operator +(const Variant& v) const { return *Add(v); }
	inline Variant operator -(const Variant& v) const { return *Sub(v); }
	inline Variant operator /(const Variant& v) const { return *Div(v); }
	inline Variant operator *(const Variant& v) const { return *Mul(v); }
	inline Variant operator %(const Variant& v) const { return *Mod(v); }
	inline Variant operator ^(const Variant& v) const { return *Exp(v); }
	inline Variant operator <<(const Variant& v) const { return *ShiftLeft(v); }
	inline Variant operator >>(const Variant& v) const { return *ShiftRigth(v); }
	inline Variant operator |(const Variant& v) const { return *OrBin(v); }
	inline Variant operator ||(const Variant& v) const { return *OrLog(v); }
	inline Variant operator &(const Variant& v) const { return *AndBin(v); }
	inline Variant operator &&(const Variant& v) const { return *AndLog(v); }
	inline Variant operator !() const { return *Not(); }
	inline Variant operator ~() const { return *Comp(); }
	inline Variant operator -() const { return *Neg(); }

	VariantPtr Clone() const;

	inline bool IsUndefined() const { return m_data->MajicNumber() == UNDEFINED_MAJIC; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 582, Variant );
REGISTER_TYPEOF( 600, Array<Variant> );
REGISTER_TYPEOF( 604, Vector<Variant> );

/** @} */
}
#endif

