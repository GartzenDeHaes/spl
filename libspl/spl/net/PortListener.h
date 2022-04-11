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
#ifndef _PortListener_h
#define _PortListener_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/net/ServerSocket.h>
#include <spl/threading/Thread.h>

namespace spl
{
/**
 * @defgroup socket Sockets
 * @ingroup network
 * @{
 */

/** @brief Callback interface for socket accept events. */
class IPortListenerListener
{
public:
	virtual ~IPortListenerListener();
	virtual void IPortListener_OnConnect( TcpSocketPtr sock ) = 0;
	virtual void IPortListener_OnStop() = 0;
};

REGISTER_TYPEOF(38, IPortListenerListener);

/** @brief Delegate dispatcher for socket accept events. */
class PortListenerDelegateDispatch : public IMemoryValidate
{
private:
	DelegateDispatch m_onStop;
	DelegateDispatchOneParameter<TcpSocketPtr> m_onConnect;

public:
	inline PortListenerDelegateDispatch()
	: m_onConnect(), m_onStop()
	{
	}

	virtual ~PortListenerDelegateDispatch();

	inline DelegateDispatch& OnStop() { return m_onStop; }
	inline DelegateDispatchOneParameter<TcpSocketPtr>& OnConnect() { return m_onConnect; }

	inline void DispatchOnStop() { m_onStop.Call(); }
	inline void DispatchOnConnect(TcpSocketPtr sock) { m_onConnect.Call(sock); }

	/** Use this if you want to receive events through an interface instead of a delegate. */
	void Add(IPortListenerListener *l);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF(40, PortListenerDelegateDispatch);

class PortListener;
typedef RefCountPtr<PortListener> PortListenerPtr;

/** @brief Creates a thread and listens for connections on a port.
 */
class PortListener : public Thread
{
private:
	// Copy constructor doesn't make sense for this class
	inline PortListener(const PortListener& sock) : Thread() {}
	inline void operator =(const PortListener& sock) {}

protected:
	int m_port;
	bool m_running;
	ServerSocketPtr m_socket;
	PortListenerDelegateDispatch m_listeners;

public:
	PortListener(int port);
	virtual ~PortListener();

	virtual void Run();
	void Stop();

	inline PortListenerDelegateDispatch& Delegates() { return m_listeners; }

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

REGISTER_TYPEOF(42, PortListener);

/** @} */
}
#endif
