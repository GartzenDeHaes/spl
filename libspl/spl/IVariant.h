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
#ifndef _ivariant_h
#define _ivariant_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Compare.h>
#include <spl/DateTime.h>
#include <spl/Decimal.h>
#include <spl/Exception.h>
#include <spl/math/Math.h>
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

/** @brief The purpose of this class is to allow JsObject to be a variant type, and to allow Variant to be the JsObject's property type.
 *
 */
class IVariant : public IConvertable
{
public:
	IVariant();
	~IVariant();

	virtual bool ToBool() const = 0;
	virtual byte ToByte() const = 0;
	virtual int16 ToInt16() const = 0;
	virtual int32 ToInt32() const = 0;
	virtual int64 ToInt64() const = 0;
	virtual float32 ToFloat32() const = 0;
	virtual float64 ToFloat64() const = 0;
	virtual DateTime ToDateTime() const = 0;
	virtual Date ToDate() const = 0;
	virtual Decimal ToDecimal() const = 0;
	virtual StringPtr ToString() const = 0;

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

	virtual bool Equals( const IComparable& a ) const = 0;
	virtual int Compare( const IComparable& a ) const = 0;
	virtual int32 MajicNumber() const = 0;
	virtual uint32 HashCode() const = 0;

	virtual bool IsObject() const = 0;

	virtual String TypeName() const = 0;

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#endif
};

/** @} */
}
#endif
