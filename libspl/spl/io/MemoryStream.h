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
#ifndef _memorystream_h
#define _memorystream_h

#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/io/IStream.h>
#include <spl/RefCountPtr.h>
#include <spl/collection/Vector.h>
#include <spl/WeakReference.h>

namespace spl
{
/**
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class MemoryStream;
typedef RefCountPtrCast<MemoryStream, IStream, IStreamPtr> MemoryStreamPtr;
typedef WeakReference<MemoryStream, IStreamPtr> MemoryStreamRef;

REGISTER_TYPEOF(110, MemoryStreamPtr);
REGISTER_TYPEOF( 459, MemoryStreamRef );

/** @brief A stream for operating on memory buffers.
 *
 */
class MemoryStream : public IStream
{
protected:
	// not excessively efficient
	Vector<byte> m_buf;
	int m_ptr;

public:
	MemoryStream();

	inline MemoryStream(const MemoryStream& ms)
	: IStream(), m_buf(ms.m_buf), m_ptr(ms.m_ptr)
	{
	}

	virtual ~MemoryStream();

	inline MemoryStream& operator =(const MemoryStream &ms)
	{
		m_buf = ms.m_buf;
		m_ptr = ms.m_ptr;
		return *this;
	}

	virtual void Close();
	virtual void Flush();
	virtual int Read(Array<byte>& buffer, const int offset, int count);
	inline int Read(Array<byte>& buffer) { return Read(buffer, 0, buffer.Length()); }
	virtual int ReadByte();
	virtual long Seek(const long offset, const SeekOrigin origin);
	virtual void Write(const Array<byte>& buffer, const int offset, const int count);
	inline void Write(const Array<byte>& buffer) { Write(buffer, 0, buffer.Length()); }
	virtual void WriteByte(byte value);

	virtual bool CanRead() const;
	virtual bool CanSeek() const;
	virtual bool CanWrite() const;

	virtual long Length() const;
	virtual long Position() const;

	StringPtr ToString();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF(112, MemoryStream);

/** @} */
}
#endif
