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
/* on some platforms, threads overrides IO routines */
#include <spl/threading/Thread.h>
#include <spl/net/SocketSet.h>
#include <spl/io/log/Log.h>

using namespace spl;

ISocketService::~ISocketService()
{
}

SocketListenerPair::SocketListenerPair(IStreamReadListenerPtr listener, TcpSocketPtr sp)
: m_listener(listener), m_sp(sp)
{
}

SocketSet::SocketSet(  )
: m_vread(), m_svreadMutex(), m_buf(SOCKBUF_SIZE)
{
	m_to = new struct timeval;
	m_to->tv_sec = 5;
	m_to->tv_usec = 0;

	FD_ZERO(&m_read);
	//FD_ZERO(&m_write);
	FD_ZERO(&m_excpt);

	// Poll every ten seconds, just in case
	m_sockAddedEvent = new Event( 10 * 1000 );
	
	Start();
}

SocketSet::~SocketSet()
{
	Close();
	if ( NULL != m_to )
	{
		delete m_to;
	}
	delete m_sockAddedEvent;
}

void SocketSet::Join(int timeoutms)
{
	Thread::Join(timeoutms);
}

void SocketSet::Join()
{
	Thread::Join();
}

int SocketSet::SocketCount() const
{ 
	return m_vread.Count();
}

void SocketSet::AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp )
{
	sp.ValidateMem();

	SocketListenerPair *pair = new SocketListenerPair(listener, sp);

	m_svreadMutex.Lock();
	m_vread.Add( pair );

	m_svreadMutex.Unlock();

	m_sockAddedEvent->Notify();
}

bool SocketSet::RemoveSocket( TcpSocket& sp )
{
	sp.ValidateMem();
	ValidateMem();

	sp.Close();

	bool removed = false;
	int count = m_vread.Count();
	for ( int x = 0; x < count; x++ )
	{
		if ( *m_vread.ElementAt(x)->m_sp == sp )
		{
			SocketListenerPair *pair = m_vread.RemoveAt( x );
			if ( NULL == pair )
			{
				throw new Exception("Internal error with Vector::RemoveAt");
			}
			ASSERT_MEM(pair, sizeof(SocketListenerPair));
			pair->m_listener.ValidateMem();
			ASSERT(*pair->m_sp == sp);
			delete pair;
			removed = true;
			break;
		}
	}

	ASSERT(removed);
	if ( !removed )
	{
		return false;
	}
	/*
	m_swrite.Remove( (int)sp->getSocket() );
	count = m_vwrite.Count();
	for ( int x = 0; x < count; x++ )
	{
		if ( m_vwrite.ElementAt(x) == sp )
		{
			m_vwrite.RemoveAt( x );
			break;
		}
	}*/
	//FD_CLR( sp->m_sock.GetFD(), &m_read );
	//FD_CLR( sp->m_sock.GetFD(), &m_write );
	//FD_CLR( sp->m_sock.GetFD(), &m_excpt );

	return true;
}

