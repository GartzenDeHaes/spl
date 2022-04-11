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

#include <spl/io/log/Log.h>
#include <spl/net/PortListener.h>

using namespace spl;

IPortListenerListener::~IPortListenerListener()
{
}

PortListenerDelegateDispatch::~PortListenerDelegateDispatch()
{
}

void PortListenerDelegateDispatch::Add(IPortListenerListener *l)
{
	m_onStop += Delegate<IPortListenerListener>::Create(l, &IPortListenerListener::IPortListener_OnStop);
	m_onConnect += DelegateOneParameter<IPortListenerListener, TcpSocketPtr>::Create(l, &IPortListenerListener::IPortListener_OnConnect);
}

#ifdef DEBUG
void PortListenerDelegateDispatch::ValidateMem() const
{
	m_onStop.ValidateMem();
	m_onConnect.ValidateMem();
}

void PortListenerDelegateDispatch::CheckMem() const
{
	m_onStop.CheckMem();
	m_onConnect.CheckMem();
}
#endif

PortListener::PortListener(int port)
: m_port(port), m_listeners(), m_socket(ServerSocketPtr(new ServerSocket(port, 100)))
{
	Start();
}

PortListener::~PortListener()
{
	m_running = false; 
	m_socket->Close(); 
	Thread::YYield();
}

void PortListener::Stop() 
{ 
	m_running =false; 
	m_socket->Close(); 
	m_listeners.DispatchOnStop(); 
}

void PortListener::Run()
{
	m_running = true;
	while ( m_running )
	{
		try
		{
			TcpSocketPtr sock = m_socket->Accept();
			if ( sock.IsNull() )
			{
				// out of memory or timeout?
				continue;
			}
			m_listeners.DispatchOnConnect( sock );
		}
		catch ( SocketException *se )
		{
			// Log the error
			Log::SWriteError("SocketException: " + String(se->Message()));
			delete se;
		}
		ValidateMem();
	}
}

#if defined(DEBUG) || defined(_DEBUG)
void PortListener::CheckMem() const
{
	m_socket.CheckMem();
	m_listeners.CheckMem();
}

void PortListener::ValidateMem() const
{
	m_socket.ValidateMem();
	m_listeners.ValidateMem();
}
#endif
