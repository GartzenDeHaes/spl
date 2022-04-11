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
#ifndef _httphandler_h
#define _httphandler_h

#include <spl/web/HttpResponse.h>
#include <spl/web/HttpRequest.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
class HttpHandler;
typedef RefCountPtr<HttpHandler> HttpHandlerPtr;

///@brief Base class for handlers used by HttpInstance to process http requests.
class HttpHandler : public IMemoryValidate
{
protected:
	Vector<String> m_fileExtensions;
	String m_defaultMimeType;

public:
	HttpHandler(const String& defaultMimeType);
	virtual ~HttpHandler();

	virtual void ProcessRequest(HttpRequest& request, HttpResponse& response);

	inline const String& MimeType() const
	{
		return m_defaultMimeType;
	}

	inline const Vector<String>& FileExtensions() const
	{
		return m_fileExtensions;
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};
}
#endif
