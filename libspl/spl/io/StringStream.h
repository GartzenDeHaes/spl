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
#ifndef _stringstream_h
#define _stringstream_h

#include <spl/Debug.h>
#include <spl/io/IStream.h>
#include <spl/io/MemoryStream.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class StringStream;
typedef RefCountPtrCast<StringStream, IStream, IStreamPtr> StringStreamPtr;
typedef WeakReference<StringStream, IStreamPtr> StringStreamRef;

REGISTER_TYPEOF(134, StringStreamPtr);
REGISTER_TYPEOF( 466, StringStreamRef );

/** @brief A MemoryStream that initializes from a string.
 *	@ref String
 */
class StringStream : public MemoryStream
{
protected:

public:
	StringStream(const String& str);
	virtual ~StringStream();

	inline StringStream& operator =(const StringStream &ms)
	{
		m_buf = ms.m_buf;
		m_ptr = ms.m_ptr;
		return *this;
	}
};

REGISTER_TYPEOF(136, StringStream);

/** @} */
}
#endif
