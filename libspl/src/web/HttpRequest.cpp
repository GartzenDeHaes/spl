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

// RFC 2616 http://www.w3.org/Protocols/rfc2616/rfc2616.html

#include <spl/Int32.h>
#include <spl/web/HttpRequest.h>
#include <spl/net/TcpSocket.h>

using namespace spl;

HttpRequest::HttpRequest()
:	m_method("GET"),
	m_uri(),
	m_httpVersion("HTTP/1.0"),
	m_header(),
	m_body(NULL),
	m_state(HTTPREQ_STATE_METHOD),
	m_accum(128)
{
}

HttpRequest::HttpRequest(const String& uri)
:	m_method("GET"),
	m_uri(uri),
	m_httpVersion("HTTP/1.0"),
	m_header(),
	m_body(NULL),
	m_state(HTTPREQ_STATE_METHOD),
	m_accum(128)
{
}

HttpRequest::HttpRequest(const HttpRequest& req)
:	m_method(req.m_method),
	m_uri(req.m_uri),
	m_httpVersion(req.m_httpVersion),
	m_header(req.m_header),
	m_body(NULL),
	m_state(req.m_state),
	m_accum(req.m_accum)
{
	if ( NULL != req.m_body )
	{
		m_body = req.m_body->Clone();
	}
}

HttpRequest::~HttpRequest()
{
	if ( NULL != m_body )
	{
		delete m_body;
	}
}

HttpRequest& HttpRequest::operator =(const HttpRequest& req)
{
	m_method = req.m_method;
	m_uri = req.m_uri;
	m_httpVersion = req.m_httpVersion;
	m_header = req.m_header;
	m_state = req.m_state;
	m_accum = req.m_accum;

	if ( NULL != m_body )
	{
		delete m_body;
	}
	if ( NULL == req.m_body )
	{
		m_body = NULL;
	}
	else
	{
		m_body = req.m_body->Clone();
	}

	return *this;
}

bool HttpRequest::ParseLine(const byte *data, int len, int *pos)
{
	while ( *pos < len )
	{
		char ch = (char)data[(*pos)++];
		if ( ch == '\r' )
		{
			continue;
		}
		if ( ch == '\n' )
		{
			return true;
		}
		m_accum.Append( ch );
	}
	return false;
}

void HttpRequest::Parse(const Array<byte>& data, int len)
{
	int pos = 0;

	while ( pos < len )
	{
		switch ( m_state )
		{
		case HTTPREQ_STATE_METHOD:
			while ( pos < len )
			{
				char ch = (char)data[pos++];
				if ( ch == ' ' ) 
				{
					m_method = *m_accum.ToString();
					m_accum.SetLength(0);
					m_state = HTTPREQ_STATE_URI;
					break;
				}
				m_accum.Append( ch );
				if ( m_accum.Length() > 6 )
				{
					throw new Exception(String::Format("%s %s", "Invalid HTTP method", m_accum.ToString()->GetChars())->GetChars());
				}
			}
			break;

		case HTTPREQ_STATE_URI:
			while ( pos < len )
			{
				char ch = (char)data[pos++];
				if ( ch == ' ' ) 
				{
					StringPtr str = m_accum.ToString();
					m_uri.Parse(*str);
					m_uri.ValidateMem();
					m_accum.SetLength(0);
					m_state = HTTPREQ_STATE_VERSION;
					break;
				}
				m_accum.Append( ch );
			}
			break;

		case HTTPREQ_STATE_VERSION:
			if ( ParseLine(data, len, &pos) )
			{
				m_httpVersion = *m_accum.ToString();
				m_accum.SetLength(0);
				m_state = HTTPREQ_STATE_HEADERS;
			}
			break;

		case HTTPREQ_STATE_HEADERS:
			if ( ParseLine(data, len, &pos) )
			{
				if ( m_accum.Length() == 0 )
				{
					m_state = HTTPREQ_STATE_BODY;
					break;
				}
				m_header.ParseLine(m_accum.ToString());
				m_accum.SetLength(0);
			}
			break;

		case HTTPREQ_STATE_BODY:
			if ( NULL == m_body )
			{
				if ( m_header.HasHeader("Content-Type") )
				{
					String& contentType = m_header.Header("Content-Type");
					if ( contentType.Equals("application/x-www-form-urlencoded") )
					{
						m_body = new HttpRequestBodyFormData();
					}
					else
					{
						m_body = new HttpRequestBodyGeneric();
					}
				}
				else
				{
					m_body = new HttpRequestBodyGeneric();
				}
			}
			int contentLen;
			if ( m_header.HasHeader("Content-Length") )
			{
				String& scl = m_header.Header("Content-Length");
				ASSERT( Int32::IsInt(scl) );
				contentLen = Int32::Parse(scl);
			}
			else
			{
				contentLen = 0;
			}

			m_body->Parse( data, pos, len - pos, contentLen );
			pos = len;
			break;

		default:
			throw new Exception("HttpRequest::Parse: corrupted state.");
		}
	}
}

