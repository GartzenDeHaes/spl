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
#include <spl/web/server/HttpHandler.h>

using namespace spl;

HttpHandler::HttpHandler(const String& defaultMimeType)
: m_fileExtensions(), m_defaultMimeType(defaultMimeType)
{
}

HttpHandler::~HttpHandler()
{
}

void HttpHandler::ProcessRequest(HttpRequest& request, HttpResponse& response)
{
	response.AddHeader("Content-Type", m_defaultMimeType);
}

#ifdef DEBUG
void HttpHandler::ValidateMem() const
{
	m_fileExtensions.ValidateMem();
	m_defaultMimeType.ValidateMem();
}

void HttpHandler::CheckMem() const
{
	m_fileExtensions.CheckMem();
	m_defaultMimeType.CheckMem();
}
#endif
