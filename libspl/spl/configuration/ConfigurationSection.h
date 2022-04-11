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
#ifndef _configurationsectoin_h
#define _configurationsection_h

#include <spl/collection/Array.h>
#include <spl/collection/List.h>
#include <spl/String.h>
#include <spl/collection/Hashtable.h>

namespace spl
{
/** 
 * @defgroup config Configuration
 * @{
 */

class ConfigurationSection;
typedef RefCountPtr<ConfigurationSection> ConfigurationSectionPtr;

class ConfigurationSection : public IMemoryValidate
{
protected:
	String m_name;
	Hashtable<String, List<StringPtr> > m_idx;

public:
	ConfigurationSection();
	ConfigurationSection(const String& name);
	ConfigurationSection(const ConfigurationSection& cs);
	virtual ~ConfigurationSection();

	ConfigurationSection& operator =(const ConfigurationSection& cs);

	inline String& Name() { return m_name; }
	
	inline Hashtable<String, List<StringPtr> >::Iterator Begin() { return m_idx.Begin(); }

	Array<String> Keys();

	inline bool ContainsKey(const String& key) { return m_idx.ContainsKey(key); }
	
	StringPtr Value(const String& key);
	List<StringPtr>& Values(const String& key);

	void Set(const String& key, const String& value);
	void Remove(const String& key);

	StringPtr ToIniString();
	StringPtr ToXmlString();

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

/** @} */
}
#endif
