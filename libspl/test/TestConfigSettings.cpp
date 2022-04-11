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
#include <spl/Debug.h>

#ifdef DEBUG
#include <spl/configuration/ConfigurationSettings.h>
#include <spl/io/log/Log.h>
//#include <spl/xml/XmlElement.h>

using namespace spl;

static const char *_xmlConfigTestStr = "<?xml version=\"1.0\"?>\n<configuration><appSettings><add key=\"KEY1\" value=\"VALUE1\" /></appSettings></configuration>";

static void _TestConfigSettings1()
{
/*	ConfigurationSettings *cs = new ConfigurationSettings();

	XmlDocumentPtr doc = XmlDocument::ParseXml(_xmlConfigTestStr);
	cs->Load(doc);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(cs);
	cs->CheckMem();
	doc.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("ConfigurationSettings 1.0");

	UNIT_ASSERT("Lookup", *cs->Value("appSettings", "KEY1") == "VALUE1");

	delete cs;
	doc.ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	doc.CheckMem();
*/
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("ConfigurationSettings 1.2");
	Log::SWriteOkFail( "ConfigurationSettings 1" );
}

static void _TestConfigSettingsToString()
{
/*	ConfigurationSettings cs;

	XmlDocumentPtr doc = XmlDocument::ParseXml(_xmlConfigTestStr);
	cs.Load(doc);

	StringPtr xml(cs.ToXmlString());

	doc = XmlDocument::ParseXml(xml);
	XmlNodePtr as(doc->SelectSingleNode("//appSettings"));
	UNIT_ASSERT("appSettings node", as.IsNotNull());
	UNIT_ASSERT("appSettings node", as->FirstChild()->ToElement()->Name().Equals("add"));
*/
	Log::SWriteOkFail( "ConfigurationSettings ToXmlString" );
}

void _TestConfigSettings()
{
	_TestConfigSettings1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestConfigSettingsToString();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
