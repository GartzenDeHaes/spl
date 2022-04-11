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
#ifndef _decimal_h
#define _decimal_h

#include <spl/types.h>
#include <spl/Numeric.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define DECIMAL_MAJIC 0x1234		//< Majic number for ASSERT's in Compare and Convert

/** @brief Fixed point decimal helper.
 *
 */
class Decimal : public Numeric
{
protected:
	int64 n;

	static const int precision;
	static const int64 q;
	
	inline static Decimal Intern(int64 n) { Decimal dec(0); dec.n = n; return dec; }

	Decimal(int64 intPart, int64 fractPart);
	
	inline void Init(int64 intPart, int64 fractPart)
	{
		n=intPart;
		n*=q;
		n+=fractPart;
	}

public:
	static void Initialize();

	Decimal(const int64 i);
	Decimal(const int32 i);
	Decimal(const int16 i);
	Decimal(void);
	Decimal(const Decimal& d);
	Decimal(const double d);
	Decimal(const float d);

	virtual ~Decimal(void);

	Decimal Round();

	Decimal& operator =(const Decimal& d);
	Decimal& operator =(const int32 d);
	Decimal& operator =(const int64 d);
	Decimal& operator =(const float d);
	Decimal& operator =(const double d);
	Decimal operator +(const Decimal& d) const;
	Decimal operator -(const Decimal& d) const;
	Decimal operator *(const Decimal& d) const;
	Decimal operator /(const Decimal& d) const;

	Decimal operator +=(const Decimal& d);
	Decimal operator -=(const Decimal& d);
	Decimal operator *=(const Decimal& d);
	Decimal operator /=(const Decimal& d);

	bool operator ==(const Decimal&) const;
	bool operator !=(const Decimal&) const;
	bool operator <(const Decimal&) const;
	bool operator <=(const Decimal&) const;
	bool operator >(const Decimal&) const;
	bool operator >=(const Decimal&) const;

	virtual int ToInt() const;
	virtual double ToDouble() const;
	virtual StringPtr ToString() const;
	inline int64 ToLongInt(void) const { return (int64)(n/q); }

	virtual uint32 HashCode() const;

	inline Decimal Abs() { if ( n < 0 ) return Decimal(n * -1); return Decimal(n); }

	inline int64 RawData() const { return n; }
	inline static Decimal Parse( const String *str ) { return Parse(*str); }
	static Decimal Parse( const String& str );

	bool Equals(Decimal *d) const;
	virtual bool Equals(const IComparable *a) const;
	virtual int Compare(const IComparable *a) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif

