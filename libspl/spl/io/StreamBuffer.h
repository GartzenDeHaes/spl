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
#include <spl/io/Stream.h>

#ifdef _stream2_h
/* _stream2_h is in Stream.h, only compile this file if using the new Stream.h header */

#ifndef _streambuffer_h
#define _streambuffer_h

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class StreamBuffer;
typedef RefCountPtrCast<StreamBuffer, spl::IStream, spl::IStreamPtr> StreamBufferPtr;

/// @brief Buffers reads and writes -- call Flush to force I/O on short packets.
class StreamBuffer : public spl::IStream
{
protected:
	spl::IStreamPtr m_strm;
	bool m_deleteStream;
	Array<byte> m_buf;
	int m_bufpos;
	int m_buflen;

public:
	StreamBuffer( spl::IStreamPtr strm, const int buflen = 512 );
	virtual ~StreamBuffer();

	/** Closes the underlying stream. */
	virtual void Close();
	
	/** Forces output, does NOT discard anything. */
	virtual void Flush();
	
	/** Read the requested buffer size; whether this blocks or not depends on the underlying stream. */
	virtual int Read(Array<byte>& buffer, const int offset, int count);

	/** Read a byte; whether this blocks or not depends on the underlying stream. */
	virtual int ReadByte();

	/** Seek -- may not be supported by the stream; whether this blocks or not depends on the underlying stream. */
	virtual long Seek(const long offset, const SeekOrigin origin);
	
	/** Write the buffer; whether this blocks or not depends on the underlying stream. */
	virtual void Write(const Array<byte>& buffer, const int offset, const int count);

	/** Write a byte; whether this blocks or not depends on the underlying stream. */
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

/** @} */
}
#endif
#endif
