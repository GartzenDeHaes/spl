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
#include <spl/web/HttpHeader.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

HttpHeader::~HttpHeader()
{
}

String& HttpHeader::Header(const String& _name)
{
	StringPtr name = _name.ToUpper();
	if ( ! m_headerIdx.ContainsKey(*name) )
	{
		int idx = m_headers.Count();
		m_headers.Add( Association<String, String>(_name, String()) );
		m_headerIdx.Set( *name, idx );
	}
	return m_headers.ElementAtRef( m_headerIdx.Get(*name) ).ValueRef();
}

void HttpHeader::SetCookie(const String& key, const String& value)
{
	HttpCookie cookie(key, value);
	m_cookies.Set(key, cookie);
	m_headers.Add(Association<String, String>("Set-Cookie", cookie.ToString()));
	m_headerIdx.Set("SET-COOKIE", m_headers.Count());
}

void HttpHeader::Write(spl::IStream& stream) const
{
	StringPtr header = ToString();
	stream.Write( header->ToByteArray() );
}

void HttpHeader::ParseLine( const String& line )
{
	int colonPos = line.IndexOf(':');
	if (0 > colonPos)
	{
		throw new InvalidArgumentException("Expected a colon " + line);
	}
	StringPtr key = line.Substring(0, colonPos);
	StringPtr val = line.Substring(colonPos + 1)->Trim();
	Header(*key) = *val->Trim();
	m_headerIdx.Set(key->ToUpper(), m_headers.Count()-1);
	ASSERT(Header(key) == *val);
	
	if (key->EqualsIgnoreCase("Cookie"))
	{
		HttpCookiePtr cookie(HttpCookie::Parse(val));
		m_cookies.Set(cookie->Key(), *cookie);
	}
}

StringPtr HttpHeader::ToString() const
{
	StringBuffer buf;

	int headcount = m_headers.Count();
	for ( int x = 0; x < headcount; x++ )
	{
		Association<String, String>& pair = m_headers.ElementAtRef(x);
		buf.Append( pair.KeyRef() );
		buf.Append( ":\t" );
		buf.Append( pair.ValueRef() );
		buf.Append( "\r\n" );	
	}

	if ( m_headers.Count() > 0 )
	{
		buf.Append( "\r\n" );
	}
	else
	{
		buf.Append( "\r\n\r\n" );
	}

	return buf.ToString();
}

#ifdef DEBUG
void HttpHeader::ValidateMem() const
{
	m_headers.ValidateMem();
	m_headerIdx.ValidateMem();
	m_cookies.ValidateMem();
}

void HttpHeader::CheckMem() const
{
	m_headers.CheckMem();
	m_headerIdx.CheckMem();
	m_cookies.CheckMem();
}
#endif
