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
#ifndef _configurationsettings_h
#define _configurationsettings_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/configuration/ConfigurationSection.h>
#include <spl/io/DelimitedFile.h>
#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
//#include <spl/xml/XmlDocument.h>

namespace spl
{
/** 
 * @defgroup config Configuration
 * @{
 */

class ConfigurationSettings;
typedef RefCountPtr<ConfigurationSettings> ConfigurationSettingsPtr;

/** @brief Reads INI or XML "app.config" style configuration files.
 */
class ConfigurationSettings : public IMemoryValidate
{
protected:
	String m_filename;
	Hashtable<String, ConfigurationSection> m_sections;

public:
	ConfigurationSettings(const String& filename);
	ConfigurationSettings(const ConfigurationSettings& config);
	ConfigurationSettings();
	virtual ~ConfigurationSettings();

	ConfigurationSettings& operator =(const ConfigurationSettings& config);

	ConfigurationSection& Section(const String& name);
	inline bool HasSection(const String& name) const { return m_sections.ContainsKey(name); }

	void Add(const String& section, const String& key, const String& value);
	inline void Add(const String& key, const String& value) { Add("", key, value); }
	bool ContainsKey(const String& section, const String& key);
	inline bool ContainsKey(const String& key) { return ContainsKey("", key); }
	
	StringPtr Value(const String& section, const String& key);
	inline StringPtr Value(const String& key) { return Value("", key); }
	List<StringPtr>& Values(const String& section, const String& key);
	inline List<StringPtr>& Values(const String& key) { return Values("", key); }

	//void Load(XmlDocument& appconfig);
	void Load(const DelimitedFile& inifile);
	void Load(const String& filename);

	StringPtr ToXmlString();
	StringPtr ToIniString();

	void SaveToIniFile(const String& filename);
	//void SaveToXmlFile(const String& filename);
	void SaveToRegistry();

	static ConfigurationSettingsPtr LoadAppConfig();
	static String AppConfigName();

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 410, ConfigurationSettings );

/** @} */
}
#endif