void SocketSet::WaitForIO(  ) 
{
	//
	// should put copy of sockets in a vector
	//
	int x;

	m_svreadMutex.Lock();

	int readcount = m_vread.Count();

	FD_ZERO( &m_read );
	FD_ZERO( &m_excpt );

	for ( x = 0; x < readcount; x++ )
	{
		TcpSocketPtr sp = m_vread.ElementAt(x)->m_sp;
		sp.ValidateMem();

		FD_SET( sp->m_sock->GetFD(), &m_read );
		FD_SET( sp->m_sock->GetFD(), &m_excpt );
	}
	m_svreadMutex.Unlock();

	if ( readcount == 0 )
	{
		return;
	}

	int count = select( FD_SETSIZE, &m_read, NULL, &m_excpt, m_to );
	if ( count < 0 )
	{
#ifdef _WIN32
		//int error = WSAGetLastError();
#else
		//int error = errno;
#endif
		//Log::WriteError("select error=%d readcount=%d", error, readcount);

		for ( x = 0; x < readcount; x++ )
		{
			TcpSocketPtr sp = m_vread.ElementAt(x)->m_sp;
			if ( sp->GetErrorCode() != 0 || sp->IsClosed() )
			{
				RemoveSocket(sp);
				readcount--;
				x--;
			}
		}
		return;
	}
	else if ( count == 0 )
	{
		return;
	}
	if ( ! m_running )
	{
		return;
	}

	m_svreadMutex.Lock();
		
	for ( x = 0; x < readcount && count > 0 && m_running; x++ )
	{
		if ( x >= m_vread.Count() )
		{
			break;
		}

		SocketListenerPair *sp = m_vread.ElementAt(x);
		ASSERT_MEM( sp, sizeof( SocketListenerPair ) );
		sp->m_sp.ValidateMem();

		try
		{
			if ( FD_ISSET( sp->m_sp->m_sock->GetFD(), &m_read ) )
			{
				count--;
				if ( sp->m_sp->IsClosed() )
				{
					// socket closed.  If zero, socket is shutdown.
					sp->m_listener->IStreamRead_OnClose();
					RemoveSocket( sp->m_sp );
					readcount--;
					continue;
				}
				int bytes = sp->m_sp->m_sock->GetBytesAvail();
				if (0 == bytes)
				{
					// Sockets from .net don't seem to close correctly.
					readcount--;
					sp->m_listener->IStreamRead_OnError( "select() indicates read, but no data available (.net socket?).  Closing socket." );
					sp->m_listener->IStreamRead_OnClose();
					RemoveSocket( sp->m_sp );
					continue;
				}
				while ( bytes > 0 )
				{
					if ( ! m_running )
					{
						m_svreadMutex.Unlock();
						return;
					}
					bytes = sp->m_sp->GetStream()->Read(m_buf, 0, SOCKBUF_SIZE);
					if ( 0 > bytes )
					{
						// socket closed
						sp->m_listener->IStreamRead_OnClose();
						RemoveSocket( sp->m_sp );
						readcount--;
						break;
					}
					ASSERT(sp->m_listener.IsNotNull());
					sp->m_listener->IStreamRead_OnRead( m_buf, bytes );

					bytes = (sp->m_sp->m_sock->IsClosed()) ? -1 : sp->m_sp->m_sock->GetBytesAvail();
				}
			}
			else if ( FD_ISSET( sp->m_sp->m_sock->GetFD(), &m_excpt ) )
			{
				count--;
				readcount--;
				sp->m_listener->IStreamRead_OnError( "Socket exception set" );
				sp->m_listener->IStreamRead_OnClose();
				RemoveSocket( sp->m_sp );
			}
		}
		catch ( SocketException *se )
		{
			ASSERT_MEM( sp, sizeof(SocketListenerPair) );
			sp->m_sp.ValidateMem();
			sp->m_listener->IStreamRead_OnError( se->Message() );
			sp->m_listener->IStreamRead_OnClose();
			RemoveSocket( sp->m_sp );
			delete se;
		}
		catch (Exception *ex)
		{
			Log::SWrite(ex);
			ASSERT_MEM( sp, sizeof(SocketListenerPair) );
			sp->m_sp.ValidateMem();
			sp->m_listener->IStreamRead_OnError( ex->Message() );
			sp->m_listener->IStreamRead_OnClose();
			RemoveSocket( sp->m_sp );
			delete ex;
			Thread::YYield();
		}
	}

	m_svreadMutex.Unlock();
}

void SocketSet::Close()
{
	if ( ! m_running )
	{
		return;
	}
	m_running = false;
	m_sockAddedEvent->Notify();

	m_svreadMutex.Lock();
	m_vread.ValidateMem();

	int count = m_vread.Count();

	for ( int x = 0; x < count; x++ )
	{
		SocketListenerPair *sp = m_vread.ElementAt(x);
		ASSERT_MEM( sp, sizeof( SocketListenerPair ) );
		sp->m_sp->ValidateMem();
		if ( sp->m_sp.IsNotNull() )
		{
			sp->m_sp->Close();
		}
		delete sp;
		m_vread.ValidateMem();
	}
	m_vread.ValidateMem();
	m_vread.Clear();
	ASSERT( 0 == m_vread.Count() );

	//m_swrite.Clear();
	//m_vwrite.Clear();

	m_svreadMutex.Unlock();
}

void SocketSet::CloseAndDelete()
{
	if ( ! m_running )
	{
		return;
	}
	m_running = false;

	m_sockAddedEvent->Notify();
	m_svreadMutex.Lock();

	int count = m_vread.Count();

	for ( int x = 0; x < count; x++ )
	{
		SocketListenerPair *sp = m_vread.ElementAt(x);
		ASSERT_MEM( sp, sizeof( SocketListenerPair ) );
		sp->m_sp->ValidateMem();
		if ( sp->m_sp.IsNotNull() )
		{
			sp->m_sp->Close();
		}
		delete sp;
	}
	m_vread.Clear();

	m_svreadMutex.Unlock();
}

void SocketSet::Run()
{
	m_running = true;

	while ( m_running )
	{
		try
		{
			if ( SocketCount() == 0 )
			{
				m_sockAddedEvent->Wait();
			}
			WaitForIO();
		}
		catch ( OutOfMemoryException mex )
		{
			Log::SWrite(mex);
		}
		catch ( Exception *ex )
		{
			Log::SWrite(ex);
			delete ex;
			Thread::YYield();
		}
	}
}

