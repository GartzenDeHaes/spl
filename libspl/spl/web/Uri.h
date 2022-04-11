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
#ifndef _uri_h
#define _uri_h

#include <string.h>
#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Association.h>
#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup web Web
 * @{
 */

class Uri : public IMemoryValidate, public IHashable
{
protected:
	String m_protocol;
	String m_server;
	int m_port;
	String m_path;
	String m_filename;
	String m_fileext;

	Vector<Association<String, String> > m_args;
	Hashtable<String, String> m_argIdx;

public:
	Uri(  );
	Uri( const Uri& uri );
	Uri( const String& cpuri );
	virtual ~Uri();

	Uri& operator =(const Uri& uri);

	inline String& Protocol() { return m_protocol; }
	inline String& Host() { return m_server; }
	inline int& Port() { return m_port; }
	inline String& Path() { return m_path; }
	inline String& Filename() { return m_filename; }
	inline String& FileExt() { return m_fileext; }

	inline const Vector<Association<String, String> >& Args() { return m_args; }

	inline bool HasArg(const String& argname) const { return m_argIdx.ContainsKey(argname); }

	inline String& GetArg(const String& argname) { return m_argIdx.GetRef(argname); }

	void Parse( const String& uri );

	virtual uint32 HashCode() const;

	StringPtr AbsolutePath() const;
	StringPtr ToString() const;

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

/** @} */
}
#endif
