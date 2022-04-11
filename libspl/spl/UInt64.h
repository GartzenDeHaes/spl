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
#ifndef _uint64_h
#define _uint64_h

#include <spl/types.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
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

#define UINT64_MAJIC 0x013		//< Majic number for ASSERT's in Compare and Convert

/** @brief Unsigned 64-bit integer helper
 *
 */
class UInt64 : public Numeric
{
private:
	uint64 m_val;
public:
	UInt64();
	UInt64(const uint64 i);
	UInt64(const UInt64& i);
	virtual ~UInt64();

	virtual uint32 HashCode() const;

	virtual bool Equals(const IComparable *a) const;
	virtual int Compare(const IComparable *a) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	virtual int ToInt() const;
	virtual double ToDouble() const;
	virtual StringPtr ToString() const;

	inline operator uint64()
	{
		return m_val;
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

public:
	static uint64 Parse(const char *cp, const int cplen, int radix);
	inline static uint64 Parse(const char *cp, const int cplen) { return Parse(cp, cplen, 10); }
	inline static uint64 Parse(const String& str) { return Parse(str.GetChars(), str.Length(), 10); }
	inline static uint64 Parse(const String *str) { return Parse(str->GetChars(), str->Length(), 10); }
	inline static uint64 Parse(const String& str, int radix) { return Parse(str.GetChars(), str.Length(), radix); }
	inline static uint64 Parse(const String *str, int radix) { return Parse(str->GetChars(), str->Length(), radix); }

	inline static bool IsInt(const char *cp, const int cplen) { return cp[0] != '-' && cp[0] != '+' && Int64::IsInt(cp, cplen); }
	inline static bool IsInt(const String& str) { return IsInt(str.GetChars(), str.Length()); }
	inline static bool IsInt(const String *str) { return IsInt(str->GetChars(), str->Length()); }

	static StringPtr ToString(uint64 i, int radix);
	inline static StringPtr ToString(uint64 i) { return ToString(i, 10); }

	inline static uint64 MaxValue() 
	{ 
#ifdef ULLONG_MAX
		return ULLONG_MAX;
#elif _MSC_VER > 1200
		return IL_MAX_UINT64; 
#else
		return (int64)0x7FFFFFFFFFFFFFFF;
#endif
	}
	
	inline static uint64 MinValue() { return 0; }
};

REGISTER_TYPEOF( 578, UInt64 );

/** @} */
}
#endif
