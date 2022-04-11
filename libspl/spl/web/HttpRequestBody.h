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
#ifndef _httprequestbody_h
#define _httprequestbody_h

#include <spl/collection/Association.h>
#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>
#include <spl/io/TextStream.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup web Web
 * @{
 */

class IHttpRequestBody : public IMemoryValidate
{
public:
	inline IHttpRequestBody() {}
	virtual ~IHttpRequestBody();

	virtual void Parse( const Array<byte>& cp, int pos, int len, int contentLen ) = 0;

	virtual int ByteCount() const = 0;

	virtual IHttpRequestBody *Clone() const = 0;

	virtual void Write( TextWriter& strm ) const = 0;

	virtual StringPtr ToString() const = 0;

	virtual StringPtr GetValue(const String& key);
	virtual bool HasKey(const String& key);
};

class HttpRequestBodyGeneric : public IHttpRequestBody
{
protected:
	StringBuffer m_buf;

public:
	HttpRequestBodyGeneric();
	HttpRequestBodyGeneric(const HttpRequestBodyGeneric& b);
	virtual ~HttpRequestBodyGeneric();

	virtual int ByteCount() const;

	virtual IHttpRequestBody *Clone() const;

	virtual void Parse( const Array<byte>& cp, int pos, int len, int contentLen );

	virtual void Write( TextWriter& strm ) const;

	virtual StringPtr ToString() const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

class HttpRequestBodyFormData : public IHttpRequestBody
{
protected:
	enum State
	{
		HTTPBODY_STATE_NAME = 0,
		HTTPBODY_STATE_VAL = 1
	};

	Vector<Association<StringPtr, StringPtr> > m_data;
	Hashtable<String, StringPtr> m_idx;
	int m_byteCount;
	HttpRequestBodyFormData::State m_state;
	StringBuffer m_accum;

public:
	HttpRequestBodyFormData();
	HttpRequestBodyFormData(const HttpRequestBodyFormData& body);
	virtual ~HttpRequestBodyFormData();

	HttpRequestBodyFormData& operator =(const HttpRequestBodyFormData& body);

	virtual int ByteCount() const;

	virtual IHttpRequestBody *Clone() const;

	virtual void Parse( const Array<byte>& cp, int pos, int len, int contentLen );

	inline int Count() const { return m_data.Count(); }

	virtual bool HasKey(const String& key);
	virtual StringPtr GetValue(const String& key);
	inline Association<StringPtr, StringPtr>& Item(int idx) { return m_data.ElementAtRef(idx); }

	virtual void Write( TextWriter& strm ) const;

	virtual StringPtr ToString() const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
