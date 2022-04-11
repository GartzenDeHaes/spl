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
#ifndef _convert_h
#define _convert_h

#include <spl/IConvertable.h>
#include <spl/Null.h>
#include <spl/Undefined.h>
//#include <spl/interp/IJsObject.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

/** @brief Helper for Variant and templates to perform automatic type conversion.
 *
 */
class Convert
{
private:
	Convert() {}
	Convert(const Convert& c) {}
	Convert& operator =(const Convert& c) { return *this; }

public:
	inline static bool IsFloat(const Null& c) { return false; }
	inline static bool IsFloat(const Null *c) { return false; }
	inline static bool IsFloat(const Undefined& c) { return false; }
	inline static bool IsFloat(const Undefined *c) { return false; }
	inline static bool IsFloat(const IConvertable *c) { String typeName = c->TypeName(); return typeName == "float" || typeName == "double"; }
	inline static bool IsFloat(const IConvertable& c) { String typeName = c.TypeName(); return typeName == "float" || typeName == "double"; }
	inline static bool IsFloat(char c) { return false; }
	inline static bool IsFloat(bool c) { return false; }
	inline static bool IsFloat(byte c) { return false; }
	inline static bool IsFloat(int16 c) { return false; }
	inline static bool IsFloat(int32 c) { return false; }
	inline static bool IsFloat(int64 c) { return false; }
	inline static bool IsFloat(uint32 c) { return false; }
	inline static bool IsFloat(uint64 c) { return false; }
	inline static bool IsFloat(const BigInteger& c) { return false; }
	inline static bool IsFloat(float32 c) { return true;}
	inline static bool IsFloat(float64 c) { return true; }
	inline static bool IsFloat(const Decimal& c) { return true; }
	inline static bool IsFloat(const DateTime& c) { return false; }
	inline static bool IsFloat(const Date& c) { return false; }
	inline static bool IsFloat(const String& c) { return false; }
	//inline static bool IsFloat(const IJsObject& c) { return false; }
	//inline static bool IsFloat(const IJsObject* c) { return false; }

	inline static bool IsInt(const Null& c) { return false; }
	inline static bool IsInt(const Null *c) { return false; }
	inline static bool IsInt(const Undefined& c) { return false; }
	inline static bool IsInt(const Undefined *c) { return false; }
	inline static bool IsInt(const IConvertable *c) { String typeName = c->TypeName(); return typeName == "int32" || typeName == "int32" || typeName == "int16"; }
	inline static bool IsInt(const IConvertable& c) { String typeName = c.TypeName(); return typeName == "int32" || typeName == "int32" || typeName == "int16"; }
	inline static bool IsInt(char c) { return false; }
	inline static bool IsInt(bool c) { return false; }
	inline static bool IsInt(byte c) { return false; }
	inline static bool IsInt(int16 c) { return true; }
	inline static bool IsInt(int32 c) { return true; }
	inline static bool IsInt(int64 c) { return true; }
	inline static bool IsInt(uint32 c) { return false; }
	inline static bool IsInt(uint64 c) { return false; }
	inline static bool IsInt(const BigInteger& c) { return false; }
	inline static bool IsInt(float32 c) { return true;}
	inline static bool IsInt(float64 c) { return true; }
	inline static bool IsInt(const Decimal& c) { return true; }
	inline static bool IsInt(const DateTime& c) { return false; }
	inline static bool IsInt(const Date& c) { return false; }
	inline static bool IsInt(const String& c) { return false; }
	//nline static bool IsInt(const IJsObject& c) { return false; }
	//inline static bool IsInt(const IJsObject* c) { return false; }

	inline static bool IsObject(const Null& c) { return false; }
	inline static bool IsObject(const Null *c) { return false; }
	inline static bool IsObject(const Undefined& c) { return false; }
	inline static bool IsObject(const Undefined *c) { return false; }
	inline static bool IsObject(const IConvertable *c) { return false; }
	inline static bool IsObject(const IConvertable& c) { return false; }
	inline static bool IsObject(char c) { return false; }
	inline static bool IsObject(bool c) { return false; }
	inline static bool IsObject(byte c) { return false; }
	inline static bool IsObject(int16 c) { return false; }
	inline static bool IsObject(int32 c) { return false; }
	inline static bool IsObject(int64 c) { return false; }
	inline static bool IsObject(uint32 c) { return false; }
	inline static bool IsObject(uint64 c) { return false; }
	inline static bool IsObject(const BigInteger& c) { return false; }
	inline static bool IsObject(float32 c) { return false;}
	inline static bool IsObject(float64 c) { return false; }
	inline static bool IsObject(const Decimal& c) { return false; }
	inline static bool IsObject(const DateTime& c) { return false; }
	inline static bool IsObject(const Date& c) { return false; }
	inline static bool IsObject(const String& c) { return false; }
	//inline static bool IsObject(const IJsObject& c) { return true; }
	//inline static bool IsObject(const IJsObject* c) { return true; }

