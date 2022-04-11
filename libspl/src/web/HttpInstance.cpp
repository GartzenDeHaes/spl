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
#include <spl/web/server/HttpHandlerFactory.h>

using namespace spl;

HttpInstance::HttpInstance(TcpSocketPtr sock, HttpHandlerFactory *handlerFact)
: m_request(), m_sock(sock), m_handlerFact(handlerFact)
{
	m_sock->SetLingerOn();
}

HttpInstance::~HttpInstance()
{
}

void HttpInstance::IStreamRead_OnClose()
{
	ValidateMem();
	printf("Connection from %s was closed.\n", m_sock->GetRemoteIp().GetChars());
}

void HttpInstance::IStreamRead_OnRead(const Array<byte>& buf, int len)
{
	m_request.Parse(buf, len);

	if (m_request.IsComplete())
	{
		HttpResponse response;
		HttpHandler *handler = m_handlerFact->GetHandler(m_request.URI().FileExt());
		if (NULL == handler)
		{
			response.StatusCode() = 404;
			response.GetBodyStream()->Write(String("<html><body><h1>404 File not found</h1></body></html>").ToByteArray());
		}
		else
		{
			handler->ProcessRequest(m_request, response);
		}
		response.Write(*m_sock->GetStream());
		m_sock->Close();
	}
}

void HttpInstance::IStreamRead_OnError( const String& msg )
{
	ValidateMem();
	printf("%s\n", msg.GetChars());
}

#if defined(DEBUG) || defined(_DEBUG)
void HttpInstance::CheckMem() const
{
	m_request.CheckMem();
	m_sock.CheckMem();
}

void HttpInstance::ValidateMem() const
{
	m_request.ValidateMem();
	m_sock.ValidateMem();
}
#endif
