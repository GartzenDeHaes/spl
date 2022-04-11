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
#ifndef _httpresponse_h
#define _httpresponse_h

#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/web/HttpHeader.h>
#include <spl/Memory.h>
#include <spl/io/Stream.h>
#include <spl/io/MemoryStream.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>

namespace spl
{
/** 
 * @defgroup web Web
 * @{
 */

class HttpResponse : public IMemoryValidate
{
private:
	enum State
	{
		HTTPRES_STATE_VERSION = 0,
		HTTPRES_STATE_CODE = 1,
		HTTPRES_STATE_MESSAGE = 2,
		HTTPRES_STATE_HEADERS = 3,
		HTTPRES_STATE_BODY = 4
	};

protected:
	String m_httpVersion;
	int m_statusCode;
	HttpHeader m_header;
	MemoryStreamPtr m_body;

	StringBuffer m_accum;
	enum HttpResponse::State m_state;

public:
	HttpResponse();
	HttpResponse(const HttpResponse& resp);
	virtual ~HttpResponse();

	HttpResponse& operator =(const HttpResponse& resp);

	inline int& StatusCode() { return m_statusCode; }
	
	inline void AddHeader(const String& header, const String& value) { m_header.Header(header) = value; }
	inline HttpHeader& Headers() { return m_header; }
	
	void Redirect(const String& location);

	inline spl::IStreamPtr GetBodyStream() { return m_body; }

	inline int ContentLength() { return m_body->Length(); }

	void Write( spl::IStream& strm );

	void Parse( const Array<byte>& data, int len );
	void Parse( spl::IStream& strm );

	static String GetResponseText(int responseCode);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
