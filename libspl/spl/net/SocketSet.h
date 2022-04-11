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
#ifndef _socketset_h
#define _socketset_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/net/Socket.h>
#include <spl/io/IStream.h>
#include <spl/net/Socket.h>
#include <spl/threading/Event.h>
#include <spl/threading/Mutex.h>
#include <spl/net/PortListener.h>
#include <spl/threading/Thread.h>
#include <spl/collection/Hashtable.h>

namespace spl
{
/**
 * @defgroup socket Sockets
 * @ingroup network
 * @{
 */

class ISocketService;
typedef RefCountPtr<ISocketService> ISocketServicePtr;

/** @brief Base class for SocketSet and PooledSocketSet. */
class ISocketService
{
public:
	inline ISocketService() {}
	virtual ~ISocketService();

	virtual void AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp ) = 0;
	virtual void Close() = 0;
	virtual void CloseAndDelete() = 0;
	virtual int SocketCount() const = 0;
	inline void Broadcast( const Array<byte>& buf ) { Broadcast(buf, buf.Length()); }
	virtual void Broadcast( const Array<byte>& buf, const int len ) = 0;
	virtual void Join(int timeoutms) = 0;
	virtual void Join() = 0;

#if defined(DEBUG)
	virtual void CheckMem() const = 0;
	virtual void ValidateMem() const = 0;
#endif
};

///@brief used by SocketSet to track socket listners (internal).
class SocketListenerPair
{
public:
	IStreamReadListenerPtr m_listener;
	TcpSocketPtr m_sp;

	SocketListenerPair(IStreamReadListenerPtr listener, TcpSocketPtr sp);

	inline ~SocketListenerPair()
	{
	}
};

#ifdef DEBUG
inline void TypeValidate( SocketListenerPair *slp )
{
	if ( NULL != slp )
	{
		ASSERT_MEM( slp, sizeof(SocketListenerPair) );
		slp->m_listener.ValidateMem();
		slp->m_sp.ValidateMem();
	}
}

inline void TypeCheckMem( SocketListenerPair *slp )
{
	if ( NULL != slp )
	{
		DEBUG_NOTE_MEM( slp );
		slp->m_listener.CheckMem();
		slp->m_sp.CheckMem();
	}
}
#endif

class SocketSet;
typedef RefCountPtrCast<SocketSet, ISocketService, ISocketServicePtr> SocketSetPtr;

REGISTER_TYPEOF(56, SocketSetPtr);

///@brief A threaded select() IO dispatcher.
class SocketSet : public ISocketService, public Thread
{
private:
	// Copy constructor doesn't make sense for this class
	inline SocketSet(const SocketSet& ss) : Thread() {}
	inline void operator =(const SocketSet& s) {}

protected:
	Array<byte> m_buf;
	Vector<SocketListenerPair *>m_vread;
	struct timeval *m_to;
	fd_set m_read;
	fd_set m_excpt;

	Event *m_sockAddedEvent;
	Mutex m_svreadMutex;
	bool m_running;

	void WaitForIO();
	void Run();
	bool RemoveSocket( TcpSocket& sp );

public:
	SocketSet(  );
	virtual ~SocketSet();

	virtual void AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp );
	virtual void Close();
	virtual void CloseAndDelete();
	virtual int SocketCount() const;
	void Broadcast( const Array<byte>& buf, const int len );

	virtual void Join(int timeoutms);
	virtual void Join();

	inline void Lock() { m_svreadMutex.Lock(); }
	inline void UnLock() { m_svreadMutex.Unlock(); }

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

REGISTER_TYPEOF(58, SocketSet);

class IServerConnectionFactory;
typedef RefCountPtr<IServerConnectionFactory> IServerConnectionFactoryPtr;

REGISTER_TYPEOF(60, IServerConnectionFactoryPtr);

///@breif When SocketSetServer accepts a connection, it uses
/// IServerConnectionFactory to allow the application to create an
/// application specific connection wrapper.
class IServerConnectionFactory : public IMemoryValidate
{
public:
	virtual ~IServerConnectionFactory();
	virtual IStreamReadListenerPtr Create(TcpSocketPtr sock) = 0;
};

REGISTER_TYPEOF(62, IServerConnectionFactory);

class SocketSetServer;
typedef RefCountPtr<SocketSetServer> SocketSetServerPtr;

REGISTER_TYPEOF(64, SocketSetServerPtr);

///@breif Accepts connections at a port and automatically adds them to the SocketSet.
class SocketSetServer : public ISocketService, public IPortListenerListener
{
protected:
	SocketSet m_ss;
	PortListener m_listener;
	IServerConnectionFactory *m_conFactory;

	virtual void IPortListener_OnConnect( TcpSocketPtr sock );
	virtual void IPortListener_OnStop();

public:
	SocketSetServer(IServerConnectionFactory *conFactory, int serverPort);
	virtual ~SocketSetServer();

	virtual void AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp );
	virtual void Close();
	virtual void CloseAndDelete();
	virtual int SocketCount() const;
	virtual void Broadcast( const Array<byte>& buf, const int len );

	virtual void Join(int timeoutms);
	virtual void Join();

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

REGISTER_TYPEOF(66, SocketSetServer);

/** @} */
}
#endif