	inline static bool CanConvertToFloat(const Null& c) { return false; }
	inline static bool CanConvertToFloat(const Null *c) { return false; }
	inline static bool CanConvertToFloat(const Undefined& c) { return false; }
	inline static bool CanConvertToFloat(const Undefined *c) { return false; }
	inline static bool CanConvertToFloat(const IConvertable *c) { float64 c2; return c->ToFloat64(c2); }
	inline static bool CanConvertToFloat(const IConvertable& c) { float64 c2; return c.ToFloat64(c2); }
	inline static bool CanConvertToFloat(char c) { return true; }
	inline static bool CanConvertToFloat(bool c) { return false; }
	inline static bool CanConvertToFloat(byte c) { return true; }
	inline static bool CanConvertToFloat(int16 c) { return true; }
	inline static bool CanConvertToFloat(int32 c) { return true; }
	inline static bool CanConvertToFloat(int64 c) { return true; }
	inline static bool CanConvertToFloat(uint32 c) { return true; }
	inline static bool CanConvertToFloat(uint64 c) { return true; }
	inline static bool CanConvertToFloat(const BigInteger& c) { return false; }
	inline static bool CanConvertToFloat(float32 c) { return true;}
	inline static bool CanConvertToFloat(float64 c) { return true; }
	inline static bool CanConvertToFloat(const Decimal& c) { return true; }
	inline static bool CanConvertToFloat(const DateTime& c) { return false; }
	inline static bool CanConvertToFloat(const Date& c) { return false; }
	inline static bool CanConvertToFloat(const String& c) { return false; }
	//inline static bool CanConvertToFloat(const IJsObject& c) { return false; }
	//inline static bool CanConvertToFloat(const IJsObject* c) { return false; }

	inline static bool CanConvertToInt(const Null& c) { return false; }
	inline static bool CanConvertToInt(const Null *c) { return false; }
	inline static bool CanConvertToInt(const Undefined& c) { return false; }
	inline static bool CanConvertToInt(const Undefined *c) { return false; }
	inline static bool CanConvertToInt(const IConvertable *c) { int64 c2; return c->ToInt64(c2); }
	inline static bool CanConvertToInt(const IConvertable& c) { int64 c2; return c.ToInt64(c2); }
	inline static bool CanConvertToInt(char c) { return true; }
	inline static bool CanConvertToInt(bool c) { return false; }
	inline static bool CanConvertToInt(byte c) { return true; }
	inline static bool CanConvertToInt(int16 c) { return true; }
	inline static bool CanConvertToInt(int32 c) { return true; }
	inline static bool CanConvertToInt(int64 c) { return true; }
	inline static bool CanConvertToInt(uint32 c) { return true; }
	inline static bool CanConvertToInt(uint64 c) { return false; }
	inline static bool CanConvertToInt(const BigInteger& c) { return false; }
	inline static bool CanConvertToInt(float32 c) { return true;}
	inline static bool CanConvertToInt(float64 c) { return true; }
	inline static bool CanConvertToInt(const Decimal& c) { return true; }
	inline static bool CanConvertToInt(const DateTime& c) { return false; }
	inline static bool CanConvertToInt(const Date& c) { return false; }
	inline static bool CanConvertToInt(const String& c) { return false; }
	//inline static bool CanConvertToInt(const IJsObject& c) { return false; }
	//inline static bool CanConvertToInt(const IJsObject* c) { return false; }

	inline static bool ToChar(const Null& c, char& c2) { c2 = 0; return true; }
	inline static bool ToChar(const Null *c, char& c2) { c2 = 0; return true; }
	inline static bool ToChar(const Undefined& c, char& c2) { return false; }
	inline static bool ToChar(const Undefined *c, char& c2) { return false; }
	inline static bool ToChar(const IConvertable *c, char& c2) { return c->ToChar(c2); }
	inline static bool ToChar(const IConvertable& c, char& c2) { return c.ToChar(c2); }
	inline static bool ToChar(char c, char& c2) { c2 = c; return true; }
	inline static bool ToChar(bool c, char& c2) { c2 = c ? 'T' : 'F'; return true; }
	inline static bool ToChar(byte c, char& c2) { c2 = (char)c; return true; }
	inline static bool ToChar(int16 c, char& c2) { c2 = (char)c; return true; }
	inline static bool ToChar(int32 c, char& c2) { c2 = (char)c; return true; }
	inline static bool ToChar(int64 c, char& c2) { c2 = (char)c; return true; }
	inline static bool ToChar(uint32 c, char& c2) { c2 = (char)c; return true; }
	inline static bool ToChar(uint64 c, char& c2) { c2 = (char)c; return true; }
	inline static bool ToChar(const BigInteger& c, char& c2) { return false; }
	inline static bool ToChar(float32 c, char& c2) { c2 = (char)(int)c; return true;}
	inline static bool ToChar(float64 c, char& c2) { c2 = (char)(int)c; return true; }
	inline static bool ToChar(const DateTime& c, char& c2) { return false; }
	inline static bool ToChar(const Date& c, char& c2) { return false; }
	inline static bool ToChar(const String& c, char& c2) { c2 = c.Length() == 0 ? '\0' : c.CharAt(0); return true; }
	inline static bool ToChar(const Decimal& c, char& c2) { return false; }
	//inline static bool ToChar(const IJsObject& c, char& c2) { return false; }
	//inline static bool ToChar(const IJsObject* c, char& c2) { return false; }

