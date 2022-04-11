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
#ifndef _int64_h
#define _int64_h

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

#define INT64_MAJIC 0x0011		//< Majic number for ASSERT's in Compare and Convert

/** @brief 64-bit integer helper.
 *
 */
class Int64 : public Numeric
{
private:
	int64 m_val;
public:
	Int64();
	Int64(const int64 i);
	Int64(const Int64& i);
	virtual ~Int64();

	virtual uint32 HashCode() const;

	virtual bool Equals(const IComparable *a) const;
	virtual int Compare(const IComparable *a) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	virtual int ToInt() const;
	virtual double ToDouble() const;
	virtual StringPtr ToString() const;

	inline operator int64()
	{
		return m_val;
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

public:
	static int64 Parse(const char *cp, const int cplen);
	inline static int64 Parse(const String& str) { return Parse(str.GetChars(), str.Length()); }
	inline static int64 Parse(const String *str) { return Parse(str->GetChars(), str->Length()); }


	static bool IsInt(const char *cp, const int cplen);		
	inline static bool IsInt(const String& str) { return IsInt(str.GetChars(), str.Length()); }
	inline static bool IsInt(const String *str) { return IsInt(str->GetChars(), str->Length()); }

	static StringPtr ToString(int64 i, int numberBase);
	inline static StringPtr ToString(int64 i) { return ToString(i, 10); }

	inline static int64 MaxValue() 
	{ 
#ifdef LLONG_MAX
		return LLONG_MAX; 
#elif _MSC_VER > 1200
		return IL_MAX_INT64;
#else
		return 9223372036854775807;
#endif
	}
	
	inline static int64 MinValue() 
	{ 
#ifdef LLONG_MIN
		return LLONG_MIN; 
#elif _MSC_VER > 1200
		return IL_MIN_INT64;
#else
		return -int64(9223372036854775808);
#endif
	}
};

/** @} */
}
#endif
