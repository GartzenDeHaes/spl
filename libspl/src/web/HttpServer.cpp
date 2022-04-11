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
#include <spl/web/server/HttpInstance.h>
#include <spl/web/server/HttpServer.h>

using namespace spl;

HttpServer::HttpServer(int port)
: m_server(new SocketSetServer(this, port)), m_handlerFact()
{
}

HttpServer::~HttpServer()
{
}

IStreamReadListenerPtr HttpServer::Create(TcpSocketPtr sock)
{
	return IStreamReadListenerPtr(new HttpInstance(sock, &m_handlerFact));
}

void HttpServer::Close()
{
	m_server->CloseAndDelete();
}

void HttpServer::Join(int timeoutms)
{
	m_server->Join(timeoutms);
}

#if defined(DEBUG) || defined(_DEBUG)
void HttpServer::CheckMem() const
{
	m_server.CheckMem();
	m_handlerFact.CheckMem();
}

void HttpServer::ValidateMem() const
{
	m_server.ValidateMem();
	m_handlerFact.ValidateMem();
}
#endif
