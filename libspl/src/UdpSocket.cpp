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
#include <spl/Char.h>
#include <spl/Environment.h>
#include <spl/net/UdpSocket.h>

using namespace spl;

UdpSocket::UdpSocket(const UdpSocket& sock)
: m_sock(sock.m_sock), m_port(sock.m_port)
{
}

UdpSocket& UdpSocket::operator =(const UdpSocket& sock)
{
	m_sock = sock.m_sock; 
	m_port = sock.m_port;

	return *this;
}

UdpSocket::UdpSocket()
: m_sock(new Socket(0, AF_INET, SOCK_DGRAM, 0)), m_port(0)
{
	memset(&m_recvFromAddr, 0, sizeof(m_recvFromAddr));
}

UdpSocket::UdpSocket( int port )
: m_sock(new Socket(port, AF_INET, SOCK_DGRAM, 0)), m_port(port)
{
	memset(&m_recvFromAddr, 0, sizeof(m_recvFromAddr));
	
	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = 0; // choose any
	local.sin_port = htons(port);
	
	if (0 != ::bind(m_sock->GetFD(), (sockaddr *)&local, sizeof(local)))
	{
		m_sock->m_errorStatus = Environment::LastError();
		throw new SocketException(Environment::LastErrorMessage());
	}
}

UdpSocket::~UdpSocket()
{
	m_sock->Close();
}

void UdpSocket::Send(const String& address, int port, const Array<byte>& packet)
{
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );
	
	if (isdigit(address.CharAt(0)))
	{
		addr.sin_addr.s_addr = inet_addr(address.GetChars());
	}
	else
	{
		hostent *phostent = gethostbyname(address.GetChars());
		if (NULL == phostent)
		{
			throw new SocketException("Unknown host name " + address);
		}
		memcpy (&addr.sin_addr, phostent->h_addr, phostent->h_length);
	}
	
	int ret;
	if (packet.Length() != (ret = sendto(m_sock->GetFD(), (const char *)(byte *)packet, packet.Length(), 0, (sockaddr *)&addr, sizeof(addr))))
	{
		m_sock->m_errorStatus = Environment::LastError();
		throw new SocketException(Environment::LastErrorMessage());	
	}
}

int UdpSocket::Recv(Array<byte>& packet)
{
#ifdef _WINDOWS
	int addrSize = sizeof(m_recvFromAddr);
	return recvfrom(m_sock->GetFD(), (char *)(byte *)packet, packet.Length(), 0, (sockaddr *)&m_recvFromAddr, &addrSize);
#else
	socklen_t addrSize = sizeof(m_recvFromAddr);
	return recvfrom(m_sock->GetFD(), (char *)(byte *)packet, packet.Length(), 0, (sockaddr *)&m_recvFromAddr, &addrSize);
#endif
}

#if defined(DEBUG) || defined(_DEBUG)
void UdpSocket::CheckMem() const 
{
	m_sock.CheckMem();
}

void UdpSocket::ValidateMem() const 
{
	m_sock.ValidateMem();
}
#endif
