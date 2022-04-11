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
#ifndef _cdirectory_h
#define _cdirectory_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/Memory.h>
#include <spl/io/Permissions.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup file File System
 * @ingroup io
 * @{
 */

/** @brief File system directory operations.
 *
 */
class Directory : public IMemoryValidate, public IComparable
{
protected:
	String m_raw;
	RefCountPtr<Vector<StringPtr> > m_parts;
	
	static char m_seperator;

public:
	Directory( const String& director );
	Directory( const Directory& dir );
	virtual ~Directory();

	Directory& operator =(const Directory& dir);

	Directory Parent() const;

	inline Vector<StringPtr> ListFiles(const String& filter) const
	{
		return ListFiles(m_raw, filter);
	}

	inline StringPtr RemoveTrailingSeperator() const
	{
		return RemoveTrailingSeperator(ToString());
	}

	inline bool IsDirectory() const
	{
		return IsDirectory(m_raw);
	}

	virtual uint32 HashCode() const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	StringPtr ToString() const;

	static void ChangeWorkingDirectory(const String& directory);
	
	static StringPtr GetWorkingDirectory();

	static Vector<StringPtr> ListFiles(const String& directory, const String& filter);
	
	inline static char SeperatorChar() { return m_seperator; }

	static StringPtr RemoveTrailingSeperator(const String& path);

	static bool IsDirectory(const String &path);

	static void Delete(const String& path);
	static void Rename(const String& oldname, const String& newname);
	static void Create(const String& path, const Permissions& perm = Permissions::DefaultDirPerms);

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

REGISTER_TYPEOF(84, Directory);

/** @} */
}
#endif
