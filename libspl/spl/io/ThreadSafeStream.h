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
#ifndef _threadsafestream_h
#define _threadsafestream_h

#include <stdio.h>

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/threading/Event.h>
#include <spl/threading/Mutex.h>
#include <spl/io/IStream.h>
#include <spl/Memory.h>
#include <spl/RefCountPtrCast.h>
#include <spl/io/Stream.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup io IO
 * @{
 */

 /** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class ThreadSafeStream;
typedef RefCountPtrCast<ThreadSafeStream, spl::IStream, spl::IStreamPtr> ThreadSafeStreamPtr;
typedef WeakReference<ThreadSafeStream, spl::IStreamPtr> ThreadSafeStreamRef;

REGISTER_TYPEOF(148, ThreadSafeStreamPtr);
REGISTER_TYPEOF( 470, ThreadSafeStreamRef );

/// @brief Serializes access to a stream.
class ThreadSafeStream : public spl::IStream
{
protected:
	spl::IStreamPtr m_strm;
	Mutex m_lock;

public:
	ThreadSafeStream( spl::IStreamPtr strm );
	virtual ~ThreadSafeStream();

	inline void Lock() { m_lock.Lock(); }
	inline void Unlock() { m_lock.Unlock(); }

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

REGISTER_TYPEOF(150, ThreadSafeStream);

/** @} */
/** @} */
}
#endif
