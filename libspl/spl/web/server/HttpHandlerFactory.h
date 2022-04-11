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
#ifndef _httphandlerfactory_h
#define _httphandlerfactory_h

#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/collection/Vector.h>
#include <spl/web/server/HttpHandler.h>

namespace spl
{
///@brief Used by HttpInstance to create handlers for requests.
class HttpHandlerFactory : public IMemoryValidate
{
private:
	Vector<HttpHandlerPtr> m_handlers;
	Hashtable<String, HttpHandler *> m_handlerIdx;

public:
	HttpHandlerFactory();
	virtual ~HttpHandlerFactory();

	///@brief Add a handler for a file extension.
	void AddHandler(HttpHandlerPtr handler);

	HttpHandler *GetHandler(const String& fileExtension) const;

	///@brief Remove and delete all handlers.
	void Clear();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};
}
#endif
