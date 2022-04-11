#include <spl/configuration/ConfigurationSection.h>
#include <spl/Environment.h>
#include <spl/text/StringBuffer.h>
//#include <spl/xml/XmlText.h>

using namespace spl;

ConfigurationSection::ConfigurationSection()
: m_name(), m_idx()
{
}

ConfigurationSection::ConfigurationSection(const String& name)
: m_name(name), m_idx()
{
}

ConfigurationSection::ConfigurationSection(const ConfigurationSection& cs)
: m_name(cs.m_name), m_idx(cs.m_idx)
{
}

ConfigurationSection::~ConfigurationSection()
{
}

ConfigurationSection& ConfigurationSection::operator =(const ConfigurationSection& cs)
{
	m_name = cs.m_name;
	m_idx.Clear();

	for(Hashtable<String, List<StringPtr> >::Iterator iter = cs.m_idx.Begin(); iter.Next(); )
	{
		m_idx.Set(iter.CurrentKeyRef(), List<StringPtr>());

		for (List<StringPtr>::Iterator vals(iter.CurrentRef().Begin()); vals.Next(); )
		{
			m_idx.GetRef(iter.CurrentKeyRef()).Add(vals.CurrentRef()->Clone());
		}
	}

	return *this;
}

Array<String> ConfigurationSection::Keys()
{
	int pos = 0;
	Array<String> keys(m_idx.Count());

	for(Hashtable<String, List<StringPtr> >::Iterator iter = m_idx.Begin(); iter.Next(); )
	{
		keys[pos] = iter.CurrentKey();
	}

	return keys;
}

void ConfigurationSection::Set(const String& key, const String& value)
{
	if (! m_idx.ContainsKey(key))
	{
		m_idx.Set(key, List<StringPtr>());
	}
	m_idx.GetRef(key).Add(value.Clone());
}

void ConfigurationSection::Remove(const String& key)
{
	m_idx.Remove(key);
}

StringPtr ConfigurationSection::Value(const String& key)
{
	if (! m_idx.ContainsKey(key))
	{
		return StringPtr();
	}

	List<StringPtr>& list = m_idx.GetRef(key);
	if (list.Count() == 0)
	{
		return StringPtr();
	}
	if (list.Count() == 1)
	{
		return list.Peek();
	}

	StringBuffer val;
	for (List<StringPtr>::Iterator vals(list.Begin()); vals.Next(); )
	{
		if (val.Length() > 0)
		{
			val.Append(',');
		}
		val.Append(vals.Current());
	}

	return val.ToString();
}

List<StringPtr>& ConfigurationSection::Values(const String& key)
{
	if (! m_idx.ContainsKey(key))
	{
		m_idx.Set(key, List<StringPtr>());
	}
	return m_idx.GetRef(key);
}

StringPtr ConfigurationSection::ToIniString()
{
	StringBuffer ini;
	String nl(Environment::NewLine());

	ini.Append('[');
	ini.Append(m_name);
	ini.Append(']');
	ini.Append(nl);

	for(Hashtable<String, List<StringPtr> >::Iterator iter = m_idx.Begin(); iter.Next(); )
	{
		for (List<StringPtr>::Iterator vals(iter.CurrentRef().Begin()); vals.Next(); )
		{
			ini.Append(iter.CurrentKeyRef());
			ini.Append('=');
			ini.Append(vals.Current());
			ini.Append(nl);
		}
	}
	ini.Append(nl);

	return ini.ToString();
}

StringPtr ConfigurationSection::ToXmlString()
{
	StringBuffer xml;

/*	xml.Append('<');
	XmlText::XmlEncode(m_name, xml);
	xml.Append(">\n");

	for(Hashtable<String, List<StringPtr> >::Iterator iter = m_idx.Begin(); iter.Next(); )
	{
		for (List<StringPtr>::Iterator vals(iter.CurrentRef().Begin()); vals.Next(); )
		{
			xml.Append("<add key=\"");
			XmlText::XmlEncode(iter.CurrentKeyRef(), xml);
			xml.Append("\" value=\"");
			XmlText::XmlEncode(vals.Current(), xml);
			xml.Append("\" />\n");
		}
	}

	xml.Append("</");
	XmlText::XmlEncode(m_name, xml);
	xml.Append('>');
	xml.Append('\n');
*/
	return xml.ToString();
}

#ifdef DEBUG
void ConfigurationSection::ValidateMem() const
{
	m_name.ValidateMem();
	m_idx.ValidateMem();
}

void ConfigurationSection::CheckMem() const
{
	m_name.CheckMem();
	m_idx.CheckMem();
}
#endif

