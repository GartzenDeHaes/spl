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
#ifndef _int32_h
#define _int32_h

#include <spl/types.h>
#include <spl/Numeric.h>
#include <spl/String.h>

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define INT32_MAJIC 0x0010		//< Majic number for ASSERT's in Compare and Convert

/** @brief 32-bit integer helper.
 *
 */
class Int32 : public Numeric
{
private:
	int32 m_val;

public:
	inline Int32()
	: m_val(0)
	{
	}

	inline Int32(const Int32& i)
	: m_val(i.m_val)
	{

	}	
	inline Int32(const int32 i)
	: m_val(i)
	{
	}
	
	virtual ~Int32();

	virtual uint32 HashCode() const;

	virtual bool Equals(const IComparable *a) const;
	virtual int Compare(const IComparable *a) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	virtual int ToInt() const;
	virtual double ToDouble() const;
	virtual StringPtr ToString() const;

	inline operator int32() const
	{
		return m_val;
	}
	
	inline bool operator ==(const int32 i)
	{
		return m_val == i;
	}

	inline bool operator !=(const int32 i)
	{
		return m_val != i;
	}
	
	inline bool operator >(const int32 i)
	{
		return m_val > i;
	}
	
	inline bool operator <(const int32 i)
	{
		return m_val < i;
	}
	
	inline bool operator >=(const int32 i)
	{
		return m_val >= i;
	}
	
	inline bool operator <=(const int32 i)
	{
		return m_val <= i;
	}
	
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

	static int32 Parse(const char *cp, const int cplen, int radix);
	inline static int32 Parse(const char *cp, const int cplen) { return Parse(cp, cplen, 10); }
	inline static int32 Parse(const String& str) { return Parse(str.GetChars(), str.Length(), 10); }
	inline static int32 Parse(const String *str) { return Parse(str->GetChars(), str->Length(), 10); }
	inline static int32 Parse(const String& str, int radix) { return Parse(str.GetChars(), str.Length(), radix); }
	inline static int32 Parse(const String *str, int radix) { return Parse(str->GetChars(), str->Length(), radix); }

	static bool IsInt(const char *cp, const int cplen, int radix);	
	inline static bool IsInt(const char *cp, const int cplen) { return IsInt(cp, cplen, 10); }
	inline static bool IsInt(const String& str) { return IsInt(str.GetChars(), str.Length(), 10); }
	inline static bool IsInt(const String *str) { return IsInt(str->GetChars(), str->Length(), 10); }
	inline static bool IsInt(const String& str, int radix) { return IsInt(str.GetChars(), str.Length(), radix); }
	inline static bool IsInt(const String *str, int radix) { return IsInt(str->GetChars(), str->Length(), radix); }

	static StringPtr ToString(int32 i, int radix);
	inline static StringPtr ToString(int32 i) { return ToString(i, 10); }

	inline static int MaxValue() { return INT_MAX; }
	inline static int MinValue() { return INT_MIN; }
};

REGISTER_TYPEOF( 566, Int32 );

/** @} */
}
#endif
