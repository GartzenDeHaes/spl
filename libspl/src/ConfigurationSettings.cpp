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
#include <spl/configuration/ConfigurationSettings.h>
#include <spl/io/Directory.h>
#include <spl/Environment.h>
#include <spl/Exception.h>
#include <spl/io/File.h>
#include <spl/text/StringBuffer.h>
//#include <spl/xml/XmlElement.h>

#ifdef _WINDOWS
#include <spl/configuration/windows/WinRegKey.h>
#endif

using namespace spl;

ConfigurationSettings::ConfigurationSettings()
: m_sections(), m_filename()
{
}

ConfigurationSettings::ConfigurationSettings(const String& filename)
: m_sections(), m_filename()
{
	Load(filename);
}

ConfigurationSettings::ConfigurationSettings(const ConfigurationSettings& config)
: m_filename(config.m_filename), m_sections(config.m_sections)
{
}

ConfigurationSettings::~ConfigurationSettings()
{
}

ConfigurationSettings& ConfigurationSettings::operator =(const ConfigurationSettings& config)
{
	m_filename = config.m_filename;
	m_sections = config.m_sections;

	return *this;
}

ConfigurationSection& ConfigurationSettings::Section(const String& name)
{
	if (! m_sections.ContainsKey(name))
	{
		m_sections.Set(name, ConfigurationSection(name));
	}
	return m_sections.GetRef(name);
}

void ConfigurationSettings::Load(const String& filename)
{
	m_filename = filename;
	if ( m_filename.EndsWith(".ini") )
	{
		TextReader reader(File::OpenText(filename));
		DelimitedFilePtr ini = DelimitedFile::Parse(reader, '=');
		Load(ini);
		return;
	}
	//else if ( m_filename.EndsWith( ".config" ) || m_filename.EndsWith( ".xml" ) )
	//{
	//	XmlDocumentPtr doc = XmlDocument::Parse(filename);
	//	Load(doc);
	//	return;
	//}
	else
	{
		throw new ConfigurationException("Unknown file type.");
	}
}

/*void ConfigurationSettings::Load(XmlDocument& appconfig)
{
	m_sections.Clear();

	XmlElementPtr config = appconfig.RootElement();
	if ( config.IsNull() || ! config->Name().Equals("configuration") )
	{
		throw new InvalidArgumentException("Invalid file format, expected <configuration>");
	}
	
	XmlNodePtr section = config->FirstChildElement();
	while(section.IsNotNull())
	{
		XmlElementPtr node = section->FirstChildElement();
		while ( node.IsNotNull() )
		{
			if (! node->Name().Equals("add"))
			{
				throw new InvalidArgumentException("Expected <add> tag.");
			}

			XmlAttributePtr keynode = node->Attribute("key");
			XmlAttributePtr valnode = node->Attribute("value");

			if ( keynode.IsNull() || valnode.IsNull() )
			{
				throw new InvalidArgumentException("Malformed <add> tag.");
			}

			StringPtr key = keynode->Value();
			StringPtr val = valnode->Value();

			Section(section->Name()).Set(*key, *val);
			
			node = node->NextSiblingElement();
		}

		section = section->NextSibling();
	}
}
*/

void ConfigurationSettings::Load(const DelimitedFile& inifile)
{
	m_sections.Clear();

	String section;

	for ( int r = 0; r < inifile.RowCount(); r++ )
	{
		DataRowPtr row = inifile.RowAt(r);
		if ( row->Count() == 0 )
		{
			continue;
		}
		StringPtr key = row->Cell(0)->ToString();
		if (key->StartsWith('['))
		{
			//Section name
			section = *key->Mid(1, key->Length() - 1);
			ASSERT(! section.EndsWith(']'));
			Section(section);
			continue;
		}
		if ( row->Count() == 1 )
		{
			Section(section).Set(*key, String(""));
		}
		else if ( row->Count() == 2 )
		{
			Section(section).Set(*key, row->Cell(1)->ToString());
		}
		else
		{
			throw new InvalidArgumentException("Invalid file format, too many columns.");
		}
	}
}

bool ConfigurationSettings::ContainsKey(const String& section, const String& key)
{
	return Section(section).ContainsKey(key);
}

void ConfigurationSettings::Add(const String& section, const String& key, const String& value)
{
	Section(section).Set(key, value);
}

StringPtr ConfigurationSettings::Value(const String& section, const String& key)
{
	if ( ! m_sections.ContainsKey(section) )
	{
		m_sections.Set(section, ConfigurationSection(section));
	}
	return m_sections.Get(section).Value(key);
}

List<StringPtr>& ConfigurationSettings::Values(const String& section, const String& key)
{
	if ( ! m_sections.ContainsKey(section) )
	{
		m_sections.Set(section, ConfigurationSection(section));
	}
	return m_sections.Get(section).Values(key);
}

StringPtr ConfigurationSettings::ToXmlString()
{
	StringBuffer xml("<?xml version=\"1.0\"?>\n<configuration>\n");

	for (Hashtable<String, ConfigurationSection>::Iterator iter(m_sections.Begin()); iter.Next(); )
	{
		xml.Append(iter.CurrentRef().ToXmlString());
	}

	xml.Append("</configuration>\n");
	return xml.ToString();
}

StringPtr ConfigurationSettings::ToIniString()
{
	StringBuffer ini;
	for (Hashtable<String, ConfigurationSection>::Iterator iter(m_sections.Begin()); iter.Next(); )
	{
		ini.Append(iter.CurrentRef().ToIniString());
	}
	return ini.ToString();
}

