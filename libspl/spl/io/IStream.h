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
#ifndef _mistream_h
#define _mistream_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/DelegateDispatch.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/RefCountPtrCast.h>
#include <spl/String.h>

/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

/** @brief The spl name space is used for some of the stream classes to prevent conflicts with windoze.  Perhaps
 *	it should be used for the entire library, maybe. */
namespace spl
{
	class IStream;
	typedef RefCountPtr<IStream> IStreamPtr;

	/** @brief Stream interface. */
	class IStream : public IMemoryValidate
	{
	private:
		// Don't alow copy constructor for streams
		inline IStream(const IStream& strm) {}
		inline void operator =(const IStream& strm) {}

	public:
		/** Offset position for seeks. */
		typedef enum _SeekOrigin
		{
			SEEK_Begin = 0,		//< Seek from the start of the stream, not supported on all streams.
			SEEK_Current = 1,	//< Seek from the current stream pointer.
			SEEK_End = 2		//< Seek from the end of the stream, not supported on all streams.
		} SeekOrigin;

	protected:
		IStream();

	public:
		virtual ~IStream();

		virtual void Close() = 0;
		virtual void Flush() = 0;
		virtual int Read(Array<byte>& buffer, const int offset, int count) = 0;
		inline int Read(Array<byte>& buffer) { return Read(buffer, 0, buffer.Length()); }
		virtual int ReadByte() = 0;
		virtual long Seek(const long offset, const SeekOrigin origin) = 0;
		virtual void Write(const Array<byte>& buffer, const int offset, const int count) = 0;
		inline void Write(const Array<byte>& buffer) { Write(buffer, 0, buffer.Length()); }
		virtual void WriteByte(byte value) = 0;

		virtual bool CanRead() const = 0;
		virtual bool CanSeek() const = 0;
		virtual bool CanWrite() const = 0;

		virtual long Length() const = 0;
		virtual long Position() const = 0;

	#ifdef DEBUG
		virtual void ValidateMem() const = 0;
		virtual void CheckMem() const = 0;
	#endif
	};

	REGISTER_TYPEOF(102, IStreamPtr);
	REGISTER_TYPEOF(104, IStream);

	class IStreamReadListener;
	typedef RefCountPtr<IStreamReadListener> IStreamReadListenerPtr;

	REGISTER_TYPEOF(106, IStreamReadListenerPtr);

	/** Callback interface for stream events.  Usually, delegates can be used instead of implementing this interface. */
	class IStreamReadListener : public IMemoryValidate
	{
	public:
		virtual ~IStreamReadListener();
		virtual void IStreamRead_OnClose() = 0;
		virtual void IStreamRead_OnRead(const Array<byte>& buf, int len) = 0;
		virtual void IStreamRead_OnError( const String& msg ) = 0;
	};

	REGISTER_TYPEOF(108, IStreamReadListener);

/** @} */
}

#endif
