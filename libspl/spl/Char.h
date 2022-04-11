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
#ifndef _character_h
#define _character_h

#include <ctype.h>
#include <wctype.h>
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

#define CHAR_MAJIC 0x0015		//< Majic number for ASSERT's in Compare and Convert

/** @brief char helper class
 *
 */
class Char : public Numeric
{
private:
	int m_val;

public:
	Char();
	Char(const Char& i);
	Char(const int i);
	virtual ~Char();

	virtual uint32 HashCode() const;

	virtual bool Equals(const IComparable *a) const;
	virtual int Compare(const IComparable *a) const;
	virtual bool Equals(const IComparable& a) const;
	virtual int Compare(const IComparable& a) const;
	virtual int32 MajicNumber() const;

	virtual int ToInt() const;
	virtual double ToDouble() const;
	virtual StringPtr ToString() const;

	inline operator char() { return (char)m_val; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

	inline static bool IsAscii(const int32 c) { return c < 256; }
	inline static bool IsControl(const int32 c) { return 0 != iswcntrl(c); }
	inline static bool IsDigit(const int32 c) { return 0 != iswdigit(c); }
	inline static bool IsLetter(const int32 c) { return 0 != iswalpha(c); }
	inline static bool IsLetterOrDigit(const int32 c) { return 0 != iswalnum(c); }
	inline static bool IsLower(const int32 c) { return 0 != iswlower(c); }
	inline static bool IsNumber(const int32 c) { return 0 != iswdigit(c); }
	inline static bool IsPunctuation(const int32 c) { return 0 != iswpunct(c); }
	inline static bool IsUpper(const int32 c) { return 0 != iswupper(c); }
	inline static bool IsPrint(const int32 c) { return 0 != isprint(c); }
	static bool IsWhiteSpace(const int32 c);
	inline static int32 ToUpper(const int32 c) { return towupper(c); }
	inline static int32 ToLower(const int32 c) { return towlower(c); }

	static StringPtr ToString(int32 i);
	static StringPtr ToString(char c);

	inline static int MaxValue() { return 255; }
	inline static int MinValue() { return 0; }
};

REGISTER_TYPEOF( 556, Char );

/** @} */
}
#endif
