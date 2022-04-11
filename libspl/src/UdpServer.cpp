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
#include <spl/net/UdpServer.h>

using namespace spl;

UdpServer::UdpServer
(
	int port, 
	RefCountPtr<IDelegateThreeParameter<UdpSocket&, Array<byte>&, int> > callback,
	int bufferSize
)
:	m_sock(port), 
	m_callback(callback), 
	m_thread(this, &UdpServer::Run), 
	m_running(false),
	m_bufSize(bufferSize)
{
	m_sock.SetBlocking();
	m_thread.Start();
}

UdpServer::~UdpServer()
{
}

void UdpServer::Run()
{
	ASSERT(m_running == false);
	
	Array<byte> m_buf(m_bufSize);	
	m_running = true;
	
	while (m_running)
	{
		int len = m_sock.Recv(m_buf);
		if (0 > len)
		{
			m_running = false;
			return;
		}
		m_callback->Call(m_sock, m_buf, len);
	}
}

void UdpServer::Join()
{
	m_thread.Join();
}

void UdpServer::Close()
{
	m_running = false;
	m_sock.Close();
}

#if defined(DEBUG) || defined(_DEBUG)
void UdpServer::CheckMem() const
{
	m_sock.CheckMem();
}

void UdpServer::ValidateMem() const
{
	m_sock.ValidateMem();
}
#endif