	inline static bool ToBool(const Null& c, bool& c2) { c2 = false; return true; }
	inline static bool ToBool(const Null *c, bool& c2) { c2 = false; return true; }
	inline static bool ToBool(const Undefined& c, bool& c2) { return false; }
	inline static bool ToBool(const Undefined *c, bool& c2) { return false; }
	inline static bool ToBool(const IConvertable *c, bool& c2) { return c->ToBool(c2); }
	inline static bool ToBool(const IConvertable& c, bool& c2) { return c.ToBool(c2); }
	inline static bool ToBool(char c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(bool c, bool& c2) { c2 = c; return true; }
	inline static bool ToBool(byte c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(int16 c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(int32 c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(int64 c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(uint32 c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(uint64 c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(BigInteger& c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(float32 c, bool& c2) { c2 = c != 0; return true;}
	inline static bool ToBool(float64 c, bool& c2) { c2 = c != 0; return true; }
	inline static bool ToBool(const DateTime& c, bool& c2) { return false; }
	inline static bool ToBool(const Date& c, bool& c2) { return false; }
	inline static bool ToBool(const String& c, bool& b) { b = c.EqualsIgnoreCase("true"); return true; }
	inline static bool ToBool(const Decimal& c, bool& c2) { return false; }
	//inline static bool ToBool(const IJsObject& c, bool& c2) { c2 = true; return true; }
	//inline static bool ToBool(const IJsObject* c, bool& c2) { c2 = true; return true; }

	inline static bool ToByte(const Null& c, byte& c2) { c2 = 0; return true; }
	inline static bool ToByte(const Null *c, byte& c2) { c2 = 0; return true; }
	inline static bool ToByte(const Undefined& c, byte& c2) { return false; }
	inline static bool ToByte(const Undefined *c, byte& c2) { return false; }
	inline static bool ToByte(const IConvertable *c, byte& c2) { return c->ToByte(c2); }
	inline static bool ToByte(const IConvertable& c, byte& c2) { return c.ToByte(c2); }
	inline static bool ToByte(char c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(bool c, byte& c2) { c2 = c ? 1 : 0; return true; }
	inline static bool ToByte(byte c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(int16 c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(int32 c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(int64 c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(uint32 c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(uint64 c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(BigInteger& c, byte& c2) { return false; }
	inline static bool ToByte(float32 c, byte& c2) { c2 = (int8)c; return true;}
	inline static bool ToByte(float64 c, byte& c2) { c2 = (int8)c; return true; }
	inline static bool ToByte(const DateTime& c, byte& c2) { return false; }
	inline static bool ToByte(const Date& c, byte& c2) { return false; }
	inline static bool ToByte(const String& c, byte& c2) 
	{ 
		if (UInt32::IsInt(c))
		{
			c2 = (byte)UInt32::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToByte(const Decimal& c, byte& c2) { return false; }
//	inline static bool ToByte(const IJsObject& c, byte& c2) { return false; }
//	inline static bool ToByte(const IJsObject* c, byte& c2) { return false; }

	inline static bool ToInt16(const Null& c, int16& c2) { c2 = 0; return true; }
	inline static bool ToInt16(const Null *c, int16& c2) { c2 = 0; return true; }
	inline static bool ToInt16(const Undefined& c, int16& c2) { return false; }
	inline static bool ToInt16(const Undefined *c, int16& c2) { return false; }
	inline static bool ToInt16(const IConvertable *c, int16& c2) { return c->ToInt16(c2); }
	inline static bool ToInt16(const IConvertable& c, int16& c2) { return c.ToInt16(c2); }
	inline static bool ToInt16(char c, int16& c2) { c2 = c; return true; }
	inline static bool ToInt16(bool c, int16& c2) { c2 = c ? 1 : 0; return true; }
	inline static bool ToInt16(byte c, int16& c2) { c2 = (int16)c; return true; }
	inline static bool ToInt16(int16 c, int16& c2) { c2 = (int16)c; return true; }
	inline static bool ToInt16(int32 c, int16& c2) { c2 = (int16)c; return true; }
	inline static bool ToInt16(int64 c, int16& c2) { c2 = (int16)c; return true; }
	inline static bool ToInt16(uint32 c, int16& c2) { c2 = (int16)c; return true; }
	inline static bool ToInt16(uint64 c, int16& c2) { c2 = (int16)c; return true; }
	inline static bool ToInt16(BigInteger& c, int16& c2) { return false; }
	inline static bool ToInt16(float32 c, int16& c2) { c2 = (int16)c; return true;}
	inline static bool ToInt16(float64 c, int16& c2) { c2 = (int16)c; return true; }
	inline static bool ToInt16(const DateTime& c, int16& c2) { return false; }
	inline static bool ToInt16(const Date& c, int16& c2) { return false; }
	inline static bool ToInt16(const String& c, int16& c2)
	{ 
		if (Int32::IsInt(c))
		{
			c2 = (int16)Int32::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToInt16(const Decimal& c, int16& c2) { c2 = (int16)c.ToInt(); return true; }
//	inline static bool ToInt16(const IJsObject& c, int16& c2) { return false; }
//	inline static bool ToInt16(const IJsObject* c, int16& c2) { return false; }

	inline static bool ToInt32(const Null& c, int32& c2) { c2 = 0; return true; }
	inline static bool Toint32(const Null *c, int32& c2) { c2 = 0; return true; }
	inline static bool ToInt32(const Undefined& c, int32& c2) { return false; }
	inline static bool Toint32(const Undefined *c, int32& c2) { return false; }
	inline static bool ToInt32(const IConvertable *c, int32& c2) { return c->ToInt32(c2); }
	inline static bool ToInt32(const IConvertable& c, int32& c2) { return c.ToInt32(c2); }
	inline static bool ToInt32(char c, int32& c2) { c2 = c; return true; }
	inline static bool ToInt32(bool c, int32& c2) { c2 = c ? 1 : 0; return true; }
	inline static bool ToInt32(byte c, int32& c2) { c2 = (int32)c; return true; }
	inline static bool ToInt32(int16 c, int32& c2) { c2 = (int32)c; return true; }
	inline static bool ToInt32(int32 c, int32& c2) { c2 = (int32)c; return true; }
	inline static bool ToInt32(int64 c, int32& c2) { c2 = (int32)c; return true; }
	inline static bool ToInt32(uint32 c, int32& c2) { c2 = (int32)c; return true; }
	inline static bool ToInt32(uint64 c, int32& c2) { c2 = (int32)c; return true; }
	inline static bool ToInt32(BigInteger& c, int32& c2) { c2 = (int32)c.ToInt(); return true; }
	inline static bool ToInt32(float32 c, int32& c2) { c2 = (int32)c; return true;}
	inline static bool ToInt32(float64 c, int32& c2) { c2 = (int32)c; return true; }
	inline static bool ToInt32(const DateTime& c, int32& c2) { return false; }
	inline static bool ToInt32(const Date& c, int32& c2) { c2 = c.ToRevInt(); return true; }
	inline static bool ToInt32(const String& c, int32& c2)
	{ 
		if (Int32::IsInt(c))
		{
			c2 = Int32::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToInt32(const Decimal& c, int32& c2) { c2 = c.ToInt(); return true; }
//	inline static bool ToInt32(const IJsObject& c, int32& c2) { return false; }
//	inline static bool ToInt32(const IJsObject* c, int32& c2) { return false; }

	inline static bool ToInt64(const Null& c, int64& c2) { c2 = 0; return true; }
	inline static bool ToInt64(const Null *c, int64& c2) { c2 = 0; return true; }
	inline static bool ToInt64(const Undefined& c, int64& c2) { return false; }
	inline static bool ToInt64(const Undefined *c, int64& c2) { return false; }
	inline static bool ToInt64(const IConvertable *c, int64& c2) { return c->ToInt64(c2); }
	inline static bool ToInt64(const IConvertable& c, int64& c2) { return c.ToInt64(c2); }
	inline static bool ToInt64(char c, int64& c2) { c2 = c; return true; }
	inline static bool ToInt64(bool c, int64& c2) { c2 = c ? 1 : 0; return true; }
	inline static bool ToInt64(byte c, int64& c2) { c2 = (int64)c; return true; }
	inline static bool ToInt64(int16 c, int64& c2) { c2 = (int64)c; return true; }
	inline static bool ToInt64(int32 c, int64& c2) { c2 = (int64)c; return true; }
	inline static bool ToInt64(int64 c, int64& c2) { c2 = (int64)c; return true; }
	inline static bool ToInt64(uint32 c, int64& c2) { c2 = (int64)c; return true; }
	inline static bool ToInt64(uint64 c, int64& c2) { c2 = (int64)c; return true; }
	inline static bool ToInt64(BigInteger& c, int64& c2) { c2 = c.ToInt(); return true; }
	inline static bool ToInt64(float32 c, int64& c2) { c2 = (int)c; return true;}
	inline static bool ToInt64(float64 c, int64& c2) { c2 = (int)c; return true; }
	inline static bool ToInt64(const DateTime& c, int64& c2) { return false; }
	inline static bool ToInt64(const Date& c, int64& c2) { c2 = c.ToRevInt(); return true; }
	inline static bool ToInt64(const String& c, int64& c2)
	{ 
		if (Int64::IsInt(c))
		{
			c2 = Int64::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToInt64(const Decimal& c, int64& c2) { c2 = c.ToInt(); return true; }
//	inline static bool ToInt64(const IJsObject& c, int64& c2) { return false; }
//	inline static bool ToInt64(const IJsObject* c, int64& c2) { return false; }

	inline static bool ToUInt32(const Null& c, uint32& c2) { c2 = 0; return true; }
	inline static bool ToUInt32(const Null *c, uint32& c2) { c2 = 0; return true; }
	inline static bool ToUInt32(const Undefined& c, uint32& c2) { return false; }
	inline static bool ToUInt32(const Undefined *c, uint32& c2) { return false; }
	inline static bool ToUInt32(const IConvertable *c, uint32& c2) { return c->ToUInt32(c2); }
	inline static bool ToUInt32(const IConvertable& c, uint32& c2) { return c.ToUInt32(c2); }
	inline static bool ToUInt32(char c, uint32& c2) { c2 = c; return true; }
	inline static bool ToUInt32(bool c, uint32& c2) { c2 = c ? 1 : 0; return true; }
	inline static bool ToUInt32(byte c, uint32& c2) { c2 = (uint32)c; return true; }
	inline static bool ToUInt32(int16 c, uint32& c2) { c2 = (uint32)c; return true; }
	inline static bool ToUInt32(int32 c, uint32& c2) { c2 = (uint32)c; return true; }
	inline static bool ToUInt32(int64 c, uint32& c2) { c2 = (uint32)c; return true; }
	inline static bool ToUInt32(uint32 c, uint32& c2) { c2 = (uint32)c; return true; }
	inline static bool ToUInt32(uint64 c, uint32& c2) { c2 = (uint32)c; return true; }
	inline static bool ToUInt32(BigInteger& c, uint32& c2) { c2 = (uint32)c.ToInt(); return true; }
	inline static bool ToUInt32(float32 c, uint32& c2) { c2 = (uint32)c; return true;}
	inline static bool ToUInt32(float64 c, uint32& c2) { c2 = (uint32)c; return true; }
	inline static bool ToUInt32(const DateTime& c, uint32& c2) { return false; }
	inline static bool ToUInt32(const Date& c, uint32& c2) { c2 = c.ToRevInt(); return true; }
	inline static bool ToUInt32(const String& c, uint32& c2)
	{ 
		if (UInt32::IsInt(c))
		{
			c2 = UInt32::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToUInt32(const Decimal& c, uint32& c2) { c2 = c.ToInt(); return true; }
//	inline static bool ToUInt32(const IJsObject& c, uint32& c2) { return false; }
//	inline static bool ToUInt32(const IJsObject* c, uint32& c2) { return false; }

	inline static bool ToUInt64(const Null& c, uint64& c2) { c2 = 0; return true; }
	inline static bool ToUInt64(const Null *c, uint64& c2) { c2 = 0; return true; }
	inline static bool ToUInt64(const Undefined& c, uint64& c2) { return false; }
	inline static bool ToUInt64(const Undefined *c, uint64& c2) { return false; }
	inline static bool ToUInt64(const IConvertable *c, uint64& c2) { return c->ToUInt64(c2); }
	inline static bool ToUInt64(const IConvertable& c, uint64& c2) { return c.ToUInt64(c2); }
	inline static bool ToUInt64(char c, uint64& c2) { c2 = c; return true; }
	inline static bool ToUInt64(bool c, uint64& c2) { c2 = c ? 1 : 0; return true; }
	inline static bool ToUInt64(byte c, uint64& c2) { c2 = (uint64)c; return true; }
	inline static bool ToUInt64(int16 c, uint64& c2) { c2 = (uint64)c; return true; }
	inline static bool ToUInt64(int32 c, uint64& c2) { c2 = (uint64)c; return true; }
	inline static bool ToUInt64(int64 c, uint64& c2) { c2 = (uint64)c; return true; }
	inline static bool ToUInt64(uint32 c, uint64& c2) { c2 = (uint64)c; return true; }
	inline static bool ToUInt64(uint64 c, uint64& c2) { c2 = (uint64)c; return true; }
	inline static bool ToUInt64(BigInteger& c, uint64& c2) { c2 = c.ToInt(); return true; }
	inline static bool ToUInt64(float32 c, uint64& c2) { c2 = (uint64)c; return true;}
	inline static bool ToUInt64(float64 c, uint64& c2) { c2 = (uint64)c; return true; }
	inline static bool ToUInt64(const DateTime& c, uint64& c2) { return false; }
	inline static bool ToUInt64(const Date& c, uint64& c2) { c2 = c.ToRevInt(); return true; }
	inline static bool ToUInt64(const String& c, uint64& c2)
	{ 
		if (UInt64::IsInt(c))
		{
			c2 = UInt64::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToUInt64(const Decimal& c, uint64& c2) { c2 = c.ToInt(); return true; }
//	inline static bool ToUInt64(const IJsObject& c, uint64& c2) { return false; }
//	inline static bool ToUInt64(const IJsObject* c, uint64& c2) { return false; }

	//inline static bool ToBigInteger(const Null& c, BigInteger& c2) { c2 = 0; return true; }
	//inline static bool ToBigInteger(const Null *c, BigInteger& c2) { c2 = 0; return true; }
	//inline static bool ToBigInteger(const Undefined& c, BigInteger& c2) { return false; }
	//inline static bool ToBigInteger(const Undefined *c, BigInteger& c2) { return false; }
	//inline static bool ToBigInteger(const IConvertable *c, BigInteger& c2) { return c->ToBigInteger(c2); }
	//inline static bool ToBigInteger(const IConvertable& c, BigInteger& c2) { return c.ToBigInteger(c2); }
	//inline static bool ToBigInteger(char c, BigInteger& c2) { c2 = c; return true; }
	//inline static bool ToBigInteger(bool c, BigInteger& c2) { c2 = c ? 0 : 1; return true; }
	//inline static bool ToBigInteger(byte c, BigInteger& c2) { c2 = (int64)c; return true; }
	//inline static bool ToBigInteger(int16 c, BigInteger& c2) { c2 = (int64)c; return true; }
	//inline static bool ToBigInteger(int32 c, BigInteger& c2) { c2 = (int64)c; return true; }
	//inline static bool ToBigInteger(int64 c, BigInteger& c2) { c2 = (int64)c; return true; }
	//inline static bool ToBigInteger(uint32 c, BigInteger& c2) { c2 = (int64)c; return true; }
	//inline static bool ToBigInteger(uint64 c, BigInteger& c2) { c2 = (int64)c; return true; }
	//inline static bool ToBigInteger(BigInteger& c, BigInteger& c2) { c2 = c; return true; }
	//inline static bool ToBigInteger(float32 c, BigInteger& c2) { c2 = (int64)c; return true;}
	//inline static bool ToBigInteger(float64 c, BigInteger& c2) { c2 = (int64)c; return true; }
	//inline static bool ToBigInteger(const DateTime& c, BigInteger& c2) { return false; }
	//inline static bool ToBigInteger(const Date& c, BigInteger& c2) { c2 = c.ToRevInt(); return true; }
	//inline static bool ToBigInteger(const String& c, BigInteger& c2)
	//{ 
	//	if (Int64::IsInt(c))
	//	{
	//		c2 = Int64::Parse(c);
	//		return true;
	//	}
	//	return false;
	//}
	//inline static bool ToBigInteger(const Decimal& c, BigInteger& c2) { c2 = c.ToInt(); return true; }
	//inline static bool ToBigInteger(const IJsObject& c, begInteger& c2) { return false; }

	inline static bool ToFloat32(const Null& c, float32& c2) { c2 = 0; return true; }
	inline static bool ToFloat32(const Null *c, float32& c2) { c2 = 0; return true; }
	inline static bool ToFloat32(const Undefined& c, float32& c2) { return false; }
	inline static bool ToFloat32(const Undefined *c, float32& c2) { return false; }
	inline static bool ToFloat32(const IConvertable *c, float32& c2) { return c->ToFloat32(c2); }
	inline static bool ToFloat32(const IConvertable& c, float32& c2) { return c.ToFloat32(c2); }
	inline static bool ToFloat32(char c, float32& c2) { c2 = c; return true; }
	inline static bool ToFloat32(bool c, float32& c2) { c2 = c ? 1.0f : 0.0f; return true; }
	inline static bool ToFloat32(byte c, float32& c2) { c2 = (float32)c; return true; }
	inline static bool ToFloat32(int16 c, float32& c2) { c2 = (float32)c; return true; }
	inline static bool ToFloat32(int32 c, float32& c2) { c2 = (float32)c; return true; }
	inline static bool ToFloat32(int64 c, float32& c2) { c2 = (float32)c; return true; }
	inline static bool ToFloat32(uint32 c, float32& c2) { c2 = (float32)c; return true; }
	inline static bool ToFloat32(uint64 c, float32& c2) { c2 = (float32)c; return true; }
	inline static bool ToFloat32(BigInteger& c, float32& c2) { c2 = (float32)c.ToInt(); return true; }
	inline static bool ToFloat32(float32 c, float32& c2) { c2 = (float32)c; return true;}
	inline static bool ToFloat32(float64 c, float32& c2) { c2 = (float32)c; return true; }
	inline static bool ToFloat32(const DateTime& c, float32& c2) { return false; }
	inline static bool ToFloat32(const Date& c, float32& c2) { c2 = (float32)c.ToRevInt(); return true; }
	inline static bool ToFloat32(const String& c, float32& c2)
	{ 
		if (Double::IsDouble(c))
		{
			c2 = (float32)Double::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToFloat32(const Decimal& c, float32& c2) { c2 = (float32)c.ToDouble(); return true; }
//	inline static bool ToFloat32(const IJsObject& c, float32& c2) { return false; }
//	inline static bool ToFloat32(const IJsObject* c, float32& c2) { return false; }

	inline static bool ToFloat64(const Null& c, float64& c2) { c2 = 0; return true; }
	inline static bool ToFloat64(const Null *c, float64& c2) { c2 = 0; return true; }
	inline static bool ToFloat64(const Undefined& c, float64& c2) { return false; }
	inline static bool ToFloat64(const Undefined *c, float64& c2) { return false; }
	inline static bool ToFloat64(const IConvertable *c, float64& c2) { return c->ToFloat64(c2); }
	inline static bool ToFloat64(const IConvertable& c, float64& c2) { return c.ToFloat64(c2); }
	inline static bool ToFloat64(char c, float64& c2) { c2 = c; return true; }
	inline static bool ToFloat64(bool c, float64& c2) { c2 = c ? 1.0 : 0.0; return true; }
	inline static bool ToFloat64(byte c, float64& c2) { c2 = (float64)c; return true; }
	inline static bool ToFloat64(int16 c, float64& c2) { c2 = (float64)c; return true; }
	inline static bool ToFloat64(int32 c, float64& c2) { c2 = (float64)c; return true; }
	inline static bool ToFloat64(int64 c, float64& c2) { c2 = (float64)c; return true; }
	inline static bool ToFloat64(uint32 c, float64& c2) { c2 = (float64)c; return true; }
	inline static bool ToFloat64(uint64 c, float64& c2) { c2 = (float64)c; return true; }
	inline static bool ToFloat64(BigInteger& c, float64& c2) { c2 = (float64)c.ToInt(); return true; }
	inline static bool ToFloat64(float32 c, float64& c2) { c2 = (float64)c; return true;}
	inline static bool ToFloat64(float64 c, float64& c2) { c2 = (float64)c; return true; }
	inline static bool ToFloat64(const DateTime& c, float64& c2) { return false; }
	inline static bool ToFloat64(const Date& c, float64& c2) { c2 = c.ToRevInt(); return true; }
	inline static bool ToFloat64(const String& c, float64& c2)
	{ 
		if (Double::IsDouble(c))
		{
			c2 = Double::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToFloat64(const Decimal& c, float64& c2) { c2 = c.ToDouble(); return true; }
//	inline static bool ToFloat64(const IJsObject& c, float64& c2) { return false; }
//	inline static bool ToFloat64(const IJsObject* c, float64& c2) { return false; }

	inline static bool ToDateTime(const Null& c, DateTime& c2) { return false; }
	inline static bool ToDateTime(const Null *c, DateTime& c2) { return false; }
	inline static bool ToDateTime(const Undefined& c, DateTime& c2) { return false; }
	inline static bool ToDateTime(const Undefined *c, DateTime& c2) { return false; }
	inline static bool ToDateTime(const IConvertable *c, DateTime& c2) { return c->ToDateTime(c2); }
	inline static bool ToDateTime(const IConvertable& c, DateTime& c2) { return c.ToDateTime(c2); }
	inline static bool ToDateTime(char c, DateTime& c2) { return false; }
	inline static bool ToDateTime(bool c, DateTime& c2) { return false; }
	inline static bool ToDateTime(byte c, DateTime& c2) { return false; }
	inline static bool ToDateTime(int16 c, DateTime& c2) { return false; }
	inline static bool ToDateTime(int32 c, DateTime& c2) { return false; }
	inline static bool ToDateTime(int64 c, DateTime& c2) { return false; }
	inline static bool ToDateTime(uint32 c, DateTime& c2) { return false; }
	inline static bool ToDateTime(uint64 c, DateTime& c2) { return false; }
	inline static bool ToDateTime(BigInteger& c, DateTime& c2) { return false; }
	inline static bool ToDateTime(float32 c, DateTime& c2) { return false; }
	inline static bool ToDateTime(float64 c, DateTime& c2) { return false; }
	inline static bool ToDateTime(const DateTime& c, DateTime& c2) { c2 = c; return true; }
	inline static bool ToDateTime(Date& c, DateTime& c2) { c2 = DateTime(c.ToSysTime()); return true; }
	inline static bool ToDateTime(const String& c, DateTime& c2) 
	{ 
		if (DateTime::IsDateTime(c))
		{
			c2 = DateTime::Parse(c); 
			return true; 
		}
		return false;
	}
	inline static bool ToDateTime(const Decimal& c, DateTime& c2) { return false; }
//	inline static bool ToDateTime(const IJsObject& c, DateTime& c2) { return false; }
//	inline static bool ToDateTime(const IJsObject* c, DateTime& c2) { return false; }

	inline static bool ToDate(const Null& c, Date& c2) { return false; }
	inline static bool ToDate(const Null *c, Date& c2) { return false; }
	inline static bool ToDate(const Undefined& c, Date& c2) { return false; }
	inline static bool ToDate(const Undefined *c, Date& c2) { return false; }
	inline static bool ToDate(const IConvertable *c, Date& c2) { return c->ToDate(c2); }
	inline static bool ToDate(const IConvertable& c, Date& c2) { return c.ToDate(c2); }
	inline static bool ToDate(char c, Date& c2) { return false; }
	inline static bool ToDate(bool c, Date& c2) { return false; }
	inline static bool ToDate(byte c, Date& c2) { return false; }
	inline static bool ToDate(int16 c, Date& c2) { return false; }
	inline static bool ToDate(int32 c, Date& c2) { return false; }
	inline static bool ToDate(int64 c, Date& c2) { return false; }
	inline static bool ToDate(uint32 c, Date& c2) { return false; }
	inline static bool ToDate(uint64 c, Date& c2) { return false; }
	inline static bool ToDate(BigInteger& c, Date& c2) { return false; }
	inline static bool ToDate(float32 c, Date& c2) { return false; }
	inline static bool ToDate(float64 c, Date& c2) { return false; }
	inline static bool ToDate(const DateTime& c, Date& c2) { c2 = c.DatePart(); return true; }
	inline static bool ToDate(const Date& c, Date& c2) { c2 = c;  return true; }
	inline static bool ToDate(const String& c, Date& c2) 
	{ 
		if (Date::IsDate(c))
		{
			c2 = Date::Parse(c); 
			return true;
		}
		return false;
	}
	inline static bool ToDate(const Decimal& c, Date& c2) { return false; }
//	inline static bool ToDate(const IJsObject& c, Date& c2) { return false; }
//	inline static bool ToDate(const IJsObject* c, Date& c2) { return false; }

	inline static bool ToDecimal(const Null& c, Decimal& c2) { c2 = 0; return true; }
	inline static bool ToDecimal(const Null *c, Decimal& c2) { c2 = 0; return true; }
	inline static bool ToDecimal(const Undefined& c, Decimal& c2) { return false; }
	inline static bool ToDecimal(const Undefined *c, Decimal& c2) { return false; }
	inline static bool ToDecimal(const IConvertable *c, Decimal& c2) { return c->ToDecimal(c2); }
	inline static bool ToDecimal(const IConvertable& c, Decimal& c2) { return c.ToDecimal(c2); }
	inline static bool ToDecimal(char c, Decimal& c2) { c2 = (int)c; return true; }
	inline static bool ToDecimal(bool c, Decimal& c2) { c2 = c ? 1 : 0; return true; }
	inline static bool ToDecimal(byte c, Decimal& c2) { c2 = (int)c; return true; }
	inline static bool ToDecimal(int16 c, Decimal& c2) { c2 = (int)c; return true; }
	inline static bool ToDecimal(int32 c, Decimal& c2) { c2 = (int)c; return true; }
	inline static bool ToDecimal(int64 c, Decimal& c2) { c2 = c; return true; }
	inline static bool ToDecimal(uint32 c, Decimal& c2) { c2 = (int64)c; return true; }
	inline static bool ToDecimal(uint64 c, Decimal& c2) { c2 = (int64)c; return true; }
	inline static bool ToDecimal(BigInteger& c, Decimal& c2) { c2 = c.ToInt(); return true; }
	inline static bool ToDecimal(float32 c, Decimal& c2) { c2 = c; return true;}
	inline static bool ToDecimal(float64 c, Decimal& c2) { c2 = c; return true; }
	inline static bool ToDecimal(const DateTime& c, Decimal& c2) { return false; }
	inline static bool ToDecimal(const Date& c, Decimal& c2) { return false; }
	inline static bool ToDecimal(const String& c, Decimal& c2)
	{ 
		if (Double::IsDouble(c))
		{
			c2 = Decimal::Parse(c);
			return true;
		}
		return false;
	}
	inline static bool ToDecimal(const Decimal& c, Decimal& c2) { c2 = c; return true; }
//	inline static bool ToDecimal(const IJsObject& c, Decimal& c2) { return false; }
//	inline static bool ToDecimal(const IJsObject* c, Decimal& c2) { return false; }

	inline static bool ToString(const Null& c, String& c2) { c2 = "null"; return true; }
	inline static bool ToString(const Null *c, String& c2) { c2 = "null"; return true; }
	inline static bool ToString(const Undefined& c, String& c2) { c2 = "undefined"; return true; }
	inline static bool ToString(const Undefined *c, String& c2) { c2 = "undefined"; return true; }
	inline static bool ToString(const IConvertable *c, String& c2) { return c->ToString(c2); }
	inline static bool ToString(const IConvertable& c, String& c2) { return c.ToString(c2); }
	inline static bool ToString(char c, String& c2) { char cp[2]; cp[0] = c; cp[1] = 0; c2 = String(cp); return true; }
	inline static bool ToString(bool c, String& c2) { c2 = c ? "TRUE" : "FALSE"; return true; }
	inline static bool ToString(byte c, String& c2) { c2 = *Int32::ToString((int)c); return true; }
	inline static bool ToString(int16 c, String& c2) { c2 = *Int32::ToString((int)c); return true; }
	inline static bool ToString(int32 c, String& c2) { c2 = *Int32::ToString(c); return true; }
	inline static bool ToString(int64 c, String& c2) { c2 = *Int64::ToString(c); return true; }
	inline static bool ToString(uint32 c, String& c2) { c2 = *UInt32::ToString(c); return true; }
	inline static bool ToString(uint64 c, String& c2) { c2 = *UInt64::ToString(c); return true; }
	inline static bool ToString(BigInteger& c, String& c2) { c2 = c.ToString(); return true; }
	inline static bool ToString(float32 c, String& c2) { c2 = *Double::ToString(c); return true;}
	inline static bool ToString(float64 c, String& c2) { c2 = *Double::ToString(c); return true; }
	inline static bool ToString(const DateTime& c, String& c2) { c2 = *c.ToString(); return true; }
	inline static bool ToString(const Date& c, String& c2) { c2 = *c.ToString(); return true; }
	inline static bool ToString(const String& c, String& c2) { c2 = c; return true; }
	inline static bool ToString(const Decimal& c, String& c2) { c2 = *c.ToString(); return true; }
//	inline static bool ToString(const IJsObject& c, String& c2) { c2 = *c.ToString(); return true; }
//	inline static bool ToString(const IJsObject* c, String& c2) { c2 = *c->ToString(); return true; }

//	inline static bool ToObject(const Null& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const Null *c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const Undefined& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const Undefined *c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const IConvertable *c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const IConvertable& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(char c, IJsObject& c2) { return false; }
//	inline static bool ToObject(bool c, IJsObject& c2) { return false; }
//	inline static bool ToObject(byte c, IJsObject& c2) { return false; }
//	inline static bool ToObject(int16 c, IJsObject& c2) { return false; }
//	inline static bool ToObject(int32 c, IJsObject& c2) { return false; }
//	inline static bool ToObject(int64 c, IJsObject& c2) { return false; }
//	inline static bool ToObject(uint32 c, IJsObject& c2) { return false; }
//	inline static bool ToObject(uint64 c, IJsObject& c2) { return false; }
//	inline static bool ToObject(BigInteger& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(float32 c, IJsObject& c2) { return false; }
//	inline static bool ToObject(float64 c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const DateTime& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const Date& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const String& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const Decimal& c, IJsObject& c2) { return false; }
//	inline static bool ToObject(const IJsObject& c, IJsObject& c2) { c2 = c; return true; }
//	inline static bool ToObject(const IJsObject* c, IJsObject& c2) { c2 = *c; return true; }

	inline static String TypeName(const Null& c) { return String("null"); }
	inline static String TypeName(const Null *c) { return String("null"); }
	inline static String TypeName(const Undefined& c) { return String("undefined"); }
	inline static String TypeName(const Undefined *c) { return String("undefined"); }
	inline static String TypeName(const IConvertable *c) { return c->TypeName(); }
	inline static String TypeName(const IConvertable& c) { return c.TypeName(); }
	inline static String TypeName(char c) { return String("char"); }
	inline static String TypeName(bool c) { return String("bool"); }
	inline static String TypeName(byte c) { return String("byte"); }
	inline static String TypeName(int16 c) { return String("int16"); }
	inline static String TypeName(int32 c) { return String("int32"); }
	inline static String TypeName(int64 c) { return String("int64"); }
	inline static String TypeName(uint32 c) { return String("uint32"); }
	inline static String TypeName(uint64 c) { return String("uint64"); }
	inline static String TypeName(BigInteger& c) { return String("BigInteger"); }
	inline static String TypeName(float32 c) { return String("float"); }
	inline static String TypeName(float64 c) { return String("double"); }
	inline static String TypeName(const DateTime& c) { return String("DateTime"); }
	inline static String TypeName(const Date& c) { return String("Date"); }
	inline static String TypeName(const String& c) { return String("String"); }
	inline static String TypeName(const Decimal& c) { return String("Decimal"); }
//	inline static String TypeName(const IJsObject& c) 
//	{
//		return *c.TypeName();
//	}
//	inline static String TypeName(const IJsObject* c) 
//	{
//		return TypeName(*c);
//	}
};

/** @} */
}
#endif
