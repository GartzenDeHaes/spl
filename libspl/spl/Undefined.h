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
#ifndef _undefined_h
#define _undefined_h

#include <spl/types.h>
#include <spl/Debug.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define UNDEFINED_MAJIC 404		//< Majic number for ASSERT's in Compare and Convert

/** @brief For an unset Variant
 *	@ref Variant
 */
class Undefined : public IComparable
{
protected:
	static Undefined m_instance;

public:
	Undefined();
	virtual ~Undefined();

	virtual bool Equals( const IComparable *a ) const;
	virtual int Compare( const IComparable *a ) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;

	///@brief Class instances with the same majic number are of the same type (can be casted).
	/// Majic numbers above 0xFFFF are available for user applications.
	virtual int32 MajicNumber() const;

	virtual uint32 HashCode() const;

	inline static Undefined& Instance() { return m_instance; }
};

inline void TypeValidate( const Undefined& u ) {}
inline void TypeValidate( const Undefined *u ) {}
inline void TypeCheckMem( const Undefined& u ) {}
inline void TypeCheckMem( const Undefined *u ) {}


REGISTER_TYPEOF( 580, Undefined );

/** @} */
}
#endif
