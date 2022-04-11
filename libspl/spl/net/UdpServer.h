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
#ifndef _udpserver_h
#define _udpserver_h

#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/collection/Array.h>
#include <spl/Delegate.h>
#include <spl/threading/ThreadStartDelegate.h>
#include <spl/net/UdpSocket.h>
#include <spl/threading/Thread.h>

namespace spl
{
class UdpServer;
typedef RefCountPtr<UdpServer> UdpServerPtr;
typedef WeakReference<UdpServer, UdpServerPtr> UdpServerRef;

/** @brief Listens at a port for UDP packets and dispatches them to a delegate.
 *	@ref DelegateThreeParameter
 *	@ref UdpSocket
 */
class UdpServer : public IMemoryValidate
{
private:
	inline UdpServer(const UdpServer& s) : m_sock(0) {}
	inline UdpServer& operator =(const UdpServer& s) {throw Exception();}

	void Run();

protected:
	ThreadStartDelegate<UdpServer> m_thread;
	UdpSocket m_sock;
	RefCountPtr<IDelegateThreeParameter<UdpSocket&, Array<byte>&, int> > m_callback;
	bool m_running;
	int m_bufSize;

public:
	UdpServer(int port, RefCountPtr<IDelegateThreeParameter<UdpSocket&, Array<byte>&, int> > callback, int bufferSize = 512);
	virtual ~UdpServer();

	inline int Port() const { return m_sock.Port(); }

	void Close();

	void Join();

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};
}
#endif
