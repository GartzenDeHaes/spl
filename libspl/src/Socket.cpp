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
#if defined(_WIN32) || defined(_WIN64)
#include <spl/configwin32.h>
#include <stdio.h>
#else
#include <spl/autoconf/config.h>
#endif

// On some platforms, thread overrides IO routines.
#include <spl/threading/Thread.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#include <spl/Environment.h>
#include <ctype.h>
#include <spl/Debug.h>
#include <spl/Exception.h>
#include <spl/io/log/Log.h>
#include <spl/net/ServerSocket.h>
#include <spl/net/Socket.h>
#include <spl/io/SocketStream.h>
#include <spl/net/TcpSocket.h>

using namespace spl;

bool Socket::m_inited = false;

void Socket::sinit()
{
#ifdef _WINDOWS
	m_inited = true;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	 
	wVersionRequested = MAKEWORD( 2, 2 );
	 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
#endif
}

void Socket::Init()
{
	if ( ! m_inited )
	{
		sinit();
	}
	m_saAddr.sin_family = AF_INET;
	m_saAddr.sin_port = htons(0);
	m_saAddr.sin_addr.s_addr = htonl(0);
	m_errorStatus = 0;
	m_closed = false;
}

Socket::Socket(const Socket& sock)
:	m_saAddr(sock.m_saAddr), 
	m_fd(sock.m_fd), 
	m_closed(sock.m_closed), 
	m_errorStatus(sock.m_errorStatus)
{
}

Socket& Socket::operator =(const Socket& sock)
{
	m_saAddr = sock.m_saAddr;
	m_fd = sock.m_fd;
	m_closed = sock.m_closed;
	m_errorStatus = sock.m_errorStatus;

	return *this;
}

Socket::Socket()
: m_fd()
{
	Init();
}

Socket::Socket(SOCKET fd, bool dummy)
{
	Init();
	m_fd = fd;
}

