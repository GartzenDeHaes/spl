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
#include <spl/Int32.h>
#include <spl/web/HttpResponse.h>
#include <spl/Int32.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

HttpResponse::HttpResponse()
:	m_httpVersion("HTTP/1.0"), 
	m_statusCode(200), 
	m_header(), 
	m_body(new MemoryStream()), 
	m_state(HTTPRES_STATE_VERSION), 
	m_accum()
{
}

HttpResponse::HttpResponse(const HttpResponse& resp)
:	m_httpVersion(resp.m_httpVersion), 
	m_statusCode(resp.m_statusCode), 
	m_header(resp.m_header), 
	m_body(new MemoryStream(*resp.m_body)), 
	m_state(resp.m_state), 
	m_accum()
{
}

HttpResponse::~HttpResponse()
{
}

HttpResponse& HttpResponse::operator =(const HttpResponse& resp)
{
	m_httpVersion = resp.m_httpVersion;
	m_statusCode = resp.m_statusCode;
	m_header = resp.m_header;
	*m_body = *resp.m_body;

	return *this;
}

void HttpResponse::Redirect(const String& location)
{
	m_statusCode = 302;
	m_header.Header("Location") = location;
}

void HttpResponse::Write( IStream& strm )
{
	StringBuffer buf;

	m_header.Header("Content-Length") = *Int32::ToString( ContentLength() );

	buf.Append( m_httpVersion );
	buf.Append( ' ' );
	buf.Append( Int32::ToString(m_statusCode) );
	buf.Append( ' ' );
	buf.Append( GetResponseText( m_statusCode ) );
	buf.Append( "\r\n" );
	buf.Append( m_header.ToString() );

	strm.Write( buf.ToByteArray() );

	Array<byte> bbuf(512);
	int count;
	while ( (count = m_body->Read(bbuf)) > 0 )
	{
		strm.Write(bbuf, 0, count);
	}
}

void HttpResponse::Parse( IStream& strm )
{
	int chb = strm.ReadByte();
	Array<byte> chbuf(1);

	while ( chb > 0 )
	{
		chbuf[0] = (byte)chb;
		Parse( chbuf, 1 );

		if ( HTTPRES_STATE_BODY == m_state )
		{
			break;
		}

		chbuf[0] = strm.ReadByte();
	}

	Array<byte> bbuf(512);
	int count;
	while ( (count = strm.Read(bbuf)) > 0 )
	{
		Parse(bbuf, count);
	}
}

void HttpResponse::Parse(const Array<byte>& data, int len)
{
	if ( m_state == HTTPRES_STATE_BODY )
	{
		m_body->Write( data, 0, len );
		return;
	}

	for ( int x = 0; x < len; x++ )
	{
		char ch = (char)data[x];

		switch ( m_state )
		{
		case HTTPRES_STATE_VERSION:
			if ( ch == ' ' )
			{
				m_httpVersion = *m_accum.ToString();
				m_accum.SetLength(0);
				m_state = HTTPRES_STATE_CODE;
				break;
			}
			m_accum.Append( ch );
			break;

		case HTTPRES_STATE_CODE:
			if ( ch == ' ' )
			{
				m_statusCode = m_accum.ToInt();
				m_accum.SetLength(0);
				m_state = HTTPRES_STATE_MESSAGE;
				break;
			}
			m_accum.Append( ch );
			break;

		case HTTPRES_STATE_MESSAGE:
			if ( ch == '\r' )
			{
				break;
			}
			if ( ch == '\n' )
			{
				m_accum.SetLength(0);
				m_state = HTTPRES_STATE_HEADERS;
				break;
			}
			m_accum.Append( ch );
			break;

		case HTTPRES_STATE_HEADERS:
			if ( ch == '\r' )
			{
				break;
			}
			if ( ch == '\n' )
			{
				if ( m_accum.Length() == 0 )
				{
					m_state = HTTPRES_STATE_BODY;
				}
				else
				{
					StringPtr headerLine = m_accum.ToString();
					m_header.ParseLine( *headerLine );
					m_accum.SetLength(0);
				}
				break;
			}
			else
			{
				m_accum.Append( ch );
			}
			break;

		case HTTPRES_STATE_BODY:
			m_body->WriteByte( (byte)ch );
			break;

		default:
			throw new Exception("Corrupted state in HttpResponse::Parse");
		}
	}
}

#ifdef DEBUG
void HttpResponse::ValidateMem() const
{
	m_httpVersion.ValidateMem();
	m_header.ValidateMem();
	m_body.ValidateMem();
}

void HttpResponse::CheckMem() const
{
	m_httpVersion.CheckMem();
	m_header.CheckMem();
	m_body.CheckMem();
}
#endif

String HttpResponse::GetResponseText(int responseCode)
{
	switch (responseCode)
	{
		case 100: return String("Continue");
		case 101: return String("Switching Protocols");
		case 102: return String("Processing");
		case 200: return String("OK");
		case 201: return String("Created");
		case 202: return String("Accepted");
		case 203: return String("Non-Authoritative Information");
		case 204: return String("No Content");
		case 205: return String("Reset Content");
		case 206: return String("Partial Content");
		case 207: return String("Multi-Status");
		case 300: return String("Multiple Choices");
		case 301: return String("Moved Permanently");
		case 302: return String("Found");
		case 303: return String("See Other");
		case 304: return String("Not Modified");
		case 305: return String("Use Proxy");
		case 307: return String("Temporary Redirect");
		case 400: return String("Bad Request");
		case 401: return String("Unauthorized");
		case 402: return String("Payment Required");
		case 403: return String("Forbidden");
		case 404: return String("Not Found");
		case 405: return String("Method Not Allowed");
		case 406: return String("Not Acceptable");
		case 407: return String("Proxy Authentication Required");
		case 408: return String("Request Timeout");
		case 409: return String("Conflict");
		case 410: return String("Gone");
		case 411: return String("Length Required");
		case 412: return String("Precondition Failed");
		case 413: return String("Request Entity Too Large");
		case 414: return String("Request-Uri Too Long");
		case 415: return String("Unsupported Media Type");
		case 416: return String("Requested Range Not Satisfiable");
		case 417: return String("Expectation Failed");
		case 422: return String("Unprocessable Entity");
		case 423: return String("Locked");
		case 424: return String("Failed Dependency");
		case 500: return String("Internal Server Error");
		case 501: return String("Not Implemented");
		case 502: return String("Bad Gateway");
		case 503: return String("Service Unavailable");
		case 504: return String("Gateway Timeout");
		case 505: return String("Http Version Not Supported");
		case 507: return String("Insufficient Storage");
		default:
			return String("Unknown");
	}
}
