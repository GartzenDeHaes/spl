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
#ifndef _guid_h
#define _guid_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define GUID_SIZE_BYTES 16

/** @brief A Globally Unique Identifier.
 *	This does not use microsoft's algorithm, but is similiar to type 4 UUID.  However,
 *	it does not have a '4' in the required position.
 */
class Guid
{
protected:
	uint32 data1;
	uint16 data2;
	uint16 data3;
	uint64 data4;

public:
	Guid();
	Guid( const Array<byte>& bytes );
	Guid( const Guid& guid );
	~Guid();

	inline static int Length() { return GUID_SIZE_BYTES; }

	void GetBytes( Array<byte>& bytes ) const;
	Array<byte> GetBytes() const;
	StringPtr ToBase64();
	StringPtr ToHex();

	inline bool Equals( const Guid& g ) { return data1 == g.data1 && data2 == g.data2 && data3 == g.data3 && data4 == g.data4; }
	
	static Guid ParseBase64(const String& str);
	static Guid ParseHex(const String& str);
};

inline void TypeValidate ( Guid *guid )
{
	DEBUG_NOTE_MEM(guid);
}

inline void TypeCheck ( Guid *guid )
{
	DEBUG_NOTE_MEM(guid);
}

/** @} */
}
#endif
