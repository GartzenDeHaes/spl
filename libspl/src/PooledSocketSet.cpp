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
#include <spl/net/PooledSocketSet.h>

using namespace spl;

PooledSocketSet::PooledSocketSet(int poolSize)
: m_sets(), m_socketCount(0), m_setsMtx()
{
	for ( int x = 0; x < poolSize; x++ )
	{
		m_sets.Add( SocketSetPtr(new SocketSet()) );
	}
}

PooledSocketSet::~PooledSocketSet()
{
	int count = m_sets.Count();
	for ( int x = 0; x < count; x++ )
	{
		SocketSetPtr ss = m_sets.ElementAt(x);
		ss.ValidateMem();
		ss->CloseAndDelete();
	}
	m_sets.Clear();
}

int PooledSocketSet::SocketCount() const
{ 
	return m_socketCount; 
}

void PooledSocketSet::AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp )
{
	m_setsMtx.Lock();

	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		if ( m_sets.ElementAt(x)->SocketCount() < FD_SETSIZE )
		{
			m_sets.ElementAt(x)->AddSocket( listener, sp );
			m_setsMtx.Unlock();
			return;
		}
	}
	listener->IStreamRead_OnClose();
	sp->Close();

	m_setsMtx.Unlock();
}
/*
void PooledSocketSet::RemoveSocket( TcpSocket *sp )
{
	m_setsMtx.Lock();
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		if ( m_sets.ElementAt(x)->RemoveSocket(sp) )
		{
			m_setsMtx.Unlock();
			return;
		}
	}
	m_setsMtx.Unlock();
}
*/
void PooledSocketSet::Close()
{
	m_setsMtx.Lock();
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		m_sets.ElementAt(x)->Close();
	}
	m_setsMtx.Unlock();
}

void PooledSocketSet::CloseAndDelete()
{
	m_setsMtx.Lock();
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		m_sets.ElementAt(x)->CloseAndDelete();
	}
	m_setsMtx.Unlock();	
}

void PooledSocketSet::Broadcast( const Array<byte>& buf, const int len )
{
	m_setsMtx.Lock();
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		m_sets.ElementAt(x)->Broadcast(buf, len);
	}
	m_setsMtx.Unlock();
}

void PooledSocketSet::Join(int timeoutms)
{
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		m_sets.ElementAt(x)->Join(timeoutms);
	}
}

void PooledSocketSet::Join()
{
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		m_sets.ElementAt(x)->Join();
	}
}

#if defined(DEBUG) || defined(_DEBUG)
void PooledSocketSet::CheckMem() const
{
	//m_setsMtx.Lock();
	m_sets.CheckMem();
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		m_sets.ElementAt(x).CheckMem();
	}
	//m_setsMtx.Unlock();
}

void PooledSocketSet::ValidateMem() const
{
	//m_setsMtx.Lock();
	m_sets.ValidateMem();
	for ( int x = 0; x < m_sets.Count(); x++ )
	{
		m_sets.ElementAt(x).ValidateMem();
	}
	//m_setsMtx.Unlock();
}
#endif

PooledSocketServer::PooledSocketServer
(
	IServerConnectionFactory *conFactory, 
	int serverPort, 
	int poolSize
)
: m_ss(poolSize), m_listener(serverPort), m_conFactory(conFactory)
{
	m_listener.Delegates().Add(this);
}

PooledSocketServer::~PooledSocketServer()
{
	m_listener.Stop();
}

void PooledSocketServer::IPortListener_OnConnect( TcpSocketPtr sock )
{
	m_ss.AddSocket(m_conFactory->Create(sock), sock);
}

void PooledSocketServer::IPortListener_OnStop()
{
	// Not needed, since OnStop is called by m_listener.Stop() below.
	//m_ss.CloseAndDelete();
}

void PooledSocketServer::AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp ) 
{ 
	m_ss.AddSocket(listener, sp); 
}

void PooledSocketServer::Close() 
{ 
	m_listener.Stop(); 
	m_ss.Close(); 
}

void PooledSocketServer::CloseAndDelete() 
{ 
	m_listener.Stop(); 
	m_ss.CloseAndDelete(); 
}

int PooledSocketServer::SocketCount() const
{ 
	return m_ss.SocketCount(); 
}

void PooledSocketServer::Broadcast( const Array<byte>& buf, const int len ) 
{ 
	m_ss.Broadcast(buf, len); 
}

void PooledSocketServer::Join(int timeoutms) 
{ 
	m_ss.Join(timeoutms); 
}

void PooledSocketServer::Join() 
{ 
	m_ss.Join(); 
}

#if defined(DEBUG) || defined(_DEBUG)
void PooledSocketServer::CheckMem() const
{
	m_ss.CheckMem();
	m_listener.CheckMem();
}

void PooledSocketServer::ValidateMem() const
{
	m_ss.ValidateMem();
	m_listener.ValidateMem();
}
#endif
