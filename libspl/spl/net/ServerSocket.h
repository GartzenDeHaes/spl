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
#ifndef _serversocket_h
#define _serversocket_h

#include <spl/net/TcpSocket.h>

namespace spl
{
/** 
 * @defgroup socket Sockets
 * @ingroup network
 * @{
 */

class ServerSocket;
typedef RefCountPtr<ServerSocket> ServerSocketPtr;

/** @brief Specialized socket for accepting connections.
 *	@ref Socket
 *	@ref TcpSocket
 */
class ServerSocket : public IMemoryValidate
{
private:
	// Copy constructor doesn't make sense for this class
	inline ServerSocket(const ServerSocket& sock) {}
	inline void operator =(const ServerSocket& pmp) {throw Exception();}

protected:
	Socket m_sock;

	void Bind ();
	void Listen (int iNumBuf);

public:
	ServerSocket( int port, int iNumBuf );
	virtual ~ServerSocket();

	TcpSocketPtr Accept (  ); // call accept
	inline void Close() { m_sock.Close(); }
	inline void Shutdown( int i=SD_BOTH ) {m_sock.Shutdown(i);}

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const {m_sock.CheckMem();}
	virtual void ValidateMem() const {m_sock.ValidateMem();}
#endif
};

/** @} */
}
#endif
