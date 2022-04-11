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
#ifndef _iserializable_h
#define _iserializable_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/StringBuffer.h>
#include <spl/io/IStream.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

/** @brief Interface to allow your class to be sent and received through a stream.
 *	@todo I don't this this is fully supported.
 */
class ISerializable
{
public:
	virtual ~ISerializable();
	virtual void Serialize(IStream& strm) = 0;
	virtual void Serialize(StringBuffer& buf) = 0;
	virtual int SerializationBinaryUniqueKey() = 0;
	virtual void SerializationTextUniqueKey(String& buf) = 0;
};

REGISTER_TYPEOF(100, ISerializable);

/** @} */
}
#endif
