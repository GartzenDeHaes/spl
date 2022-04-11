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
#ifndef _httpserver_h
#define _httpserver_h

#include <spl/web/HttpHeader.h>
#include <spl/web/HttpRequest.h>
#include <spl/web/HttpRequestBody.h>
#include <spl/web/HttpResponse.h>
#include <spl/web/HttpUtility.h>
#include <spl/web/Uri.h>
#include <spl/net/PooledSocketSet.h>

#include <spl/web/server/HttpHandlerFactory.h>

namespace spl
{
class HttpServer;
typedef RefCountPtr<HttpServer> HttpServerPtr;

///@brief HTTP service class.
class HttpServer : public IServerConnectionFactory
{
private:
	ISocketServicePtr m_server;
	HttpHandlerFactory m_handlerFact;

public:
	HttpServer(int port);
	virtual ~HttpServer();

	///@brief Add a handler for a file extension.
	inline void AddHandler(HttpHandlerPtr handler) { m_handlerFact.AddHandler(handler); }

	virtual IStreamReadListenerPtr Create(TcpSocketPtr sock);

	void Close();
	void Join(int timeoutms);

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};
}
#endif
