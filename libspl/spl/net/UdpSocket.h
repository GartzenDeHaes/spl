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
#ifndef _udpsocket_h
#define _udpsocket_h

#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/RefCountPtr.h>
#include <spl/net/Socket.h>

namespace spl
{
/** 
 * @defgroup socket Sockets
 * @ingroup network
 * @{
 */

class UdpSocket;
typedef RefCountPtr<UdpSocket> UdpSocketPtr;
typedef WeakReference<UdpSocket, UdpSocketPtr> UdpSocketRef;

/** @brief Packet oriented socket.
 *
 */
class UdpSocket : public IMemoryValidate
{
private:

protected:
	SocketPtr m_sock;
	int m_port;
	SOCKADDR_IN m_recvFromAddr;

public:
	UdpSocket(const UdpSocket& sock);
	UdpSocket(  );
	UdpSocket( int listenPort );
	virtual ~UdpSocket();

	UdpSocket& operator =(const UdpSocket& pmp);

	inline int Port() const { return m_port; }

	//void Send (const Array<byte>& packet);
	void Send (const String& address, int port, const Array<byte>& packet);
	int Recv(Array<byte>& packet);
	
	SOCKADDR_IN ReceivedFromAddress() const { return m_recvFromAddr; }

	inline bool IsClosed() { return m_sock->IsClosed(); }
	inline void Close() { m_sock->Close(); }
	inline void Shutdown( int i=SD_BOTH ) { m_sock->Shutdown(i); }
	
	inline void SetNonBlocking() { m_sock->SetNonBlocking(); }
	inline void SetBlocking() { m_sock->SetBlocking(); }
	inline void SetLingerOn() { m_sock->SetLingerOn(); }
	inline void SetLingerOff() { m_sock->SetLingerOff(); }
	inline void SetNoDelay() { m_sock->SetNoDelay(); }

	inline void SetSendTimeout(int toMS) { m_sock->SetSendTimeout(toMS); }
	inline void SetRecvTimeout(int toMS) { m_sock->SetRecvTimeout(toMS); }
	inline int GetSendTimeout() { return m_sock->GetSendTimeout(); }
	inline int GetRecvTimeout() { return m_sock->GetRecvTimeout(); }

	inline int GetErrorCode() { return m_sock->m_errorStatus; }
	inline int GetBytesAvail() { return m_sock->GetBytesAvail(); }

	inline String GetRemoteIp(  ) { return m_sock->GetRemoteIp(); }

	inline bool operator ==(const UdpSocket& s) const
	{
		return m_sock->GetFD() == s.m_sock->GetFD();
	}

	friend class ServerSocket;
	friend class SocketSet;

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

REGISTER_TYPEOF(71, UdpSocket);

/** @} */
}
#endif
