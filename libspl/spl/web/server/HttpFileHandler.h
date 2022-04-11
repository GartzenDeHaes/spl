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
#ifndef _filehandlerbase_h
#define _filehandlerbase_h

#include <spl/text/StringBuffer.h>
#include <spl/web/server/HttpHandler.h>

namespace spl
{
class FileHandlerBase;
typedef RefCountPtrCast<FileHandlerBase, HttpHandler, HttpHandlerPtr> FileHandlerBasePtr;

///@brief Base class for HttpHandlers that deal with files.
class FileHandlerBase : public HttpHandler
{
private:
	static const int BLOCK_SIZE;

public:
	FileHandlerBase(const String& defaultMimeType);
	virtual ~FileHandlerBase();

	///@brief Allow subclasses to alter the content prior to sending.
	virtual void ProcessFileContentBlock(StringBuffer& buf);

	virtual void ProcessRequest(HttpRequest& request, HttpResponse& response);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};
}
#endif