void SocketSet::Broadcast( const Array<byte>& buf, const int len )
{
	m_svreadMutex.Lock();
	int count = m_vread.Count();
	for ( int x = 0; x < count; x++ )
	{
		SocketListenerPair *sp = m_vread.ElementAt(x);
		if ( NULL != sp )
		{
			sp->m_sp->GetStream()->Write( buf, 0, len );
		}
	}
	m_svreadMutex.Unlock();
}

#if defined(DEBUG) || defined(_DEBUG)
void SocketSet::CheckMem() const
{
	//m_svreadMutex.Lock();

	m_buf.CheckMem();

	if ( NULL != m_to )
	{
		DEBUG_NOTE_MEM_ALLOCATION(m_to);
	}

	m_vread.CheckMem();
	int count = m_vread.Count();
	for ( int x = 0; x < count; x++ )
	{
		SocketListenerPair *sp = m_vread.ElementAt(x);
		if ( NULL != sp )
		{
			DEBUG_NOTE_MEM_ALLOCATION( sp );
			sp->m_sp.CheckMem();
		}
	}
	/*
	m_swrite.CheckMem();
	m_vwrite.CheckMem();
	count = m_vwrite.Count();
	for ( int x = 0; x < count; x++ )
	{
		Socket *sp = m_vwrite.ElementAt(x);
		if ( NULL != sp )
		{
			DEBUG_NOTE_MEM_ALLOCATION( sp );
			sp->CheckMem();
		}
	}*/
	DEBUG_NOTE_MEM_ALLOCATION(m_sockAddedEvent);

	//m_svreadMutex.Unlock();
}

void SocketSet::ValidateMem() const
{
	//m_svreadMutex.Lock();

	m_buf.ValidateMem();

	if ( NULL != m_to )
	{
		ASSERT_MEM(m_to, sizeof(struct timeval));
	}
	m_vread.ValidateMem();
	int count = m_vread.Count();

	for ( int x = 0; x < count; x++ )
	{
		SocketListenerPair *sp = m_vread.ElementAt(x);
		if ( NULL != sp )
		{
			ASSERT_MEM( sp, sizeof(SocketListenerPair) );
			sp->m_sp.ValidateMem();
		}
	}
	/*
	m_swrite.ValidateMem();
	m_vwrite.ValidateMem();
	count = m_vwrite.Count();
	for ( int x = 0; x < count; x++ )
	{
		Socket *sp = m_vwrite.ElementAt(x);
		if ( NULL != sp )
		{
			ASSERT_MEM( sp, sizeof(Socket) );
			sp->ValidateMem();
		}
	}*/
	ASSERT_MEM(m_sockAddedEvent, sizeof(Event));

	//m_svreadMutex.Unlock();
}
#endif

IServerConnectionFactory::~IServerConnectionFactory()
{
}

SocketSetServer::SocketSetServer
(
	IServerConnectionFactory *conFactory, 
	int serverPort
)
: m_ss(), m_listener(serverPort), m_conFactory(conFactory)
{
	m_listener.Delegates().Add(this);
}

SocketSetServer::~SocketSetServer()
{
	m_listener.Stop();
}

void SocketSetServer::AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp ) 
{ 
	m_ss.AddSocket(listener, sp); 
}

void SocketSetServer::Close() 
{ 
	m_listener.Stop(); m_ss.Close(); 
}

void SocketSetServer::CloseAndDelete() 
{ 
	m_listener.Stop(); 
	m_ss.CloseAndDelete(); 
}

int SocketSetServer::SocketCount() const
{ 
	return m_ss.SocketCount(); 
}

void SocketSetServer::Broadcast( const Array<byte>& buf, const int len ) 
{ 
	m_ss.Broadcast(buf, len); 
}

void SocketSetServer::Join(int timeoutms) 
{ 
	m_ss.Join(timeoutms); 
}

void SocketSetServer::Join()
{
	m_ss.Join();
}

void SocketSetServer::IPortListener_OnConnect( TcpSocketPtr sock )
{
	m_ss.AddSocket(m_conFactory->Create(sock), sock);
}

void SocketSetServer::IPortListener_OnStop()
{
	m_ss.CloseAndDelete();
}

#if defined(DEBUG) || defined(_DEBUG)
void SocketSetServer::CheckMem() const
{
	m_ss.CheckMem();
	m_listener.CheckMem();
}

void SocketSetServer::ValidateMem() const
{
	m_ss.ValidateMem();
	m_listener.ValidateMem();
}
#endif

