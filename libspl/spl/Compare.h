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
#ifndef _compare_h
#define _compare_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/BigInteger.h>
#include <spl/Char.h>
#include <spl/Date.h>
#include <spl/DateTime.h>
#include <spl/Decimal.h>
#include <spl/Double.h>
#include <spl/IConvertable.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
#include <spl/Null.h>
#include <spl/math/Math.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/UInt32.h>
#include <spl/UInt64.h>
#include <spl/Undefined.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

/** @brief Compare's job is to allow template classes to perform search, sorting, and balancing without understanding the types involved.
 *
 */
class Compare
{
private:
	Compare() {}
	Compare(const Compare& c) {}
	Compare& operator =(const Compare& c) { return *this; }

public:
	inline static int32 Majic( const int16& i ) { return INT16_MAJIC; }
	inline static int32 Majic( const int32& i ) { return INT32_MAJIC; }
	inline static int32 Majic( const int64& i ) { return INT64_MAJIC; }
	inline static int32 Majic( const byte& i ) { return BYTE_MAJIC; }
	inline static int32 Majic( const uint32& i ) { return UINT32_MAJIC; }
	inline static int32 Majic( const uint64& i ) { return UINT64_MAJIC; }
	inline static int32 Majic( const float32& i ) { return FLOAT32_MAJIC; }
	inline static int32 Majic( const float64& i ) { return DOUBLE_MAJIC; }
	inline static int32 Majic( const char& i ) { return CHAR_MAJIC; }
	inline static int32 Majic( const bool& i ) { return BOOL_MAJIC; }
	inline static int32 Majic( const Decimal& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const String& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const StringPtr& i ) { return i->MajicNumber(); }
	inline static int32 Majic( const IComparable& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const IComparable *i ) { return i->MajicNumber(); }
	inline static int32 Majic( const BigInteger& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const Date& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const DateTime& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const Null& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const Undefined& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const IConvertable& i ) { return i.MajicNumber(); }
	inline static int32 Majic( const IConvertable* i ) { return i->MajicNumber(); }

	inline static bool IsFloatMajic( const int32 majic ) { return majic == FLOAT32_MAJIC || majic == DOUBLE_MAJIC || majic == DECIMAL_MAJIC; }
	inline static bool IsIntMajic( const int32 majic ) { return majic == INT16_MAJIC || majic == INT32_MAJIC || majic == INT64_MAJIC; }
	inline static bool IsUnsignedMajic( const int32 majic ) { return majic == BYTE_MAJIC || majic == UINT32_MAJIC || majic == UINT64_MAJIC; }
	inline static bool IsCharMajic( const int32 majic ) { return majic == CHAR_MAJIC; }
	inline static bool IsBoolMajic( const int32 majic ) { return majic == BOOL_MAJIC; }
	inline static bool IsDateMajic( const int32 majic ) { return majic == DATE_MAJIC; }
	inline static bool IsDateTimeMajic( const int32 majic ) { return majic == DATETIME_MAJIC; }
	inline static bool IsStringMajic( const int32 majic ) { return majic == STRING_MAJIC; }

	inline static bool Equals(const Null & c2, const Null& c) { return true; }
	inline static bool Equals(const Null & c2, const IComparable& c) { return c.Equals(c2); }
	inline static bool Equals(const Null & c2, const RefCountPtr<IComparable> c) { return c->Equals(c2); }
	inline static bool Equals(const Null & c2, char c) { return 0 == c; }
	inline static bool Equals(const Null & c2, bool c) { return false == c; }
	inline static bool Equals(const Null & c2, byte c) { return 0 == c; }
	inline static bool Equals(const Null & c2, int16 c) { return c == 0; }
	inline static bool Equals(const Null & c2, int32 c) { return c == 0; }
	inline static bool Equals(const Null & c2, int64 c) { return c == 0; }
	inline static bool Equals(const Null & c2, uint32 c) { return c == 0; }
	inline static bool Equals(const Null & c2, uint64 c) { return c == 0; }
	inline static bool Equals(const Null & c2, BigInteger& c) { return c == 0L; }
	inline static bool Equals(const Null & c2, float32 c) { return c == 0; }
	inline static bool Equals(const Null & c2, float64 c) { return c == 0; }
	inline static bool Equals(const Null & c2, const DateTime& c) { return false; }
	inline static bool Equals(const Null & c2, const Date& c) { return false; }
	inline static bool Equals(const Null & c2, const String& c) { return false; }
	inline static bool Equals(const Null & c2, const StringPtr& c) { return false; }
	inline static bool Equals(const Null & c2, const IConvertable& c) { return false; }

	inline static bool Equals(const IComparable & c2, const Null& c) { return c.Equals(c2); }
	inline static bool Equals(const IComparable & c2, const IComparable& c) { return c2.Equals(c); }
	inline static bool Equals(const IComparable & c2, const RefCountPtr<IComparable>& c) { return c2.Equals(*c); }
	inline static bool Equals(const IComparable & c2, char c) { return false; }
	inline static bool Equals(const IComparable & c2, bool c) { return false; }
	inline static bool Equals(const IComparable & c2, byte c) { return false; }
	inline static bool Equals(const IComparable & c2, int16 c) { return false; }
	inline static bool Equals(const IComparable & c2, int32 c) { return false; }
	inline static bool Equals(const IComparable & c2, int64 c) { return false; }
	inline static bool Equals(const IComparable & c2, uint32 c) { return false; }
	inline static bool Equals(const IComparable & c2, uint64 c) { return false; }
	inline static bool Equals(const IComparable & c2, BigInteger& c) { return c2.Equals(c); }
	inline static bool Equals(const IComparable & c2, float32 c) { return false; }
	inline static bool Equals(const IComparable & c2, float64 c) { return false; }
	inline static bool Equals(const IComparable & c2, const DateTime& c) { return c2.Equals(c); }
	inline static bool Equals(const IComparable & c2, const Date& c) { return c2.Equals(c); }
	inline static bool Equals(const IComparable & c2, const String& c) { return c2.Equals(c); }
	inline static bool Equals(const IComparable & c2, const StringPtr& c) { return c2.Equals(*c); }
	inline static bool Equals(const IComparable & c2, const IConvertable& c) { return c2.Equals(c); }

