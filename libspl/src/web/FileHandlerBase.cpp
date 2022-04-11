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
#include <spl/io/File.h>
#include <spl/Int32.h>
#include <spl/io/TextStream.h>
#include <spl/web/server/HttpFileHandler.h>

using namespace spl;

const int FileHandlerBase::BLOCK_SIZE = 512;

FileHandlerBase::FileHandlerBase(const String& defaultMimeType)
: HttpHandler(defaultMimeType)
{
}

FileHandlerBase::~FileHandlerBase()
{
}

void FileHandlerBase::ProcessFileContentBlock(StringBuffer& buf)
{
}

void FileHandlerBase::ProcessRequest(HttpRequest& request, HttpResponse& response)
{
	HttpHandler::ProcessRequest(request, response);

	String localPath = request.URI().Path();
	if (localPath.StartsWith("/"))
	{
		localPath = "." + localPath;
	}
	else
	{
		localPath = "./" + localPath;
	}
	if (!localPath.EndsWith('/'))
	{
		localPath = localPath + "/";
	}
	localPath = localPath + request.URI().Filename();

	if (!File::Exists(localPath))
	{
		response.StatusCode() = 404;
		return;
	}

	if (request.Method() == "GET")
	{
		TextReader *reader = new TextReader(File::OpenText(localPath));
		StringPtr html = reader->ReadToEnd();
		reader->Close();
		delete reader;

		TextWriter *writer = new TextWriter(response.GetBodyStream());
		writer->Write(html);
		delete writer;
	}
	else if (request.Method() == "HEAD")
	{
		// do nothing.
		response.StatusCode() = 200;
	}
	else
	{
		response.StatusCode() = 405;
		response.AddHeader("Allow", "GET,HEAD");
	}
}

#ifdef DEBUG
void FileHandlerBase::ValidateMem() const
{
	HttpHandler::ValidateMem();
}

void FileHandlerBase::CheckMem() const
{
	HttpHandler::CheckMem();
}
#endif
