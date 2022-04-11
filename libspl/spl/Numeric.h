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
#ifndef _numeric_h
#define _numeric_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>

namespace spl
{
/**
 * @defgroup types Types
 * @ingroup core
 * @{
 */

class Numeric;
typedef RefCountPtr<Numeric> NumericPtr;

/** @brief Base class for all numeric type classes.
 *
 */
class Numeric : public IComparable, public IMemoryValidate
{
protected:
	Numeric();

public:
	virtual ~Numeric();

	virtual uint32 HashCode() const = 0;

	virtual bool Equals( const IComparable *a ) const = 0;
	virtual int Compare( const IComparable *a ) const = 0;
	virtual bool Equals( const IComparable& a ) const = 0;
	virtual int Compare( const IComparable& a ) const = 0;
	virtual int32 MajicNumber() const = 0;

	virtual int ToInt() const = 0;
	virtual double ToDouble() const = 0;
	virtual StringPtr ToString() const = 0;

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#endif

	static bool IsNumeric(const char *cp, const int cplen);
	inline static bool IsNumeric(const String& str) { return IsNumeric(str.GetChars(), str.Length()); }
	inline static bool IsNumeric(const String *str) { return IsNumeric(str->GetChars(), str->Length()); }
};

/** @} */
}
#endif
