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
#include <spl/web/HttpRequestBody.h>
#include <spl/web/HttpUtility.h>
#include <spl/io/Stream.h>
#include <spl/io/MemoryStream.h>

using namespace spl;

IHttpRequestBody::~IHttpRequestBody()
{
}

StringPtr IHttpRequestBody::GetValue(const String& key)
{
	return StringPtr();
}

bool IHttpRequestBody::HasKey(const String& key)
{
	return false;
}

HttpRequestBodyGeneric::HttpRequestBodyGeneric()
: m_buf()
{
}

HttpRequestBodyGeneric::HttpRequestBodyGeneric(const HttpRequestBodyGeneric& b)
: m_buf( b.m_buf )
{
}

HttpRequestBodyGeneric::~HttpRequestBodyGeneric()
{
}

int HttpRequestBodyGeneric::ByteCount() const
{
	return m_buf.Length();
}

IHttpRequestBody *HttpRequestBodyGeneric::Clone() const
{
	return new HttpRequestBodyGeneric(*this);
}

void HttpRequestBodyGeneric::Parse( const Array<byte>& cp, int pos, int len, int contentLen )
{
	m_buf.Append( &((const char *)cp.Data())[pos], len );
}

void HttpRequestBodyGeneric::Write( TextWriter& strm ) const
{
	strm.Write( m_buf.GetChars() );
}

StringPtr HttpRequestBodyGeneric::ToString() const
{
	return m_buf.ToString();
}

#ifdef DEBUG
void HttpRequestBodyGeneric::ValidateMem() const
{
	m_buf.ValidateMem();
}

void HttpRequestBodyGeneric::CheckMem() const
{
	m_buf.CheckMem();
}
#endif

HttpRequestBodyFormData::HttpRequestBodyFormData()
: m_data(), m_byteCount(0), m_state(HTTPBODY_STATE_NAME), m_accum()
{
}

HttpRequestBodyFormData::HttpRequestBodyFormData(const HttpRequestBodyFormData& body)
: m_data(body.m_data), m_byteCount(body.m_byteCount), m_state(body.m_state), m_accum(body.m_accum)
{
}

HttpRequestBodyFormData::~HttpRequestBodyFormData()
{
}

HttpRequestBodyFormData& HttpRequestBodyFormData::operator =(const HttpRequestBodyFormData& body)
{
	m_data = body.m_data;
	m_byteCount = body.m_byteCount;
	m_state = body.m_state;
	m_accum = body.m_accum;

	return *this;
}

IHttpRequestBody *HttpRequestBodyFormData::Clone() const
{
	return new HttpRequestBodyFormData(*this);
}

StringPtr HttpRequestBodyFormData::GetValue(const String& key)
{
	return m_idx.Get(key);
}

bool HttpRequestBodyFormData::HasKey(const String& key)
{
	return m_idx.ContainsKey(key);
}

void HttpRequestBodyFormData::Parse( const Array<byte>& cp, int pos, int len, int contentLen )
{
	m_byteCount += len;
	StringPtr key;

	for ( int x = 0; x < len; x++ )
	{
		char ch = cp[x+pos];

		switch ( m_state )
		{
		case HTTPBODY_STATE_NAME:
			if ( '=' == ch || '&' == ch || '\0' == ch )
			{
				StringPtr key = m_accum.ToString();
				m_data.Add( Association<StringPtr, StringPtr>(key, StringPtr(new String(""))) );
				m_idx.Set(key, m_data.PeekRef().Value());
				m_accum.SetLength(0);
				m_state = HTTPBODY_STATE_VAL;
				break;
			}

			m_accum.Append( ch );
			break;

		case HTTPBODY_STATE_VAL:
			if ( ch == '&' || ch == '\0' || ch == '\r' || ch == '\n' )
			{
				*m_data.PeekRef().Value() = m_accum.ToString();
				if (m_data.PeekRef().Key().IsNotNull())
				{
					m_idx.Set(m_data.PeekRef().Key(), m_data.PeekRef().Value());
				}
				m_accum.SetLength(0);
				m_state = HTTPBODY_STATE_NAME;
				break;
			}

			m_accum.Append( ch );
			break;

		default:
			throw new Exception("HttpRequestBody::Parse: state corrupted");
		}
	}

	if ( m_state == HTTPBODY_STATE_VAL )
	{
		*m_data.PeekRef().Value() = m_accum.ToString();
		if (m_data.PeekRef().Key().IsNotNull())
		{
			m_idx.Set(m_data.PeekRef().Key(), m_data.PeekRef().Value());
		}
		m_accum.SetLength(0);
		m_state = HTTPBODY_STATE_NAME;
	}
}

int HttpRequestBodyFormData::ByteCount() const 
{ 
	return m_byteCount; 
}

void HttpRequestBodyFormData::Write( TextWriter& strm ) const
{
	int count = m_data.Count();
	for ( int x = 0; x < count; x++ )
	{
		String& key = m_data.ElementAtRef(x).KeyRef();
		String& val = m_data.ElementAtRef(x).ValueRef();

		if ( x > 0 )
		{
			strm.Write("&");
		}

		if ( HttpUtility::UrlEncodeRequired(key) )
		{
			strm.Write( *HttpUtility::UrlEncode(key) );
		}
		else
		{
			strm.Write( key );
		}
		strm.Write("=");
		if ( HttpUtility::UrlEncodeRequired(val) )
		{
			strm.Write( *HttpUtility::UrlEncode(val) );
		}
		else
		{
			strm.Write( val );
		}
	}
}

StringPtr HttpRequestBodyFormData::ToString() const
{
	MemoryStreamPtr ms(new MemoryStream());
	TextWriter tw(ms);
	Write( tw );
	return ms->ToString();
}

#ifdef DEBUG
void HttpRequestBodyFormData::ValidateMem() const
{
	m_idx.ValidateMem();
	m_data.ValidateMem();
}

void HttpRequestBodyFormData::CheckMem() const
{
	m_idx.CheckMem();
	m_data.CheckMem();
}
#endif
