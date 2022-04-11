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
#ifndef _uint32_h
#define _uint32_h

#include <spl/types.h>
#include <spl/Int32.h>
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

#define UINT32_MAJIC 0x0012		//< Majic number for ASSERT's in Compare and Convert

/** @brief Unsigned 32-bit integer helper.
 *
 */
class UInt32 : public Numeric
{
private:
	uint32 m_val;
public:
	UInt32();
	UInt32(const uint32 i);
	UInt32(const UInt32& i);
	virtual ~UInt32();

	virtual uint32 HashCode() const;

	virtual bool Equals(const IComparable *a) const;
	virtual int Compare(const IComparable *a) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	virtual int ToInt() const;
	virtual double ToDouble() const;
	virtual StringPtr ToString() const;

	inline operator uint32()
	{
		return m_val;
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

public:
	static uint32 Parse(const char *cp, const int cplen, int radix);
	inline static uint32 Parse(const char *cp, const int cplen) { return Parse(cp, cplen, 10); }
	inline static uint32 Parse(const String& str) { return Parse(str.GetChars(), str.Length(), 10); }
	inline static uint32 Parse(const String *str) { return Parse(str->GetChars(), str->Length(), 10); }
	inline static uint32 Parse(const String& str, int radix) { return Parse(str.GetChars(), str.Length(), radix); }
	inline static uint32 Parse(const String *str, int radix) { return Parse(str->GetChars(), str->Length(), radix); }

	inline static bool IsInt(const char *cp, const int cplen, int radix) { return cp[0] != '-' && cp[0] != '+' && Int32::IsInt(cp, cplen, radix); }
	inline static bool IsInt(const char *cp, const int cplen) { return cp[0] != '-' && IsInt(cp, cplen, 10); }
	inline static bool IsInt(const String& str) { return IsInt(str.GetChars(), str.Length(), 10); }
	inline static bool IsInt(const String *str) { return IsInt(str->GetChars(), str->Length(), 10); }
	inline static bool IsInt(const String& str, int radix) { return IsInt(str.GetChars(), str.Length(), radix); }
	inline static bool IsInt(const String *str, int radix) { return IsInt(str->GetChars(), str->Length(), radix); }

	static StringPtr ToString(uint32 i, int radix);
	inline static StringPtr ToString(uint32 i) { return ToString(i, 10); }

	inline static uint32 MaxValue() { return UINT_MAX; }
	inline static uint32 MinValue() { return 0; }
};

REGISTER_TYPEOF( 576, UInt32 );

/** @} */
}
#endif
