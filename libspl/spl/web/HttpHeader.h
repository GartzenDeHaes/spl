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
#ifndef _httpheader_h
#define _httpheader_h

#include <spl/Debug.h>
#include <spl/collection/Association.h>
#include <spl/collection/Hashtable.h>
#include <spl/web/HttpCookie.h>
#include <spl/io/IStream.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup web Web
 * @{
 */

class HttpHeader : public IMemoryValidate
{
protected:
	Vector<Association<String, String> > m_headers;
	Hashtable<String, int> m_headerIdx;
	Hashtable<String, HttpCookie> m_cookies;

public:
	inline HttpHeader(const HttpHeader& hdr)
	: m_headers(hdr.m_headers), m_headerIdx(hdr.m_headerIdx), m_cookies(hdr.m_cookies)
	{
	}

	inline HttpHeader() : m_headers(20), m_headerIdx(), m_cookies()
	{
	}
	
	virtual ~HttpHeader();

	inline HttpHeader& operator =(const HttpHeader& hdr)
	{
		m_headers = hdr.m_headers;
		m_headerIdx = hdr.m_headerIdx;
		m_cookies = hdr.m_cookies;
		return *this;
	}

	String& Header(const String& name);

	inline bool HasHeader(const String& name) const 
	{ 
		return m_headerIdx.ContainsKey(name.ToUpper()); 
	}

	inline bool HasCookie(const String& name)
	{
		return m_cookies.ContainsKey(name);
	}
	
	void SetCookie(const String& key, const String& value);
	
	inline HttpCookie& GetCookie(const String& key)
	{
		return m_cookies.GetRef(key);
	}

	inline String& ContentLength()
	{ 
		return Header("CONTENT-LENGTH"); 
	}

	inline String& ContentType()
	{ 
		return Header("CONTENT-TYPE"); 
	}

	inline int Count() const 
	{ 
		return m_headers.Count(); 
	}

	void ParseLine( const String& line );

	void Write(spl::IStream& stream) const;
	StringPtr ToString() const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
