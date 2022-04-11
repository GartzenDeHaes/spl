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
#ifndef _iconvertable_h
#define _iconvertable_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/BigInteger.h>
#include <spl/Char.h>
#include <spl/Date.h>
#include <spl/DateTime.h>
#include <spl/Decimal.h>
#include <spl/Double.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
#include <spl/math/Math.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/UInt32.h>
#include <spl/UInt64.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

/** @brief Implement this interface to make your class compatable with Convert.
 *	@ref Convert
 */
class IConvertable : public IComparable, public IMemoryValidate
{
public:
	IConvertable();
	virtual ~IConvertable();

	virtual bool ToChar(char& c2) const = 0;

	virtual bool ToBool(bool& c2) const = 0;

	virtual bool ToByte(byte& c2) const = 0;

	virtual bool ToInt16(int16& c2) const = 0;

	virtual bool ToInt32(int32& c2) const = 0;

	virtual bool ToInt64(int64& c2) const = 0;

	virtual bool ToUInt32(uint32& c2) const = 0;

	virtual bool ToUInt64(uint64& c2) const = 0;

	//virtual bool ToBigInteger(const BigInteger& c2) const = 0;

	virtual bool ToFloat32(float32& c2) const = 0;

	virtual bool ToFloat64(float64& c2) const = 0;

	virtual bool ToDateTime(DateTime& c2) const = 0;

	virtual bool ToDate(Date& c2) const = 0;

	virtual bool ToDecimal(Decimal& c2) const = 0;

	virtual bool ToString(String& c2) const = 0;

	virtual String TypeName() const = 0;

	virtual bool Equals( const IComparable& a ) const = 0;
	virtual int Compare( const IComparable& a ) const = 0;

	///@brief Class instances with the same majic number are of the same type (can be casted).
	/// Majic numbers above 0xFFFF are available for user applications.
	virtual int32 MajicNumber() const = 0;

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#endif
};

/** @} */
}
#endif
