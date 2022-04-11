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
#ifndef _stringtable_h
#define _stringtable_h

#include <spl/collection/Hashtable.h>
#include <spl/collection/List.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

/** Table of strings to reduce memory usage.
 */
class StringTable : public IMemoryValidate
{
protected:
	Hashtable<int, Vector<String *> *> m_stringIdx;
	Vector<String *> m_strings;
	List<Vector<String *> *> m_lists;

public:
	StringTable(const StringTable& st);
	StringTable();
	virtual ~StringTable();

	StringTable& operator =(const StringTable& st);

	void Clear();

	inline int Count() const { return m_strings.Count(); }

	inline String *GetPtr(StringBuffer *str) { return GetPtr(str->GetChars()); }
	inline String *GetPtr(const StringBuffer& str) { return GetPtr(str.GetChars()); }
	inline String *GetPtr(String *str) { return GetPtr(str->GetChars()); }
	inline String *GetPtr(const String& str) { return GetPtr(str.GetChars()); }
	String *GetPtr(const char *str);
	inline String* GetPtr(int idx) const { return m_strings.ElementAt(idx); }

	inline String& Get(StringBuffer *str) { return *GetPtr(str->GetChars()); }
	inline String& Get(const StringBuffer& str) { return *GetPtr(str.GetChars()); }
	inline String& Get(String *str) { return *GetPtr(str->GetChars()); }
	inline String& Get(const String& str) { return *GetPtr(str.GetChars()); }
	inline String& Get(const char *str) { return *GetPtr(str); }
	inline String& Get(int idx) const { return *m_strings.ElementAt(idx); }

	int PositionOf(const String& str);
	inline int PositionOf(const String* str) { return PositionOf(*str); }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 406, StringTable );

/** @} */
}
#endif
