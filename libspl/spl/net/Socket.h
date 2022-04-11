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
#ifndef _sock_h
#define _sock_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/io/IStream.h>
#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/WeakReference.h>

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#include <sys/types.h>

#if defined(_WINDOWS)
//#define FD_SETSIZE 512
#define _WINSOCKAPI_
//#include <spl/cleanwindows.h>
#include <winsock2.h>
#include <spl/configwin32.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if !defined(SOCKET) && !defined(_WINDOWS)
typedef int SOCKET;
#endif
#if !defined(_WINDOWS)
typedef struct sockaddr_in SOCKADDR_IN;
#endif
#if !defined(_WINDOWS)
#define closesocket(A) close(A)
#define INVALID_SOCKET -1
#endif
#if !defined(SD_BOTH)
#define SD_BOTH SHUT_RDWR
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef _TANDEM
typedef size_t socklen_t;
#endif

/** 
 * @defgroup network Network
 * @{
 */

namespace spl
{
/** 
 * @defgroup socket Sockets
 * @ingroup network
 * @{
 */

#define SOCKBUF_SIZE 1024

class Socket;
typedef RefCountPtr<Socket> SocketPtr;
typedef WeakReference<Socket, SocketPtr> SocketRef;

/** @brief Base socket type, you probably want to use TcpSocket or ServerSocket instead.
 *	@ref TcpSocket
 *	@ref ServerSocket
 */
class Socket : public IMemoryValidate
{
private:

protected:
	static bool m_inited;

	int m_errorStatus;	   /* from errno */
	SOCKET m_fd;
	SOCKADDR_IN m_saAddr;
	bool m_closed;

	void SetAddrReuse(int i);

	static void sinit();

	Socket(SOCKET fd, bool dummy);
	void Init();

	Socket ( int port, int family, int streamType, int protocol );
	
public:
	Socket();
	Socket(const Socket& sock);
	Socket ( int port );
	~Socket();

	Socket& operator =(const Socket& sock);

	inline void SetAddrReuseOn() { SetAddrReuse(1); }
	
	void SetNonBlocking();
	void SetBlocking();
	
	void Send (const Array<byte>& data, const int offset, const int len);  ///< write to sock
	inline void Send (const Array<byte>& data, const int len) { Send(data, 0, len); }
	inline void Send (const Array<byte>& data) { Send(data, data.Length()); }
	inline void Send (const String& str) { Send(str.ToByteArray()); }

	// returns bytes read
	int Recv (Array<byte>& buf, const int offset, const int blen);   ///< pull data from sock.
	inline int Recv (Array<byte>& buf, const int blen) { return Recv(buf, 0, blen); }
	int RecvByte();

	void Flush();
	int GetBytesAvail();

	inline SOCKET GetFD() const { return m_fd; }
	inline const SOCKADDR_IN *GetAddr() const { return &m_saAddr; }

	virtual bool IsClosed();

	inline void Close() 
	{ 
		if (!m_closed) 
		{ 
			m_closed = true; 
			::closesocket(m_fd); 
		} 
	}

	void Shutdown( int i=SD_BOTH );
	
	void SetLingerOn();
	void SetLingerOff();
	void SetNoDelay();

	void SetSendTimeout(int toMS);
	void SetRecvTimeout(int toMS);
	int GetSendTimeout();
	int GetRecvTimeout();

	inline int GetErrorCode() const { return m_errorStatus; }

	String GetRemoteIp(  );

	static void ShutdownService();

	friend class ServerSocket;
	friend class TcpSocket;
	friend class UdpSocket;

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

/** @} */
/** @} */
}
#endif
