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
#ifndef _socketstream_h
#define _socketstream_h

#include <spl/io/Stream.h>
#include <spl/net/TcpSocket.h>

namespace spl
{
/**
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class SocketStream;
typedef RefCountPtrCast<SocketStream, spl::IStream, spl::IStreamPtr> SocketStreamPtr;
typedef WeakReference<SocketStream, spl::IStreamPtr> SocketStreamRef;

REGISTER_TYPEOF(120, SocketStreamPtr);
REGISTER_TYPEOF( 463, SocketStreamRef );

/** @brief Stream for a socket, returned from TcpSocket::GetStream().
 *	@ref TcpSocket
 */
class SocketStream : public spl::IStream
{
private:
	// Copy constructor doesn't make sense for this class?
	inline SocketStream(const SocketStream& strm) : IStream() {}

protected:
	SocketPtr m_sock;

	SocketStream(SocketPtr sock);
	friend class Socket;
	friend class TcpSocket;

public:
	virtual ~SocketStream();

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

REGISTER_TYPEOF(122, SocketStream);

/** @} */
}
#endif
