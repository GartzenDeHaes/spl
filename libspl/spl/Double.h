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
#ifndef _double_h
#define _double_h

#include <spl/Numeric.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define DOUBLE_MAJIC 0x0014		//< Majic number for ASSERT's in Compare and Convert
#define FLOAT_MAJIC 0x1000		//< Majic number for ASSERT's in Compare and Convert

/** @brief double helper type.
 *
 */
class Double : public Numeric
{
private:
	double m_val;

public:
	Double();
	Double(const double d);
	Double(const Double& d);
	virtual ~Double();

	virtual uint32 HashCode() const;

	virtual bool Equals(const IComparable *a) const;
	virtual int Compare(const IComparable *a) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	virtual int ToInt() const;
	virtual double ToDouble() const;
	virtual StringPtr ToString() const;

	inline operator double()
	{
		return m_val;
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

    /**
     * Convert the argument to a double.
     * 
	 * @throws InvalidArgumentException if the argument cannot be parsed.
     * @param cp string representation of a floating point number.
	 * @param cplen the length of cp in bytes.
     * @return True if Parse() will succeed.
     */
	static float64 Parse(const char *cp, const int cplen);

    /**
     * Convert the argument to a double.
     * 
	 * @throws InvalidArgumentException if the argument cannot be parsed.
     * @param str string representation of a floating point number.
     * @return True if Parse() will succeed.
     */
	inline static float64 Parse(const String& str) { return Parse(str.GetChars(), str.Length()); }

    /**
     * Convert the argument to a double.
     * 
	 * @throws InvalidArgumentException if the argument cannot be parsed.
     * @param i string representation of a floating point number.
     * @return True if Parse() will succeed.
     */
	inline static float64 Parse(const String *str) { return Parse(str->GetChars(), str->Length()); }

    /**
     * Test if the argument can be parsed to a double.
     * 
     * @param cp string representation of a floating point number.
	 * @param cplen the length of cp.
     * @return True if Parse() will succeed.
     */
	static bool IsDouble(const char *cp, const int cplen);

    /**
     * Test if the argument can be parsed to a double.
     * 
     * @param str string representation of a floating point number.
     * @return True if Parse() will succeed.
     */
	inline static bool IsDouble(const String& str) { return IsDouble(str.GetChars(), str.Length()); }

    /**
     * Test if the argument can be parsed to a double.
     * 
     * @param str string representation of a floating point number.
     * @return True if Parse() will succeed.
     */
	inline static bool IsDouble(const String *str) { return IsDouble(str->GetChars(), str->Length()); }

    /**
     * Test if the argument can be parsed to a float.
     * 
     * @param cp string representation of a floating point number.
	 * @param cplen the length of cp.
     * @return True if Parse() will succeed.
     */
	inline static bool IsFloat(const char *cp, const int cplen) { return IsDouble(cp, cplen); }

    /**
     * Test if the argument can be parsed to a float.
     * 
     * @param str string representation of a floating point number.
     * @return True if Parse() will succeed.
     */
	inline static bool IsFloat(const String& str) { return IsDouble(str.GetChars(), str.Length()); }

    /**
     * Test if the argument can be parsed to a float.
     * 
     * @param str string representation of a floating point number.
     * @return True if Parse() will succeed.
     */
	inline static bool IsFloat(const String *str) { return IsDouble(str->GetChars(), str->Length()); }

    /**
     * Creates a string representation of the value
     * 
     * @param i target number
     * @return A string representation of the argument.  Trailing zeros are truncated.
     */
	static StringPtr ToString(float64 i);

    /**
     * Creates a string representation of the value
     * 
     * @param i target number
     * @return A string representation of the argument.  Trailing zeros are truncated.
     */
	inline static StringPtr ToString(float32 i) { return ToString((float64)i); }

    /**
     * Determine if target is not a number
     * 
     * @param d target number
     * @return true if target represents the "not a number" value
     */
	static bool IsNaN(double d);

    /**
     * Determine if target is positive 0.
     * 
     * @param d target number
     * @return true if target represents the value for positive 0.
     */
	static bool IsPositiveZero(double d);

    /**
     * Determine if target is negative 0
     * 
     * @param d target number
     * @return true if target represents the value for negative 0
     */
	static bool IsNegativeZero(double d);

	/**
     * Determine if target is positive 0.
     * 
     * @param d target number
     * @return true if target represents the value for positive 0.
     */
	static double PositiveInfinity();

    /**
     * Determine if target is negative infinity
     * 
     * @param d target number
     * @return true if target represents the value for negative infinity
     */
	static double NegativeInfinity();

    /**
     * Determine if target is positive infinity
     * 
     * @param d target number
     * @return true if target represents the value for positive infinity
     */
	inline static bool IsPositiveInfinity(double d)
	{
		return d == PositiveInfinity();
	}

    /**
     * Determine if target is negative infinity
     * 
     * @param d target number
     * @return true if target represents the value for negative infinity
     */
	inline static bool IsNegativeInfinity(double d)
	{
		return d == NegativeInfinity();
	}
};

/** @} */
}
#endif
