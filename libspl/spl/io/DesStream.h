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
#ifndef _desstream_h
#define _desstream_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/math/Des.h>
#include <spl/io/Stream.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class DesStream;
typedef RefCountPtrCast<DesStream, spl::IStream, spl::IStreamPtr> DesStreamPtr;
typedef WeakReference<DesStream, spl::IStreamPtr> DesStreamRef;

REGISTER_TYPEOF(80, DesStreamPtr);
REGISTER_TYPEOF( 448, DesStreamRef );

/** @brief Applies DES on data written and read.
 *
 */
class DesStream : public spl::IStream
{
protected:
	DES m_des;
	String m_password;
	spl::IStreamPtr m_stream;
	bool m_ownStream;

public:
	DesStream(const String& password, spl::IStreamPtr stream);
	virtual ~DesStream();

	virtual void Close();
	virtual void Flush();
	virtual int Read(Array<byte>& buffer, const int offset, int count);
	virtual int ReadByte();
	virtual long Seek(const long offset, const SeekOrigin origin);
	virtual void Write(const Array<byte>& buffer, const int offset, const int count);
	virtual void WriteByte(byte value);

	virtual bool CanRead() const;
	virtual bool CanSeek() const;
	virtual bool CanWrite() const;

	virtual long Length() const;
	virtual long Position() const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF(82, DesStream);

/** @} */
}
#endif
