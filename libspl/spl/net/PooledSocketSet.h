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
#ifndef _poolsocketset_h
#define _poolsocketset_h

#define FD_SETSIZE 64

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/threading/Mutex.h>
#include <spl/net/PortListener.h>
#include <spl/net/SocketSet.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup socket Sockets
 * @ingroup network
 * @{
 */

class PooledSocketSet;
typedef RefCountPtrCast<PooledSocketSet, ISocketService, ISocketServicePtr> PooledSocketSetPtr;

///@brief Creates 64 SocketSets, each with its own thread -- performance should be close to windows IO completion ports.
///
class PooledSocketSet : public ISocketService
{
private:
	// Copy constructor doesn't make sense for this class
	inline PooledSocketSet(const PooledSocketSet& pss) {}
	inline void operator =(const PooledSocketSet& pss) {}

protected:
	Vector<SocketSetPtr> m_sets;
	int m_socketCount;
	Mutex m_setsMtx;

	void RemoveSocket( TcpSocket& sp );

public:
	PooledSocketSet(int poolSize = 1024/FD_SETSIZE);
	virtual ~PooledSocketSet();

	virtual void AddSocket( IStreamReadListenerPtr listener, TcpSocketPtr sp );
	virtual void Close();
	virtual void CloseAndDelete();
	virtual int SocketCount() const;
	virtual void Broadcast( const Array<byte>& buf, const int len );

	virtual void Join(int timeoutms);
	virtual void Join();

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

class PooledSocketServer;
typedef RefCountPtrCast<PooledSocketServer, ISocketService, ISocketServicePtr> PooledSocketServerPtr;

///@brief Listens on a port and adds incomming connections to a PooledSocketSet
///
class PooledSocketServer : public ISocketService, public IPortListenerListener
{
private:
	// Copy constructor doesn't make sense for this class
	inline PooledSocketServer(const PooledSocketServer& pss) 
	: m_listener(0)
	{
	}

	inline void operator =(const PooledSocketServer& pss) {}

protected:
	PooledSocketSet m_ss;
	PortListener m_listener;
	IServerConnectionFactory *m_conFactory;

	virtual void IPortListener_OnConnect( TcpSocketPtr sock );
	virtual void IPortListener_OnStop();

public:
	PooledSocketServer(IServerConnectionFactory *conFactory, int serverPort, int poolSize = 1024/FD_SETSIZE);
	virtual ~PooledSocketServer();

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

/** @} */
}
#endif
