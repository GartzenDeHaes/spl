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
#ifndef _httpinstance_h
#define _httpinstance_h

#include <spl/io/IStream.h>
#include <spl/web/HttpRequest.h>
#include <spl/web/HttpResponse.h>
#include <spl/net/TcpSocket.h>

#include <spl/web/server/HttpHandlerFactory.h>

namespace spl
{
///@brief HttpInstance is a request parser and executor.  The caller is responsable
/// for cleaning up/deleting the socket and factory.
class HttpInstance : public IStreamReadListener
{
private:
	HttpRequest m_request;
	TcpSocketPtr m_sock;
	HttpHandlerFactory *m_handlerFact;

public:
	HttpInstance(TcpSocketPtr sock, HttpHandlerFactory *handlerFact);
	virtual ~HttpInstance();

	inline TcpSocketPtr ClientConnection() { return m_sock; }

	virtual void IStreamRead_OnClose();
	virtual void IStreamRead_OnRead(const Array<byte>& buf, int len);
	virtual void IStreamRead_OnError( const String& msg );

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};
}
#endif
