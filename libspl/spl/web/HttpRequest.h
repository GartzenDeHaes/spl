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
#ifndef _httprequest_h
#define _httprequest_h

#include <spl/collection/Array.h>
#include <spl/web/HttpRequestBody.h>
#include <spl/web/HttpHeader.h>
#include <spl/web/HttpResponse.h>
#include <spl/Memory.h>
#include <spl/text/StringBuffer.h>
#include <spl/web/Uri.h>

namespace spl
{
/** 
 * @defgroup web Web
 * @{
 */

class HttpRequest : public IMemoryValidate
{
private:
	enum State
	{
		HTTPREQ_STATE_METHOD = 0,
		HTTPREQ_STATE_URI = 1,
		HTTPREQ_STATE_VERSION = 2,
		HTTPREQ_STATE_HEADERS = 3,
		HTTPREQ_STATE_BODY = 4
	};

protected:
	String m_method;
	Uri m_uri;
	String m_httpVersion;	// HTTP/1.0
	HttpHeader m_header;
	IHttpRequestBody *m_body;

	HttpRequest::State m_state;
	StringBuffer m_accum;

	bool ParseLine(const byte *data, int len, int *pos);

public:
	HttpRequest();
	HttpRequest(const String& uri);
	HttpRequest(const HttpRequest& req);
	virtual ~HttpRequest();

	HttpRequest& operator =(const HttpRequest& req);

	inline String& Method() { return m_method; }
	inline Uri& URI() { return m_uri; }

	void Parse(const Array<byte>& data, int len);
	bool IsComplete();

	HttpResponse *Send();

	inline HttpHeader& Headers() { return m_header; }
	inline IHttpRequestBody* Body() { return m_body; }

	StringPtr ToString() const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