String ConfigurationSettings::AppConfigName()
{
	return Environment::ProcessName() + ".config";
}

ConfigurationSettingsPtr ConfigurationSettings::LoadAppConfig()
{
	ConfigurationSettingsPtr cfgp(new ConfigurationSettings);
	String processName = Environment::ProcessName();

#ifdef _WINDOWS

	// try the registery
	String path("Software\\SPL\\" + Environment::ProcessName());
	
	if (WinRegKey::KeyExists(WinRegKey::HiveLocalMachine, path))
	{
		WinRegKey key(path, WinRegKey::HiveLocalMachine);
		List<WinRegKey> sections(key.ListSubKeys());
		
		for(List<WinRegKey>::Iterator iter(sections.Begin()); iter.Next(); )
		{
			WinRegKey& secKey = iter.CurrentRef();
			String sectionName(secKey.KeyName().Substring(secKey.KeyName().LastIndexOf("\\")+1));
			List<String> subKeys(secKey.ListSubValueNames());
			
			for(List<String>::Iterator iterSub(subKeys.Begin()); iterSub.Next(); )
			{
				cfgp->Add(sectionName, iterSub.CurrentRef(), secKey.ValueString(iterSub.CurrentRef()));
			}
		}
		return cfgp;
	}

#endif

	if (File::Exists(processName + ".ini"))
	{
		cfgp->Load(processName + ".ini");
	}
	else if (File::Exists(processName + ".xml"))
	{
		cfgp->Load(processName + ".xml");
	}
	else if (File::Exists(processName + ".config"))
	{
		cfgp->Load(processName + ".config");
	}
	else if (Directory::GetWorkingDirectory()->EndsWith("Debug"))
	{
		if (File::Exists("..\\" + processName + ".ini"))
		{
			cfgp->Load("..\\" + processName + ".ini");
		}
		else if (File::Exists("..\\" + processName + ".xml"))
		{
			cfgp->Load("..\\" + processName + ".xml");
		}
		else if (File::Exists("..\\" + processName + ".config"))
		{
			cfgp->Load("..\\" + processName + ".config");
		}
	}
	
	return cfgp;
}

void ConfigurationSettings::SaveToIniFile(const String& filename)
{
	ASSERT(filename.EndsWith(".ini"));
	
	if (File::Exists(filename))
	{
		File::Delete(filename);
	}
	
	TextWriterPtr writer(File::CreateText(filename));
	
	for(Hashtable<String, ConfigurationSection>::Iterator iter(m_sections.Begin()); iter.Next(); )
	{
		writer->Write(iter.CurrentRef().ToIniString());
	}
	
	writer->Close();
}

/*void ConfigurationSettings::SaveToXmlFile(const String& filename)
{
	ASSERT(filename.EndsWith(".xml"));

	if (File::Exists(filename))
	{
		File::Delete(filename);
	}
	
	TextWriterPtr writer(File::CreateText(filename));
	
	for(Hashtable<String, ConfigurationSection>::Iterator iter(m_sections.Begin()); iter.Next(); )
	{
		writer->Write(iter.CurrentRef().ToXmlString());
	}
	
	writer->Close();
}
*/

/** @TODO Untested. */
void ConfigurationSettings::SaveToRegistry()
{
#ifndef _WINDOWS
	throw new Exception("Not supported");
#else
	String path("Software\\SPL\\" + Environment::ProcessName());
	if (!WinRegKey::KeyExists(WinRegKey::HiveLocalMachine, path))
	{
		WinRegKey::Create(WinRegKey::HiveLocalMachine, path);
	}
	
	WinRegKey rootKey(path, WinRegKey::HiveLocalMachine);
	rootKey.OpenForReadWrite();

	List<WinRegKey> rootChilds = rootKey.ListSubKeys();	
	for (List<WinRegKey>::Iterator ii(rootChilds.Begin()); ii.Next(); )
	{
		List<String> existValues = ii.CurrentRef().ListSubValueNames();
		for (List<String>::Iterator eiter(existValues.Begin()); eiter.Next(); )
		{
			ii.CurrentRef().DeleteValue(eiter.CurrentRef());
		}
		rootKey.DeleteSubKey(ii.CurrentRef().KeyName());
	}	
	
	for(Hashtable<String, ConfigurationSection>::Iterator iter(m_sections.Begin()); iter.Next(); )
	{
		// create section key
		String sectionPath = path + "\\" + iter.CurrentKeyRef();
		WinRegKey sectionKey(WinRegKey::Create(WinRegKey::HiveLocalMachine, sectionPath));
		sectionKey.OpenForReadWrite();
		
		for (Hashtable<String, List<StringPtr> >::Iterator viter(iter.CurrentRef().Begin()); viter.Next(); )
		{
			for (List<StringPtr>::Iterator liter(viter.CurrentRef().Begin()); liter.Next(); )
			{
				sectionKey.SetSubKey(viter.CurrentKeyRef(), liter.CurrentRef());
			}
		}
		sectionKey.Close();
	}
		
	rootKey.Close();
#endif
}

#ifdef DEBUG
void ConfigurationSettings::ValidateMem() const
{
	m_sections.ValidateMem();
	m_filename.ValidateMem();
}

void ConfigurationSettings::CheckMem() const
{
	m_sections.CheckMem();
	m_filename.CheckMem();
}
#endif
