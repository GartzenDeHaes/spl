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
#ifndef _null_h
#define _null_h

#include <spl/types.h>
#include <spl/Debug.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define NULL_MAJIC 0		//< Majic number for ASSERT's in Compare and Convert

/** @brief This class is for a Variant type.
 *	@ref Variant
 */
class Null : public IComparable
{
protected:
	static Null m_instance;

public:
	Null();
	virtual ~Null();

	inline Null(const Null& n) {}
	inline Null& operator =(const Null& n) { return *this; }

	virtual bool Equals( const IComparable *a ) const;
	virtual int Compare( const IComparable *a ) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;

	///@brief Class instances with the same majic number are of the same type (can be casted).
	/// Majic numbers above 0xFFFF are available for user applications.
	virtual int32 MajicNumber() const;

	virtual uint32 HashCode() const;

	inline operator void *() const { return NULL; }

	inline static Null& Instance() { return m_instance; }
};

inline void TypeValidate( const Null& u ) {}
inline void TypeValidate( const Null *u ) {}
inline void TypeCheckMem( const Null& u ) {}
inline void TypeCheckMem( const Null *u ) {}

REGISTER_TYPEOF( 571, Null );

/** @} */
}
#endif