bool HttpRequest::IsComplete()
{
	if ( m_header.Count() == 0 )
	{
		return false;
	}

	int contentLen;
	if ( m_header.HasHeader("Content-Length") )
	{
		String& scl = m_header.Header("Content-Length");
		ASSERT( Int32::IsInt(scl) );
		contentLen = Int32::Parse(scl);
	}
	else
	{
		contentLen = 0;
	}

	if ( NULL != m_body && contentLen > 0 )
	{
		return m_body->ByteCount() >= contentLen;
	}
	else
	{
		// request does not have a content lenth, should check Transfer-Encoding and media type "multipart/byteranges".
		return m_state == HTTPREQ_STATE_BODY;
	}
}

HttpResponse *HttpRequest::Send()
{
	HttpResponse *resp = new HttpResponse();
	
	try
	{
		TcpSocket sock(m_uri.Host(), m_uri.Port());
		sock.Connect();
		sock.SetRecvTimeout( 30 * 1000 );
		sock.SetBlocking();

		if ( NULL == m_body )
		{
			m_header.ContentLength() = "0";
		}
		else
		{
			m_header.ContentLength() = *Int32::ToString(m_body->ByteCount());
		}

		if ( m_header.ContentType() == "" )
		{
			m_header.ContentType() = "Content-Type", "application/x-www-form-urlencoded";
		}

		StringPtr httpReq = ToString();
		sock.GetStream()->Write( httpReq->ToByteArray() );

		IStreamPtr reader = sock.GetStream();
		resp->Parse(reader);
		sock.Close();
	}
	catch ( Exception *ex )
	{
		delete resp;
		throw ex;
	}

	return resp;
}

StringPtr HttpRequest::ToString() const
{
	StringBuffer buf;

	buf.Append( m_method );
	buf.Append( ' ' );
	buf.Append( m_uri.AbsolutePath() );
	buf.Append( ' ' );
	buf.Append( m_httpVersion );
	buf.Append( "\r\n" );
	buf.Append( m_header.ToString() );

	if ( NULL != m_body )
	{
		buf.Append( m_body->ToString() );
	}

	return buf.ToString();
}

#ifdef DEBUG
void HttpRequest::ValidateMem() const
{
	m_method.ValidateMem();
	m_httpVersion.ValidateMem();
	m_accum.ValidateMem();

	if ( NULL != m_body )
	{
		ASSERT_PTR(m_body);
		m_body->ValidateMem();
	}
}

void HttpRequest::CheckMem() const
{
	m_method.CheckMem();
	m_httpVersion.CheckMem();
	m_accum.CheckMem();

	if ( NULL != m_body )
	{
		DEBUG_NOTE_MEM( m_body );
		m_body->CheckMem();
	}
}
#endif