	inline static bool Equals(char c2, const Null& c) { return false; }
	inline static bool Equals(char c2, const IComparable& c) { return false; }
	inline static bool Equals(char c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(char c2, char c) { return c2 == c; }
	inline static bool Equals(char c2, bool c) { return false; }
	inline static bool Equals(char c2, byte c) { return c2 == (char)c; }
	inline static bool Equals(char c2, int16 c) { return (int16)c2 == c; }
	inline static bool Equals(char c2, int32 c) { return (int32)c2 == c; }
	inline static bool Equals(char c2, int64 c) { return (int64)c2 == c; }
	inline static bool Equals(char c2, uint32 c) { return (uint32)c2 == c; }
	inline static bool Equals(char c2, uint64 c) { return (uint64)c2 == c; }
	inline static bool Equals(char c2, BigInteger& c) { return false; }
	inline static bool Equals(char c2, float32 c) { return false; }
	inline static bool Equals(char c2, float64 c) { return false; }
	inline static bool Equals(char c2, const DateTime& c) { return false; }
	inline static bool Equals(char c2, const Date& c) { return false; }
	inline static bool Equals(char c2, const String& c) { return c.Length() == 1 && c2 == c.CharAt(0); }
	inline static bool Equals(char c2, const StringPtr& c) { return c->Length() == 1 && c2 == c->CharAt(0); }
	inline static bool Equals(char c2, const IConvertable& c) { char ch; return c.ToChar(ch) && ch == c2; }

	inline static bool Equals(bool c2, const Null& c) { return c2 == false; }
	inline static bool Equals(bool c2, const IComparable& c) { return false; }
	inline static bool Equals(bool c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(bool c2, char c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, bool c) { return c2 == c; }
	inline static bool Equals(bool c2, byte c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, int16 c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, int32 c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, int64 c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, uint32 c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, uint64 c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, BigInteger& c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, float32 c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, float64 c) { return c2 == (c != 0); }
	inline static bool Equals(bool c2, const DateTime& c) { return false; }
	inline static bool Equals(bool c2, const Date& c) { return false; }
	inline static bool Equals(bool c2, const String& c) { return c2 == (c.Length() != 0); }
	inline static bool Equals(bool c2, const StringPtr& c) { return c2 == (c->Length() != 0); }
	inline static bool Equals(bool c2, const IConvertable& c) { bool b; return c.ToBool(b) && b == c2; }

	inline static bool Equals(byte c2, const Null& c) { return c2 == 0; }
	inline static bool Equals(byte c2, const IComparable& c) { return false; }
	inline static bool Equals(byte c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(byte c2, char c) { return (char)c2 == c; }
	inline static bool Equals(byte c2, bool c) { return c2 != 0; }
	inline static bool Equals(byte c2, byte c) { return c2 == c; }
	inline static bool Equals(byte c2, int16 c) { return (int16)c2 == c; }
	inline static bool Equals(byte c2, int32 c) { return (int32)c2 == c; }
	inline static bool Equals(byte c2, int64 c) { return (int64)c2 == c; }
	inline static bool Equals(byte c2, uint32 c) { return (uint32)c2 == c; }
	inline static bool Equals(byte c2, uint64 c) { return (uint64)c2 == c; }
	inline static bool Equals(byte c2, BigInteger& c) { return c.ToInt() == (int64)c2; }
	inline static bool Equals(byte c2, float32 c) { return (float32)c2 == c; }
	inline static bool Equals(byte c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(byte c2, const DateTime& c) { return false; }
	inline static bool Equals(byte c2, const Date& c) { return false; }
	inline static bool Equals(byte c2, const String& c) { return c == *UInt32::ToString((uint32)c2); }
	inline static bool Equals(byte c2, const StringPtr& c) { return *c == *UInt32::ToString((uint32)c2); }
	inline static bool Equals(byte c2, const IConvertable& c) { byte b; return c.ToByte(b) && b == c2; }

	inline static bool Equals(int16 c2, const Null& c) { return c2 == 0; }
	inline static bool Equals(int16 c2, const IComparable& c) { return false; }
	inline static bool Equals(int16 c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(int16 c2, char c) { return c2 == (int16)c; }
	inline static bool Equals(int16 c2, bool c) { return c2 != 0; }
	inline static bool Equals(int16 c2, byte c) { return c2 == (int16)c; }
	inline static bool Equals(int16 c2, int16 c) { return c2 == c; }
	inline static bool Equals(int16 c2, int32 c) { return (int32)c2 == c; }
	inline static bool Equals(int16 c2, int64 c) { return (int64)c2 == c; }
	inline static bool Equals(int16 c2, uint32 c) { return (uint32)c2 == c; }
	inline static bool Equals(int16 c2, uint64 c) { return (uint64)c2 == c; }
	inline static bool Equals(int16 c2, BigInteger& c) { return c.ToInt() == (int64)c2; }
	inline static bool Equals(int16 c2, float32 c) { return (float32)c2 == c; }
	inline static bool Equals(int16 c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(int16 c2, const DateTime& c) { return false; }
	inline static bool Equals(int16 c2, const Date& c) { return false; }
	inline static bool Equals(int16 c2, const String& c) { return c == *Int32::ToString((int32)c2); }
	inline static bool Equals(int16 c2, const StringPtr& c) { return *c == *Int32::ToString((int32)c2); }
	inline static bool Equals(int16 c2, const IConvertable& c) { int16 i; return c.ToInt16(i) && i == c2; }

	inline static bool Equals(int32 c2, const Null& c) { return c2 == 0; }
	inline static bool Equals(int32 c2, const IComparable& c) { return false; }
	inline static bool Equals(int32 c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(int32 c2, char c) { return c2 == (int32)c; }
	inline static bool Equals(int32 c2, bool c) { return c2 != 0; }
	inline static bool Equals(int32 c2, byte c) { return c2 == (int32)c; }
	inline static bool Equals(int32 c2, int16 c) { return c2 == (int32)c; }
	inline static bool Equals(int32 c2, int32 c) { return c2 == c; }
	inline static bool Equals(int32 c2, int64 c) { return (int64)c2 == c; }
	inline static bool Equals(int32 c2, uint32 c) { return (uint32)c2 == c; }
	inline static bool Equals(int32 c2, uint64 c) { return (uint64)c2 == c; }
	inline static bool Equals(int32 c2, BigInteger& c) { return c.ToInt() == (int64)c2; }
	inline static bool Equals(int32 c2, float32 c) { return (float32)c2 == c; }
	inline static bool Equals(int32 c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(int32 c2, const DateTime& c) { return false; }
	inline static bool Equals(int32 c2, const Date& c) { return false; }
	inline static bool Equals(int32 c2, const String& c) { return c == *Int32::ToString((int32)c2); }
	inline static bool Equals(int32 c2, const StringPtr& c) { return *c == *Int32::ToString((int32)c2); }
	inline static bool Equals(int32 c2, const IConvertable& c) { int32 i; return c.ToInt32(i) && i == c2; }

	inline static bool Equals(int64 c2, const Null& c) { return c2 == 0; }
	inline static bool Equals(int64 c2, const IComparable& c) { return false; }
	inline static bool Equals(int64 c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(int64 c2, char c) { return c2 == (int64)c; }
	inline static bool Equals(int64 c2, bool c) { return c2 != 0; }
	inline static bool Equals(int64 c2, byte c) { return c2 == (int64)c; }
	inline static bool Equals(int64 c2, int16 c) { return c2 == (int64)c; }
	inline static bool Equals(int64 c2, int32 c) { return c2 == (int64)c; }
	inline static bool Equals(int64 c2, int64 c) { return c2 == c; }
	inline static bool Equals(int64 c2, uint32 c) { return c2 == (int64)c; }
	inline static bool Equals(int64 c2, uint64 c) { return (uint64)c2 == c; }
	inline static bool Equals(int64 c2, BigInteger& c) { return c.ToInt() == (int64)c2; }
	inline static bool Equals(int64 c2, float32 c) { return (float64)c2 == (float64)c; }
	inline static bool Equals(int64 c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(int64 c2, const DateTime& c) { return false; }
	inline static bool Equals(int64 c2, const Date& c) { return false; }
	inline static bool Equals(int64 c2, const String& c) { return c == *Int64::ToString((int64)c2); }
	inline static bool Equals(int64 c2, const StringPtr& c) { return *c == *Int64::ToString((int64)c2); }
	inline static bool Equals(int64 c2, const IConvertable& c) { int64 i; return c.ToInt64(i) && i == c2; }

	inline static bool Equals(uint32 c2, const Null& c) { return c2 == 0; }
	inline static bool Equals(uint32 c2, const IComparable& c) { return false; }
	inline static bool Equals(uint32 c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(uint32 c2, char c) { return c2 == (uint32)c; }
	inline static bool Equals(uint32 c2, bool c) { return c2 != 0; }
	inline static bool Equals(uint32 c2, byte c) { return c2 == (uint32)c; }
	inline static bool Equals(uint32 c2, int16 c) { return c2 == (uint32)c; }
	inline static bool Equals(uint32 c2, int32 c) { return c2 == (uint32)c; }
	inline static bool Equals(uint32 c2, int64 c) { return (int64)c2 == c; }
	inline static bool Equals(uint32 c2, uint32 c) { return c2 == c; }
	inline static bool Equals(uint32 c2, uint64 c) { return (uint64)c2 == c; }
	inline static bool Equals(uint32 c2, BigInteger& c) { return c.ToInt() == (int64)c2; }
	inline static bool Equals(uint32 c2, float32 c) { return (float32)c2 == c; }
	inline static bool Equals(uint32 c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(uint32 c2, const DateTime& c) { return false; }
	inline static bool Equals(uint32 c2, const Date& c) { return false; }
	inline static bool Equals(uint32 c2, const String& c) { return c == *UInt32::ToString((uint32)c2); }
	inline static bool Equals(uint32 c2, const StringPtr& c) { return *c == *UInt32::ToString((uint32)c2); }
	inline static bool Equals(uint32 c2, const IConvertable& c) { uint32 i; return c.ToUInt32(i) && i == c2; }

	inline static bool Equals(uint64 c2, const Null& c) { return c2 == 0; }
	inline static bool Equals(uint64 c2, const IComparable& c) { return false; }
	inline static bool Equals(uint64 c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(uint64 c2, char c) { return c2 == (uint64)c; }
	inline static bool Equals(uint64 c2, bool c) { return c2 != 0; }
	inline static bool Equals(uint64 c2, byte c) { return c2 == (uint64)c; }
	inline static bool Equals(uint64 c2, int16 c) { return c2 == (uint64)c; }
	inline static bool Equals(uint64 c2, int32 c) { return c2 == (uint64)c; }
	inline static bool Equals(uint64 c2, int64 c) { return c2 == (uint64)c; }
	inline static bool Equals(uint64 c2, uint32 c) { return c2 == (uint64)c; }
	inline static bool Equals(uint64 c2, uint64 c) { return c2 == c; }
	inline static bool Equals(uint64 c2, BigInteger& c) { return c.ToInt() == (int64)c2; }
	inline static bool Equals(uint64 c2, float32 c) { return (float64)c2 == (float64)c; }
	inline static bool Equals(uint64 c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(uint64 c2, const DateTime& c) { return false; }
	inline static bool Equals(uint64 c2, const Date& c) { return false; }
	inline static bool Equals(uint64 c2, const String& c) { return c == *UInt64::ToString((uint64)c2); }
	inline static bool Equals(uint64 c2, const StringPtr& c) { return *c == *UInt64::ToString((uint64)c2); }
	inline static bool Equals(uint64 c2, const IConvertable& c) { uint64 i; return c.ToUInt64(i) && i == c2; }

	inline static bool Equals(float32 c2, const Null& c) { return false; }
	inline static bool Equals(float32 c2, const IComparable& c) { return false; }
	inline static bool Equals(float32 c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(float32 c2, char c) { return c2 == (float32)c; }
	inline static bool Equals(float32 c2, bool c) { return c2 != 0; }
	inline static bool Equals(float32 c2, byte c) { return c2 == (float32)c; }
	inline static bool Equals(float32 c2, int16 c) { return c2 == (float32)c; }
	inline static bool Equals(float32 c2, int32 c) { return c2 == (float32)c; }
	inline static bool Equals(float32 c2, int64 c) { return c2 == (float32)c; }
	inline static bool Equals(float32 c2, uint32 c) { return c2 == (float32)c; }
	inline static bool Equals(float32 c2, uint64 c) { return c2 == (float32)c; }
	inline static bool Equals(float32 c2, BigInteger& c) { return (float64)c.ToInt() == (float64)c2; }
	inline static bool Equals(float32 c2, float32 c) { return (float64)c2 == (float64)c; }
	inline static bool Equals(float32 c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(float32 c2, const DateTime& c) { return false; }
	inline static bool Equals(float32 c2, const Date& c) { return false; }
	inline static bool Equals(float32 c2, const String& c) { return false; }
	inline static bool Equals(float32 c2, const StringPtr& c) { return false; }
	inline static bool Equals(float32 c2, const IConvertable& c) { float32 i; return c.ToFloat32(i) && i == c2; }

	inline static bool Equals(float64 c2, const Null& c) { return false; }
	inline static bool Equals(float64 c2, const IComparable& c) { return false; }
	inline static bool Equals(float64 c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(float64 c2, char c) { return c2 == (float64)c; }
	inline static bool Equals(float64 c2, bool c) { return c2 != 0; }
	inline static bool Equals(float64 c2, byte c) { return c2 == (float64)c; }
	inline static bool Equals(float64 c2, int16 c) { return c2 == (float64)c; }
	inline static bool Equals(float64 c2, int32 c) { return c2 == (float64)c; }
	inline static bool Equals(float64 c2, int64 c) { return c2 == (float64)c; }
	inline static bool Equals(float64 c2, uint32 c) { return c2 == (float64)c; }
	inline static bool Equals(float64 c2, uint64 c) { return c2 == (float64)c; }
	inline static bool Equals(float64 c2, BigInteger& c) { return (float64)c.ToInt() == (float64)c2; }
	inline static bool Equals(float64 c2, float32 c) { return (float64)c2 == (float64)c; }
	inline static bool Equals(float64 c2, float64 c) { return (float64)c2 == c; }
	inline static bool Equals(float64 c2, const DateTime& c) { return false; }
	inline static bool Equals(float64 c2, const Date& c) { return false; }
	inline static bool Equals(float64 c2, const String& c) { return false; }
	inline static bool Equals(float64 c2, const StringPtr& c) { return false; }
	inline static bool Equals(float64 c2, const IConvertable& c) { float64 i; return c.ToFloat64(i) && i == c2; }

	inline static bool Equals(const DateTime& c2, const Null& c) { return false; }
	inline static bool Equals(const DateTime& c2, const IComparable& c) { return c.Equals(c2); }
	inline static bool Equals(const DateTime& c2, const RefCountPtr<IComparable> c) { return c->Equals(c2); }
	inline static bool Equals(const DateTime& c2, char c) { return false; }
	inline static bool Equals(const DateTime& c2, bool c) { return false; }
	inline static bool Equals(const DateTime& c2, byte c) { return false; }
	inline static bool Equals(const DateTime& c2, int16 c) { return false; }
	inline static bool Equals(const DateTime& c2, int32 c) { return false; }
	inline static bool Equals(const DateTime& c2, int64 c) { return false; }
	inline static bool Equals(const DateTime& c2, uint32 c) { return false; }
	inline static bool Equals(const DateTime& c2, uint64 c) { return false; }
	inline static bool Equals(const DateTime& c2, BigInteger& c) { return false; }
	inline static bool Equals(const DateTime& c2, float32 c) { return false; }
	inline static bool Equals(const DateTime& c2, float64 c) { return false; }
	inline static bool Equals(const DateTime& c2, const DateTime& c) { return c2 == c; }
	inline static bool Equals(const DateTime& c2, const Date& c) { return false; }
	inline static bool Equals(const DateTime& c2, const String& c) { return false; }
	inline static bool Equals(const DateTime& c2, const StringPtr& c) { return false; }
	inline static bool Equals(const DateTime& c2, const IConvertable& c) { DateTime i; return c.ToDateTime(i) && i == c2; }

	inline static bool Equals(const Date& c2, const Null& c) { return false; }
	inline static bool Equals(const Date& c2, const IComparable& c) { return c.Equals(c2); }
	inline static bool Equals(const Date& c2, const RefCountPtr<IComparable> c) { return c->Equals(c2); }
	inline static bool Equals(const Date& c2, char c) { return false; }
	inline static bool Equals(const Date& c2, bool c) { return false; }
	inline static bool Equals(const Date& c2, byte c) { return false; }
	inline static bool Equals(const Date& c2, int16 c) { return false; }
	inline static bool Equals(const Date& c2, int32 c) { return false; }
	inline static bool Equals(const Date& c2, int64 c) { return false; }
	inline static bool Equals(const Date& c2, uint32 c) { return false; }
	inline static bool Equals(const Date& c2, uint64 c) { return false; }
	inline static bool Equals(const Date& c2, BigInteger& c) { return false; }
	inline static bool Equals(const Date& c2, float32 c) { return false; }
	inline static bool Equals(const Date& c2, float64 c) { return false; }
	inline static bool Equals(const Date& c2, const DateTime& c) { return false; }
	inline static bool Equals(const Date& c2, const Date& c) { return c2 == c; }
	inline static bool Equals(const Date& c2, const String& c) { return false; }
	inline static bool Equals(const Date& c2, const StringPtr& c) { return false; }
	inline static bool Equals(const Date& c2, const IConvertable& c) { Date i; return c.ToDate(i) && i == c2; }

	inline static bool Equals(const String& c2, const Null& c) { return false; }
	inline static bool Equals(const String& c2, const IComparable& c) { return c.Equals(c2); }
	inline static bool Equals(const String& c2, const RefCountPtr<IComparable> c) { return c->Equals(c2); }
	inline static bool Equals(const String& c2, char c) { return false; }
	inline static bool Equals(const String& c2, bool c) { return false; }
	inline static bool Equals(const String& c2, byte c) { return false; }
	inline static bool Equals(const String& c2, int16 c) { return false; }
	inline static bool Equals(const String& c2, int32 c) { return false; }
	inline static bool Equals(const String& c2, int64 c) { return false; }
	inline static bool Equals(const String& c2, uint32 c) { return false; }
	inline static bool Equals(const String& c2, uint64 c) { return false; }
	inline static bool Equals(const String& c2, BigInteger& c) { return false; }
	inline static bool Equals(const String& c2, float32 c) { return false; }
	inline static bool Equals(const String& c2, float64 c) { return false; }
	inline static bool Equals(const String& c2, const DateTime& c) { return false; }
	inline static bool Equals(const String& c2, const Date& c) { return false; }
	inline static bool Equals(const String& c2, const String& c) { return c2 == c; }
	inline static bool Equals(const String& c2, const StringPtr& c) { return c2 == *c; }
	inline static bool Equals(const String& c2, const IConvertable& c) { String i; return c.ToString(i) && i == c2; }

	inline static bool Equals(const StringPtr& c2, const Null& c) { return false; }
	inline static bool Equals(const StringPtr& c2, const IComparable& c) { return c.Equals(c2); }
	inline static bool Equals(const StringPtr& c2, const RefCountPtr<IComparable> c) { return c->Equals(*c2); }
	inline static bool Equals(const StringPtr& c2, char c) { return false; }
	inline static bool Equals(const StringPtr& c2, bool c) { return false; }
	inline static bool Equals(const StringPtr& c2, byte c) { return false; }
	inline static bool Equals(const StringPtr& c2, int16 c) { return false; }
	inline static bool Equals(const StringPtr& c2, int32 c) { return false; }
	inline static bool Equals(const StringPtr& c2, int64 c) { return false; }
	inline static bool Equals(const StringPtr& c2, uint32 c) { return false; }
	inline static bool Equals(const StringPtr& c2, uint64 c) { return false; }
	inline static bool Equals(const StringPtr& c2, BigInteger& c) { return false; }
	inline static bool Equals(const StringPtr& c2, float32 c) { return false; }
	inline static bool Equals(const StringPtr& c2, float64 c) { return false; }
	inline static bool Equals(const StringPtr& c2, const DateTime& c) { return false; }
	inline static bool Equals(const StringPtr& c2, const Date& c) { return false; }
	inline static bool Equals(const StringPtr& c2, const String& c) { return *c2 == c; }
	inline static bool Equals(const StringPtr& c2, const StringPtr& c) { return *c2 == *c; }
	inline static bool Equals(const StringPtr& c2, const IConvertable& c) { String i; return c.ToString(i) && i == *c2; }

	inline static bool Equals(const IConvertable & c2, const Null& c) { return false; }
	inline static bool Equals(const IConvertable & c2, const IComparable& c) { return false; }
	inline static bool Equals(const IConvertable & c2, const RefCountPtr<IComparable> c) { return false; }
	inline static bool Equals(const IConvertable & c2, char c) { char ch; return c2.ToChar(ch) && ch == c; }
	inline static bool Equals(const IConvertable & c2, bool c) { bool b; return c2.ToBool(b) && b == c; }
	inline static bool Equals(const IConvertable & c2, byte c) { byte b; return c2.ToByte(b) && b == c; }
	inline static bool Equals(const IConvertable & c2, int16 c) { int16 i; return c2.ToInt16(i) && i == c; }
	inline static bool Equals(const IConvertable & c2, int32 c) { int32 i; return c2.ToInt32(i) && i == c; }
	inline static bool Equals(const IConvertable & c2, int64 c) { int64 i; return c2.ToInt64(i) && i == c; }
	inline static bool Equals(const IConvertable & c2, uint32 c) { uint32 i; return c2.ToUInt32(i) && i == c; }
	inline static bool Equals(const IConvertable & c2, uint64 c) { uint64 i; return c2.ToUInt64(i) && i == c; }
	inline static bool Equals(const IConvertable & c2, BigInteger& c) { return false; }
	inline static bool Equals(const IConvertable & c2, float32 c) { float32 d; return c2.ToFloat32(d) && d == c; }
	inline static bool Equals(const IConvertable & c2, float64 c) { float64 d; return c2.ToFloat64(d) && d == c; }
	inline static bool Equals(const IConvertable & c2, const DateTime& c) { DateTime dtm; return c2.ToDateTime(dtm) && dtm == c; }
	inline static bool Equals(const IConvertable & c2, const Date& c) { Date dt; return c2.ToDate(dt) && dt == c; }
	inline static bool Equals(const IConvertable & c2, const String& c) { String s; return c2.ToString(s) && s == c; }
	inline static bool Equals(const IConvertable & c2, const StringPtr& c) { String s; return c2.ToString(s) && s == *c; }
	inline static bool Equals(const IConvertable & c2, const IConvertable& c) { return c2.Equals(c); }

	inline static int64 Cmp(const Null & c2, const Null& c) { return 0; }
	inline static int64 Cmp(const Null & c2, const IComparable& c) { return c2.Compare(c); }
	inline static int64 Cmp(const Null & c2, const RefCountPtr<IComparable> c) { return c2.Compare(*c); }
	inline static int64 Cmp(const Null & c2, char c) { return -(int)c; }
	inline static int64 Cmp(const Null & c2, bool c) { return -1; }
	inline static int64 Cmp(const Null & c2, byte c) { return -(int)c; }
	inline static int64 Cmp(const Null & c2, int16 c) { return -c; }
	inline static int64 Cmp(const Null & c2, int32 c) { return -c; }
	inline static int64 Cmp(const Null & c2, int64 c) { return -1; }
	inline static int64 Cmp(const Null & c2, uint32 c) { return -1; }
	inline static int64 Cmp(const Null & c2, uint64 c) { return -1; }
	inline static int64 Cmp(const Null & c2, BigInteger& c) { return -1; }
	inline static int64 Cmp(const Null & c2, float32 c) { return -1; }
	inline static int64 Cmp(const Null & c2, float64 c) { return -1; }
	inline static int64 Cmp(const Null & c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(const Null & c2, const Date& c) { return -1; }
	inline static int64 Cmp(const Null & c2, const String& c) { return -1; }
	inline static int64 Cmp(const Null & c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(const Null & c2, const IConvertable& c) { return -1; }

	inline static int64 Cmp(const IComparable & c2, const Null& c) { return c2.Compare(c); }
	inline static int64 Cmp(const IComparable & c2, const IComparable& c) { return c2.Compare(c); }
	inline static int64 Cmp(const IComparable & c2, const RefCountPtr<IComparable> c) { return c2.Compare(*c); }
	inline static int64 Cmp(const IComparable & c2, char c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, bool c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, byte c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, int16 c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, int32 c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, int64 c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, uint32 c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, uint64 c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, BigInteger& c) { return c2.Compare(c); }
	inline static int64 Cmp(const IComparable & c2, float32 c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, float64 c) { return -1; }
	inline static int64 Cmp(const IComparable & c2, const DateTime& c) { return c2.Compare(c); }
	inline static int64 Cmp(const IComparable & c2, const Date& c) { return c2.Compare(c); }
	inline static int64 Cmp(const IComparable & c2, const String& c) { return c2.Compare(c); }
	inline static int64 Cmp(const IComparable & c2, const StringPtr& c) { return c2.Compare(c); }
	inline static int64 Cmp(const IComparable & c2, const IConvertable& c) { return c2.Compare(c); }

	inline static int64 Cmp(char c2, const Null& c) { return 1; }
	inline static int64 Cmp(char c2, const IComparable& c) { return 1; }
	inline static int64 Cmp(char c2, const RefCountPtr<IComparable> c) { return 1; }
	inline static int64 Cmp(char c2, char c) { return (int64)(c2 - c); }
	inline static int64 Cmp(char c2, bool c) { return 1; }
	inline static int64 Cmp(char c2, byte c) { return (int64)(c2 - (char)c); }
	inline static int64 Cmp(char c2, int16 c) { return (int16)c2 - c; }
	inline static int64 Cmp(char c2, int32 c) { return (int32)c2 - c; }
	inline static int64 Cmp(char c2, int64 c) { return (int64)c2 - c; }
	inline static int64 Cmp(char c2, uint32 c) { return (uint32)c2 - c; }
	inline static int64 Cmp(char c2, uint64 c) { return (int64)((uint64)c2 - c); }
	inline static int64 Cmp(char c2, BigInteger& c) { return false; }
	inline static int64 Cmp(char c2, float32 c) { return -1; }
	inline static int64 Cmp(char c2, float64 c) { return -1; }
	inline static int64 Cmp(char c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(char c2, const Date& c) { return -1; }
	inline static int64 Cmp(char c2, const String& c) { return c.Length() == 1 && c2 == c.CharAt(0) ? 0 : c2 - c.CharAt(0); }
	inline static int64 Cmp(char c2, const StringPtr& c) { return c->Length() == 1 && c2 == c->CharAt(0) ? 0 : c2 - c->CharAt(0); }
	inline static int64 Cmp(char c2, const IConvertable& c) { char ch; return c.ToChar(ch) ? c2 - ch : -1; }

	inline static int64 Cmp(bool c2, const Null& c) { return 1; }
	inline static int64 Cmp(bool c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(bool c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(bool c2, char c) { return -1; }
	inline static int64 Cmp(bool c2, bool c) { return -1; }
	inline static int64 Cmp(bool c2, byte c) { return -1; }
	inline static int64 Cmp(bool c2, int16 c) { return -1; }
	inline static int64 Cmp(bool c2, int32 c) { return -1; }
	inline static int64 Cmp(bool c2, int64 c) { return -1; }
	inline static int64 Cmp(bool c2, uint32 c) { return -1; }
	inline static int64 Cmp(bool c2, uint64 c) { return -1; }
	inline static int64 Cmp(bool c2, BigInteger& c) { return -1; }
	inline static int64 Cmp(bool c2, float32 c) { return -1; }
	inline static int64 Cmp(bool c2, float64 c) { return -1; }
	inline static int64 Cmp(bool c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(bool c2, const Date& c) { return -1; }
	inline static int64 Cmp(bool c2, const String& c) { return -1; }
	inline static int64 Cmp(bool c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(bool c2, const IConvertable& c) { bool b; return c.ToBool(b) ? (b == c2 ? 0 : (b ? -1 : 1)) : -1; }

	inline static int64 Cmp(byte c2, const Null& c) { return 1; }
	inline static int64 Cmp(byte c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(byte c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(byte c2, char c) { return (char)c2 - c; }
	inline static int64 Cmp(byte c2, bool c) { return 1; }
	inline static int64 Cmp(byte c2, byte c) { return (int64)(c2 - c); }
	inline static int64 Cmp(byte c2, int16 c) { return (int16)c2 - c; }
	inline static int64 Cmp(byte c2, int32 c) { return (int32)c2 - c; }
	inline static int64 Cmp(byte c2, int64 c) { return (int64)c2 - c; }
	inline static int64 Cmp(byte c2, uint32 c) { return (uint32)c2 - c; }
	inline static int64 Cmp(byte c2, uint64 c) { return (int64)((uint64)c2 - c); }
	inline static int64 Cmp(byte c2, BigInteger& c) { return c.ToInt() - (int64)c2; }
	inline static int64 Cmp(byte c2, float32 c) { return -1; }
	inline static int64 Cmp(byte c2, float64 c) { return -1; }
	inline static int64 Cmp(byte c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(byte c2, const Date& c) { return -1; }
	inline static int64 Cmp(byte c2, const String& c) { return -1; }
	inline static int64 Cmp(byte c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(byte c2, const IConvertable& c) { byte b; return c.ToByte(b) ? c2 - b : -1; }

	inline static int64 Cmp(int16 c2, const Null& c) { return 1; }
	inline static int64 Cmp(int16 c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(int16 c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(int16 c2, char c) { return c2 - (int16)c; }
	inline static int64 Cmp(int16 c2, bool c) { return 1; }
	inline static int64 Cmp(int16 c2, byte c) { return c2 - (int16)c; }
	inline static int64 Cmp(int16 c2, int16 c) { return c2 - c; }
	inline static int64 Cmp(int16 c2, int32 c) { return (int32)c2 - c; }
	inline static int64 Cmp(int16 c2, int64 c) { return (int64)((int64)c2 - c); }
	inline static int64 Cmp(int16 c2, uint32 c) { return (int64)((uint32)c2 - c); }
	inline static int64 Cmp(int16 c2, uint64 c) { return (int64)((uint64)c2 - c); }
	inline static int64 Cmp(int16 c2, BigInteger& c) { return (int64)(c.ToInt() - (int64)c2); }
	inline static int64 Cmp(int16 c2, float32 c) { return -1; }
	inline static int64 Cmp(int16 c2, float64 c) { return -1; }
	inline static int64 Cmp(int16 c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(int16 c2, const Date& c) { return -1; }
	inline static int64 Cmp(int16 c2, const String& c) { return -1; }
	inline static int64 Cmp(int16 c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(int16 c2, const IConvertable& c) { int16 i; return c.ToInt16(i) ? c2 - i : -1; }

	inline static int64 Cmp(int32 c2, const Null& c) { return 1; }
	inline static int64 Cmp(int32 c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(int32 c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(int32 c2, char c) { return c2 - (int32)c; }
	inline static int64 Cmp(int32 c2, bool c) { return 1; }
	inline static int64 Cmp(int32 c2, byte c) { return c2 - (int32)c; }
	inline static int64 Cmp(int32 c2, int16 c) { return c2 - (int32)c; }
	inline static int64 Cmp(int32 c2, int32 c) { return c2 - c; }
	inline static int64 Cmp(int32 c2, int64 c) { return (int64)((int64)c2 - c); }
	inline static int64 Cmp(int32 c2, uint32 c) { return (int64)((uint32)c2 - c); }
	inline static int64 Cmp(int32 c2, uint64 c) { return (int64)((uint64)c2 - c); }
	inline static int64 Cmp(int32 c2, BigInteger& c) { return (int64)(c.ToInt() - (int64)c2); }
	inline static int64 Cmp(int32 c2, float32 c) { return -1; }
	inline static int64 Cmp(int32 c2, float64 c) { return -1; }
	inline static int64 Cmp(int32 c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(int32 c2, const Date& c) { return -1; }
	inline static int64 Cmp(int32 c2, const String& c) { return -1; }
	inline static int64 Cmp(int32 c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(int32 c2, const IConvertable& c) { int32 i; return c.ToInt32(i) ? c2 - i : -1; }

	inline static int64 Cmp(int64 c2, const Null& c) { return 1; }
	inline static int64 Cmp(int64 c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(int64 c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(int64 c2, char c) { return (int64)(c2 - (int64)c); }
	inline static int64 Cmp(int64 c2, bool c) { return 1; }
	inline static int64 Cmp(int64 c2, byte c) { return (int64)(c2 - (int64)c); }
	inline static int64 Cmp(int64 c2, int16 c) { return (int64)(c2 - (int64)c); }
	inline static int64 Cmp(int64 c2, int32 c) { return (int64)(c2 - (int64)c); }
	inline static int64 Cmp(int64 c2, int64 c) { return (int64)(c2 - c); }
	inline static int64 Cmp(int64 c2, uint32 c) { return (int64)(c2 - (int64)c); }
	inline static int64 Cmp(int64 c2, uint64 c) { return (int64)((uint64)c2 - c); }
	inline static int64 Cmp(int64 c2, BigInteger& c) { return (int64)(c.ToInt() - (int64)c2); }
	inline static int64 Cmp(int64 c2, float32 c) { return -1; }
	inline static int64 Cmp(int64 c2, float64 c) { return -1; }
	inline static int64 Cmp(int64 c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(int64 c2, const Date& c) { return -1; }
	inline static int64 Cmp(int64 c2, const String& c) { return -1; }
	inline static int64 Cmp(int64 c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(int64 c2, const IConvertable& c) { int64 i; return c.ToInt64(i) ? c2 - i : -1; }

	inline static int64 Cmp(uint32 c2, const Null& c) { return 1; }
	inline static int64 Cmp(uint32 c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(uint32 c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(uint32 c2, char c) { return (int64)(c2 - (uint32)c); }
	inline static int64 Cmp(uint32 c2, bool c) { return 1; }
	inline static int64 Cmp(uint32 c2, byte c) { return (int64)(c2 - (uint32)c); }
	inline static int64 Cmp(uint32 c2, int16 c) { return (int64)(c2 - (uint32)c); }
	inline static int64 Cmp(uint32 c2, int32 c) { return (int64)(c2 - (uint32)c); }
	inline static int64 Cmp(uint32 c2, int64 c) { return (int64)((int64)c2 - c); }
	inline static int64 Cmp(uint32 c2, uint32 c) { return (int64)(c2 - c); }
	inline static int64 Cmp(uint32 c2, uint64 c) { return (int64)((uint64)c2 - c); }
	inline static int64 Cmp(uint32 c2, BigInteger& c) { return (int64)(c.ToInt() - (int64)c2); }
	inline static int64 Cmp(uint32 c2, float32 c) { return -1; }
	inline static int64 Cmp(uint32 c2, float64 c) { return -1; }
	inline static int64 Cmp(uint32 c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(uint32 c2, const Date& c) { return -1; }
	inline static int64 Cmp(uint32 c2, const String& c) { return -1; }
	inline static int64 Cmp(uint32 c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(uint32 c2, const IConvertable& c) { uint32 i; return c.ToUInt32(i) ? (int64)c2 - (int64)i : -1; }

	inline static int64 Cmp(uint64 c2, const Null& c) { return 1; }
	inline static int64 Cmp(uint64 c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(uint64 c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(uint64 c2, char c) { return (int64)(c2 - (uint64)c); }
	inline static int64 Cmp(uint64 c2, bool c) { return 1; }
	inline static int64 Cmp(uint64 c2, byte c) { return (int64)(c2 - (uint64)c); }
	inline static int64 Cmp(uint64 c2, int16 c) { return (int64)(c2 - (uint64)c); }
	inline static int64 Cmp(uint64 c2, int32 c) { return (int64)(c2 - (uint64)c); }
	inline static int64 Cmp(uint64 c2, int64 c) { return (int64)(c2 - (uint64)c); }
	inline static int64 Cmp(uint64 c2, uint32 c) { return (int64)(c2 - (uint64)c); }
	inline static int64 Cmp(uint64 c2, uint64 c) { return (int64)(c2 - c); }
	inline static int64 Cmp(uint64 c2, BigInteger& c) { return (int64)(c.ToInt() - (int64)c2); }
	inline static int64 Cmp(uint64 c2, float32 c) { return -1; }
	inline static int64 Cmp(uint64 c2, float64 c) { return -1; }
	inline static int64 Cmp(uint64 c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(uint64 c2, const Date& c) { return -1; }
	inline static int64 Cmp(uint64 c2, const String& c) { return -1; }
	inline static int64 Cmp(uint64 c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(uint64 c2, const IConvertable& c) { uint64 i; return c.ToUInt64(i) ? (int64)c2 - (int64)i : -1; }

	inline static int64 Cmp(float32 c2, const Null& c) { return 1; }
	inline static int64 Cmp(float32 c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(float32 c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(float32 c2, char c) { return 1; }
	inline static int64 Cmp(float32 c2, bool c) { return 1; }
	inline static int64 Cmp(float32 c2, byte c) { return 1; }
	inline static int64 Cmp(float32 c2, int16 c) { return c2 > (float32)c ? 1 : (c2 == (float32)c ? 0 : -1); }
	inline static int64 Cmp(float32 c2, int32 c) { return c2 > (float32)c ? 1 : (c2 == (float32)c ? 0 : -1); }
	inline static int64 Cmp(float32 c2, int64 c) { return c2 > (float32)c ? 1 : (c2 == (float32)c ? 0 : -1); }
	inline static int64 Cmp(float32 c2, uint32 c) { return c2 > (float32)c ? 1 : (c2 == (float32)c ? 0 : -1); }
	inline static int64 Cmp(float32 c2, uint64 c) { return c2 > (float32)c ? 1 : (c2 == (float32)c ? 0 : -1); }
	inline static int64 Cmp(float32 c2, BigInteger& c) { return 1; }
	inline static int64 Cmp(float32 c2, float32 c) { return c2 > c ? 1 : (c2 == c ? 0 : -1); }
	inline static int64 Cmp(float32 c2, float64 c) { return c2 > (float32)c ? 1 : (c2 == (float32)c ? 0 : -1); }
	inline static int64 Cmp(float32 c2, const DateTime& c) { return -1; }
	inline static int64 Cmp(float32 c2, const Date& c) { return -1; }
	inline static int64 Cmp(float32 c2, const String& c) { return -1; }
	inline static int64 Cmp(float32 c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(float32 c2, const IConvertable& c) { float32 i; return c.ToFloat32(i) ? (int64)(c2 - i) : -1; }

	inline static int64 Cmp(float64 c2, const Null& c) { return 1; }
	inline static int64 Cmp(float64 c2, const IComparable& c) { return -1; }
	inline static int64 Cmp(float64 c2, const RefCountPtr<IComparable> c) { return -1; }
	inline static int64 Cmp(float64 c2, char c) { return 1; }
	inline static int64 Cmp(float64 c2, bool c) { return 1; }
	inline static int64 Cmp(float64 c2, byte c) { return 1; }
	inline static int64 Cmp(float64 c2, int16 c) { return c2 > (float64)c ? 1 : (c2 == (float64)c ? 0 : -1); }
	inline static int64 Cmp(float64 c2, int32 c) { return c2 > (float64)c ? 1 : (c2 == (float64)c ? 0 : -1); }
	inline static int64 Cmp(float64 c2, int64 c) { return c2 > (float64)c ? 1 : (c2 == (float64)c ? 0 : -1); }
	inline static int64 Cmp(float64 c2, uint32 c) { return c2 > (float64)c ? 1 : (c2 == (float64)c ? 0 : -1); }
	inline static int64 Cmp(float64 c2, uint64 c) { return c2 > (float64)c ? 1 : (c2 == (float64)c ? 0 : -1); }
	inline static int64 Cmp(float64 c2, BigInteger& c) { return 1; }
	inline static int64 Cmp(float64 c2, float32 c) { return c2 > (float64)c ? 1 : (c2 == (float64)c ? 0 : -1); }
	inline static int64 Cmp(float64 c2, float64 c) { return c2 > c ? 1 : (c2 == c ? 0 : -1); }
	inline static int64 Cmp(float64 c2, const DateTime& c) { return false; }
	inline static int64 Cmp(float64 c2, const Date& c) { return false; }
	inline static int64 Cmp(float64 c2, const String& c) { return false; }
	inline static int64 Cmp(float64 c2, const StringPtr& c) { return false; }
	inline static int64 Cmp(float64 c2, const IConvertable& c) { float64 i; return c.ToFloat64(i) ? (int64)(c2 - i) : -1; }

	inline static int64 Cmp(const DateTime& c2, const Null& c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, const IComparable& c) { return c2.Compare(c); }
	inline static int64 Cmp(const DateTime& c2, const RefCountPtr<IComparable> c) { return c2.Compare(*c); }
	inline static int64 Cmp(const DateTime& c2, char c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, bool c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, byte c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, int16 c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, int32 c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, int64 c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, uint32 c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, uint64 c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, BigInteger& c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, float32 c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, float64 c) { return 1; }
	inline static int64 Cmp(const DateTime& c2, const DateTime& c) { return c2.Compare(c); }
	inline static int64 Cmp(const DateTime& c2, const Date& c) { return c2.Compare(c); }
	inline static int64 Cmp(const DateTime& c2, const String& c) { return -1; }
	inline static int64 Cmp(const DateTime& c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(const DateTime& c2, const IConvertable& c) { DateTime i; return c.ToDateTime(i) ? c2.DiffInSeconds(i) : -1; }

	inline static int64 Cmp(const Date& c2, const Null& c) { return 1; }
	inline static int64 Cmp(const Date& c2, const IComparable& c) { return c2.Compare(c); }
	inline static int64 Cmp(const Date& c2, const RefCountPtr<IComparable> c) { return c2.Compare(*c); }
	inline static int64 Cmp(const Date& c2, char c) { return 1; }
	inline static int64 Cmp(const Date& c2, bool c) { return 1; }
	inline static int64 Cmp(const Date& c2, byte c) { return 1; }
	inline static int64 Cmp(const Date& c2, int16 c) { return 1; }
	inline static int64 Cmp(const Date& c2, int32 c) { return 1; }
	inline static int64 Cmp(const Date& c2, int64 c) { return 1; }
	inline static int64 Cmp(const Date& c2, uint32 c) { return 1; }
	inline static int64 Cmp(const Date& c2, uint64 c) { return 1; }
	inline static int64 Cmp(const Date& c2, BigInteger& c) { return 1; }
	inline static int64 Cmp(const Date& c2, float32 c) { return 1; }
	inline static int64 Cmp(const Date& c2, float64 c) { return 1; }
	inline static int64 Cmp(const Date& c2, const DateTime& c) { return c2.Compare(c); }
	inline static int64 Cmp(const Date& c2, const Date& c) { return c2.Compare(c); }
	inline static int64 Cmp(const Date& c2, const String& c) { return -1; }
	inline static int64 Cmp(const Date& c2, const StringPtr& c) { return -1; }
	inline static int64 Cmp(const Date& c2, const IConvertable& c) { Date i; return c.ToDate(i) ? c2.DiffInDays(i) : -1; }

	inline static int64 Cmp(const String& c2, const Null& c) { return 1; }
	inline static int64 Cmp(const String& c2, const IComparable& c) { return c2.Compare(c); }
	inline static int64 Cmp(const String& c2, const RefCountPtr<IComparable> c) { return c2.Compare(*c); }
	inline static int64 Cmp(const String& c2, char c) { return 1; }
	inline static int64 Cmp(const String& c2, bool c) { return 1; }
	inline static int64 Cmp(const String& c2, byte c) { return 1; }
	inline static int64 Cmp(const String& c2, int16 c) { return 1; }
	inline static int64 Cmp(const String& c2, int32 c) { return 1; }
	inline static int64 Cmp(const String& c2, int64 c) { return 1; }
	inline static int64 Cmp(const String& c2, uint32 c) { return 1; }
	inline static int64 Cmp(const String& c2, uint64 c) { return 1; }
	inline static int64 Cmp(const String& c2, BigInteger& c) { return 1; }
	inline static int64 Cmp(const String& c2, float32 c) { return 1; }
	inline static int64 Cmp(const String& c2, float64 c) { return 1; }
	inline static int64 Cmp(const String& c2, const DateTime& c) { return 1; }
	inline static int64 Cmp(const String& c2, const Date& c) { return 1; }
	inline static int64 Cmp(const String& c2, const String& c) { return c2.Compare(c); }
	inline static int64 Cmp(const String& c2, const StringPtr& c) { return c2.Compare(*c); }
	inline static int64 Cmp(const String& c2, const IConvertable& c) { String i; return c.ToString(i) ? c2.Compare(i) : -1; }

	inline static int64 Cmp(const StringPtr& c2, const Null& c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, const IComparable& c) { return c2->Compare(c); }
	inline static int64 Cmp(const StringPtr& c2, const RefCountPtr<IComparable> c) { return c2->Compare(*c); }
	inline static int64 Cmp(const StringPtr& c2, char c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, bool c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, byte c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, int16 c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, int32 c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, int64 c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, uint32 c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, uint64 c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, BigInteger& c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, float32 c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, float64 c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, const DateTime& c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, const Date& c) { return 1; }
	inline static int64 Cmp(const StringPtr& c2, const String& c) { return c2->Compare(c); }
	inline static int64 Cmp(const StringPtr& c2, const StringPtr& c) { return c2->Compare(*c); }
	inline static int64 Cmp(const StringPtr& c2, const IConvertable& c) { String i; return c.ToString(i) ? c2->Compare(i) : -1; }

	inline static int64 Cmp(const IConvertable & c2, const Null& c) { return 1; }
	inline static int64 Cmp(const IConvertable & c2, const IComparable& c) { return 1; }
	inline static int64 Cmp(const IConvertable & c2, const RefCountPtr<IComparable> c) { return 1; }
	inline static int64 Cmp(const IConvertable & c2, char c) { char ch; return c2.ToChar(ch) ? ch - c : 1; }
	inline static int64 Cmp(const IConvertable & c2, bool c) { bool b; return c2.ToBool(b) ? (b == c ? 0 : (b ? 1 : -1)) : 1; }
	inline static int64 Cmp(const IConvertable & c2, byte c) { byte b; return c2.ToByte(b) ? b - c : 1; }
	inline static int64 Cmp(const IConvertable & c2, int16 c) { int16 i; return c2.ToInt16(i) ? i - c : 1; }
	inline static int64 Cmp(const IConvertable & c2, int32 c) { int32 i; return c2.ToInt32(i) ? i - c : 1; }
	inline static int64 Cmp(const IConvertable & c2, int64 c) { int64 i; return c2.ToInt64(i) ? i - c : 1; }
	inline static int64 Cmp(const IConvertable & c2, uint32 c) { uint32 i; return c2.ToUInt32(i) ? (i == c ? 0 : (i > c ? 1 : -1)) : 1; }
	inline static int64 Cmp(const IConvertable & c2, uint64 c) { uint64 i; return c2.ToUInt64(i) ? (i == c ? 0 : (i > c ? 1 : -1)) : 1; ; }
	inline static int64 Cmp(const IConvertable & c2, BigInteger& c) { return 1; }
	inline static int64 Cmp(const IConvertable & c2, float32 c) { float32 d; return c2.ToFloat32(d) ? (int64)(d - c) : 1; }
	inline static int64 Cmp(const IConvertable & c2, float64 c) { float64 d; return c2.ToFloat64(d) ? (int64)(d - c) : 1; }
	inline static int64 Cmp(const IConvertable & c2, const DateTime& c) { DateTime dtm; return c2.ToDateTime(dtm) ? dtm.DiffInSeconds(c) : 1; }
	inline static int64 Cmp(const IConvertable & c2, const Date& c) { Date dt; return c2.ToDate(dt) ? dt.DiffInDays(c) : 1; }
	inline static int64 Cmp(const IConvertable & c2, const String& c) { String s; return c2.ToString(s) ? s.Compare(c) : 1; }
	inline static int64 Cmp(const IConvertable & c2, const StringPtr& c) { String s; return c2.ToString(s) ? s.Compare(*c) : 1; }
	inline static int64 Cmp(const IConvertable & c2, const IConvertable& c) { return c2.Compare(c); }
};

/** @} */
}
#endif