/**
		int domain,	   // AF_UNIX,AF_INET,AF_ISO,AF_NS,AF_IMPLINK
		int type,	   // SOCK_STREAM,SOCK_DGRAM,SOCK_RAW,...
		int protocol);	   // AF_UNSPEC,AF_UNIX,AF_INET,AF_AX25,AF_IPX
*/
Socket::Socket ( int port, int family, int streamType, int protocol )
{
	Init();
	m_fd = ::socket(family, streamType, protocol);
	if (m_fd < 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
	m_saAddr.sin_port = htons(port);
}

Socket::Socket (int port)
{
	Init();
	m_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_fd < 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
	m_saAddr.sin_port = htons(port);
}

TcpSocketPtr ServerSocket::Accept()
{
	if ( ! m_sock.m_inited )
	{
		m_sock.sinit();
	}
	SOCKET fd;
	SOCKADDR_IN saAddr;
#ifdef _WINDOWS
	int size = sizeof(saAddr);
#else
	socklen_t size = sizeof(saAddr);
#endif
	m_sock.m_errorStatus = 0;
	fd = ::accept(m_sock.m_fd, (struct sockaddr *)&saAddr, &size);
	if ( fd == INVALID_SOCKET )
	{
		if (m_sock.IsClosed())
		{
			return TcpSocketPtr();
		}
#ifdef _WINDOWS
		m_sock.m_errorStatus = WSAGetLastError();
		if ( 10004 == m_sock.m_errorStatus )
		{
			return Accept();
		}
#else
		m_sock.m_errorStatus = errno;
#endif
		throw new SocketException(Environment::LastErrorMessage());
	}
	TcpSocketPtr sock = TcpSocketPtr( new TcpSocket(fd) );
	memcpy(&sock->m_sock->m_saAddr, &saAddr, sizeof(saAddr));

	//lookupName(&saAddr, 5, hostName);
	
	return sock;
}

Socket::~Socket()
{
	if (m_fd > 0)
	{
		Close ();
	}
}

void Socket::SetNonBlocking()
{
#ifdef _WINDOWS
	u_long arg = 1;
	if (ioctlsocket(m_fd, FIONBIO, &arg) != 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
#else
	int flags;

	if ((flags = fcntl(m_fd, F_GETFL, 0)) < 0 ||
			fcntl(m_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
#endif
}

void Socket::SetBlocking()
{
#ifdef _WINDOWS
	u_long arg = 0;
	if (ioctlsocket(m_fd, FIONBIO, &arg) != 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
#else
	int flags;

	if ((flags = fcntl(m_fd, F_GETFL, 0)) < 0 ||
			fcntl(m_fd, F_SETFL, flags & ~O_NONBLOCK) < 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
#endif
}

void ServerSocket::Bind ()
{
	m_sock.m_saAddr.sin_addr.s_addr = htonl (INADDR_ANY);

	if (::bind (m_sock.m_fd, (struct sockaddr *)&m_sock.m_saAddr, sizeof (m_sock.m_saAddr)) != 0)
	{
		m_sock.m_errorStatus = Environment::LastError();
		throw new SocketException(Environment::LastErrorMessage());
	}
}

void Socket::SetNoDelay()
{
#ifdef _WINDOWS
	BOOL opt = TRUE;
	if (setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&opt, sizeof(BOOL)) != 0)
#elif defined(TCP_NODELAY)
	int opt = 1;
	if (setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(int)) != 0)
#else
	if(true)
#endif
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
}

void Socket::SetSendTimeout(int toMS)
{
	int ret = setsockopt(m_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&toMS, sizeof(int));
#ifdef _WINDOWS
	if ( ret != NO_ERROR )
#else
	if ( ret < 0 )
#endif
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
}

void Socket::SetRecvTimeout(int toMS)
{
	int ret = setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&toMS, sizeof(int));
#ifdef _WIN32
	if ( ret != NO_ERROR )
#else
	if ( ret < 0 )
#endif
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
}

int Socket::GetSendTimeout()
{
	int timeout;
#ifdef _WIN32
	int len = sizeof(int);
#else
	socklen_t len = sizeof(socklen_t);
#endif
	int ret = getsockopt(m_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, &len);
#ifdef _WIN32
	if ( ret != NO_ERROR )
#else
	if ( ret < 0 )
#endif
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
	return timeout;
}

int Socket::GetRecvTimeout()
{
	int timeout;
#ifdef _WIN32
	int len = sizeof(int);
#else
	socklen_t len = sizeof(socklen_t);
#endif
	int ret = getsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, &len);
#ifdef _WIN32
	if ( ret != NO_ERROR )
#else
	if ( ret < 0 )
#endif
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
	return timeout;
}

void Socket::Flush()
{
#ifdef _WIN32
	int ret = WSAIoctl(m_fd, SIO_FLUSH, NULL, 0, NULL, 0, NULL, NULL, NULL);
	if ( ret != 0 )
	{
#ifdef _WIN32
		m_errorStatus = WSAGetLastError();
#else
		m_errorStatus = errno;
#endif
		throw new SocketException(Environment::LastErrorMessage());
	}
#endif /* _WIN32 */
}

void Socket::SetAddrReuse (int i)
{
	m_errorStatus = setsockopt (m_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(int));
	if(m_errorStatus != 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
}

void Socket::SetLingerOn ()
{
	struct linger l;

	l.l_onoff = 1;
	l.l_linger = 5;
	if (setsockopt (m_fd, SOL_SOCKET, SO_LINGER, (char *)&l, sizeof(struct linger)) != 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
}

void Socket::SetLingerOff ()
{
	struct linger l;

	l.l_onoff = 0;
	l.l_linger = 0;
	if (setsockopt (m_fd, SOL_SOCKET, SO_LINGER, (char *)&l, sizeof(struct linger)) != 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
}

void Socket::Send (const Array<byte>& data, const int offset, const int len)
{
	ASSERT(offset + len <= data.Length());

	if (::send (m_fd, &(((const char *)data.Data())[offset]), len, 0) < 0)
	{
#ifdef _WIN32
		int err = WSAGetLastError();
#else
		int err = errno;
#endif
		m_errorStatus = err;
		throw new SocketException(Environment::LastErrorMessage());
	}
}

int Socket::Recv (Array<byte>& buf, const int offset, const int blen)
{
	ASSERT(offset + blen <= buf.Length());

	int count = ::recv (m_fd, &(((char *)buf.Data())[offset]), blen, 0);
#ifdef _WIN32
	if ( SOCKET_ERROR == count )
#else
	if ( 0 > count )
#endif
	{
#ifdef _WIN32
		int err = WSAGetLastError();
#else
		int err = errno;
#endif
		m_errorStatus = err;
		throw new SocketException(Environment::LastErrorMessage());
	}
	return count;
}

int Socket::RecvByte()
{
	int b = 0;
	int count = ::recv (m_fd, (char *)&b, 1, 0);
#ifdef _WIN32
	if ( SOCKET_ERROR == count )
#else
	if ( 0 > count )
#endif
	{
#ifdef _WIN32
		int err = WSAGetLastError();
#else
		int err = errno;
#endif
		m_errorStatus = err;
		throw new SocketException(Environment::LastErrorMessage());
	}
	return b;
}

void ServerSocket::Listen (int iNumBuf)
{
	if (::listen(m_sock.m_fd, iNumBuf) != 0)
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
}

int Socket::GetBytesAvail(  )
{
#if defined(_WIN32) || defined(_WIN64)
	u_long arg = 0;
	if (ioctlsocket(m_fd, FIONREAD, &arg) == 0)
	{
		return (int)arg;
	}
	else
	{
		m_errorStatus = WSAGetLastError();
		throw new SocketException(Environment::LastErrorMessage());
	}
#else
	int arg = 0;
	if ( ioctl(m_fd, FIONREAD, &arg) != 0 )
	{
		m_errorStatus = errno;
		throw new SocketException(Environment::LastErrorMessage());
	}
	return arg;
#endif
}

String Socket::GetRemoteIp(  )
{
	SOCKADDR_IN name;
	char buf[17];

#ifdef _WIN32
	int namelen = sizeof(name);
#else
	socklen_t namelen = sizeof(name);
#endif
	if (0 != getpeername( m_fd, (struct sockaddr *)&name, &namelen ))
	{
		throw new SocketException(Environment::LastErrorMessage());
	}
#ifdef _WIN32
	sprintf(buf, "%hd.%hd.%hd.%hd", (short)name.sin_addr.S_un.S_un_b.s_b1, (short)name.sin_addr.S_un.S_un_b.s_b2, (short)name.sin_addr.S_un.S_un_b.s_b3, (short)name.sin_addr.S_un.S_un_b.s_b4);
#else
	strcpy(buf, inet_ntoa(name.sin_addr));
#endif
	return String(buf);
}

bool Socket::IsClosed() 
{ 
	// This doesn't always work

	if ( m_closed )
	{
		return true;
	}
	char buf;
	int count = recv(m_fd, &buf, 1, MSG_PEEK);
	if ( 0 > count )
	{
		m_closed = true;
		return true;
	}
	
	return false;
}

void Socket::Shutdown( int i ) 
{
	m_closed = true; 
	if ( 0 != ::shutdown(m_fd, i) )
	{
#if defined(_WIN32) || defined(_WIN64)
		int error = WSAGetLastError();
#else
		int error = errno;
#endif
		m_errorStatus = error;
		throw new SocketException(Environment::LastErrorMessage());
	}
}

void Socket::ShutdownService()
{
#if defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}

#if defined(DEBUG) || defined(_DEBUG)
void Socket::CheckMem() const
{
}

void Socket::ValidateMem() const
{
}
#endif


ServerSocket::ServerSocket( int port, int numbuf )
: m_sock(port)
{
	Bind();
	Listen( numbuf );
}

ServerSocket::~ServerSocket()
{
}

SocketStream::SocketStream(SocketPtr sock)
: m_sock(sock)
{
}

SocketStream::~SocketStream()
{
	Close();
}

void SocketStream::Close()
{
	m_sock->Close();
}

void SocketStream::Flush()
{
	m_sock->Flush();
}

int SocketStream::Read(Array<byte>& buffer, const int offset, int count)
{
	return m_sock->Recv(buffer, offset, count);
}

int SocketStream::ReadByte()
{
	return m_sock->RecvByte();
}

long SocketStream::Seek(const long offset, const SeekOrigin origin)
{
	throw new NotImplementedException();
}

void SocketStream::Write(const Array<byte>& buffer, const int offset, const int count)
{
	m_sock->Send(buffer, offset, count);
}

void SocketStream::WriteByte(byte value)
{
	Array<byte> buf(1);
	buf[0] = value;
	m_sock->Send(buf);
}

bool SocketStream::CanRead() const
{
	return true;
}

bool SocketStream::CanSeek() const
{
	return false;
}

bool SocketStream::CanWrite() const
{
	return true;
}

long SocketStream::Length() const
{
	return m_sock->GetBytesAvail();
}

long SocketStream::Position() const
{
	return 0;
}

#ifdef DEBUG
void SocketStream::ValidateMem() const
{
	m_sock.ValidateMem();
}

void SocketStream::CheckMem() const
{
	m_sock.CheckMem();
}
#endif

TcpSocket::TcpSocket( SOCKET fd )
: m_sock(new Socket(fd, false)), m_address("")
{
	m_strm = SocketStreamPtr(new SocketStream(m_sock));
}

TcpSocket::TcpSocket( const String& address, int port )
: m_sock(new Socket(port)), m_address(address)
{
	m_strm = SocketStreamPtr(new SocketStream(m_sock));
}

TcpSocket::~TcpSocket()
{
	m_sock->Close();
}

spl::IStreamPtr TcpSocket::GetStream()
{
	return m_strm;
}

/** @brief TcpSocket::Connect
 * addr must be in net byte order
 */
void TcpSocket::Connect ()
{
	//addr.sin_family = AF_INET;
	//addr.sin_port = htons (port);      	

	if (isdigit(m_address.CharAt(0)))
	{
		m_sock->m_saAddr.sin_addr.s_addr = inet_addr(m_address.GetChars());
	}
	else
	{
		hostent *phostent = gethostbyname(m_address.GetChars());
		memcpy ((char *)&(m_sock->m_saAddr.sin_addr), phostent->h_addr, phostent->h_length);
	}
	if (::connect (m_sock->m_fd, (struct sockaddr *)&m_sock->m_saAddr, sizeof (SOCKADDR_IN)) != 0)
	{
		m_sock->m_errorStatus = Environment::LastError();
		throw new SocketException(Environment::LastErrorMessage());
	}
}

#if defined(DEBUG) || defined(_DEBUG)
void TcpSocket::CheckMem() const 
{
	m_strm.CheckMem(); 
	m_address.CheckMem(); 
	m_sock.CheckMem();
}

void TcpSocket::ValidateMem() const 
{
	m_strm.ValidateMem(); 
	m_address.ValidateMem(); 
	m_sock.ValidateMem();
}
#endif
