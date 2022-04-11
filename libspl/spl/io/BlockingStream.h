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
#ifndef _blockingstream_h
#define _blockingstream_h

#include <spl/io/Stream.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class BlockingStream;
typedef RefCountPtrCast<BlockingStream, spl::IStream, spl::IStreamPtr> BlockingStreamPtr;
typedef WeakReference<BlockingStream, spl::IStreamPtr> BlockingStreamRef;

REGISTER_TYPEOF(72, BlockingStreamPtr);
REGISTER_TYPEOF( 448, BlockingStreamRef );

/// @brief BlockingStream uses a mutex and an event to enforce blocking I/O.
class BlockingStream : public spl::IStream
{
protected:
	spl::IStreamPtr m_strm;
	bool m_deleteStream;
	Mutex m_datalock;
	Event m_dataready;

public:
	BlockingStream( spl::IStreamPtr strm );
	virtual ~BlockingStream();

	virtual int Read(Array<byte>& buffer, const int offset, int count);
	virtual int ReadByte();
	virtual void Write(const Array<byte>& buffer, const int offset, const int count);
	virtual void WriteByte(byte value);

	virtual void Close();
	virtual void Flush();
	virtual long Seek(const long offset, const SeekOrigin origin);

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

REGISTER_TYPEOF(74, BlockingStream);

/** @} */
}
#endif
