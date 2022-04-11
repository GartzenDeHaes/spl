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
#include <spl/web/server/HttpHandlerFactory.h>

using namespace spl;

HttpHandlerFactory::HttpHandlerFactory()
: m_handlers(), m_handlerIdx()
{
}

HttpHandlerFactory::~HttpHandlerFactory()
{
	Clear();
}

void HttpHandlerFactory::AddHandler(HttpHandlerPtr handler)
{
	handler.ValidateMem();
	ValidateMem();

	m_handlers.Add(handler);

	for ( int x = 0; x  < handler->FileExtensions().Count(); x++ )
	{
		m_handlerIdx.Set(handler->FileExtensions().ElementAt(x).ToLower(), handler.Get());
	}
}

HttpHandler *HttpHandlerFactory::GetHandler(const String& fileExtension) const
{
	if (!m_handlerIdx.ContainsKey(fileExtension))
	{
		return NULL;
	}
	return m_handlerIdx.Get(fileExtension.ToLower());
}

void HttpHandlerFactory::Clear()
{
	ValidateMem();

	for (int x = 0; x < m_handlers.Count(); x++)
	{
		m_handlers.ElementAtRef(x) = NULL;
	}
	m_handlers.Clear();
	m_handlerIdx.Clear();
}

#ifdef DEBUG
void HttpHandlerFactory::ValidateMem() const
{
	m_handlers.ValidateMem();
	m_handlerIdx.ValidateMem();
}

void HttpHandlerFactory::CheckMem() const
{
	m_handlers.CheckMem();
	m_handlerIdx.CheckMem();
}
#endif
