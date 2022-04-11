/* Modified for the SPL project. */
/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to anyone to use this software for any 
purpose, including commercial applications, and to alter it and 
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must 
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and 
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source 
distribution.
*/
#include <spl/text/StringBuffer.h>
#include <spl/xml/XmlAttribute.h>
#include <spl/xml/XmlAttributeCollection.h>
#include <spl/xml/XmlComment.h>
#include <spl/xml/XmlDeclaration.h>
#include <spl/xml/XmlNamedNodeMap.h>
#include <spl/xml/XmlText.h>
#include <spl/xml/XmlUnknown.h>
#include <spl/xml/XmlParsingData.h>

using namespace spl;

XmlAttribute::XmlAttribute() 
: XmlNode(XmlNode::ATTRIBUTE), m_document()
{
}

/// Construct an attribute with a name and value.
XmlAttribute::XmlAttribute( const String& _name, const String& _value )
: XmlNode(XmlNode::ATTRIBUTE), m_name(_name), m_document()
{
	m_value = _value;
}

XmlAttribute::~XmlAttribute()
{
}


String XmlAttribute::Name() const
{ 
	return m_name;
}

XmlNodePtr XmlAttribute::Clone() const
{
	XmlAttributePtr clone(new XmlAttribute());
	XmlNodePtr xclone = (XmlNodePtr)clone;
	clone->m_self = xclone;
	CopyTo( *clone );
	return clone;
}

void XmlAttribute::WriteTo( TextWriter& writer ) const
{
	writer.Write( *ToString() );
}

StringPtr XmlAttribute::ToString() const
{
	StringBuffer n, v;

	_XmlEncodeString( m_name, n );
	_XmlEncodeString( *m_value.ToString(), v );

	if (m_value.IndexOf('\"') < 0) 
	{
		return String::Format( "%s=\"%s\"", n.GetChars(), v.GetChars() );
	}
	else 
	{
		return String::Format( "%s='%s'", n.GetChars(), v.GetChars() );
	}	
}

#ifdef DEBUG
void XmlAttribute::ValidateMem() const
{
	XmlNode::ValidateMem();
	m_name.ValidateMem();
}

void XmlAttribute::CheckMem() const
{
	XmlNode::CheckMem();
	m_name.CheckMem();
}
#endif

XmlComment::XmlComment( const XmlComment& copy ) : XmlNode( XmlNode::COMMENT )
{
	copy.CopyTo( *this );
}

XmlComment::XmlComment( const String& _value ) 
: XmlNode( XmlNode::COMMENT ) 
{
	SetValue( _value );
}

XmlComment::XmlComment() 
: XmlNode( XmlNode::COMMENT ) 
{
}

XmlComment::~XmlComment()	
{
}

String XmlComment::Name() const
{
	return String("#comment");
}

void XmlComment::operator=( const XmlComment& base )
{
	Clear();
	base.CopyTo( *this );
}

void XmlComment::CopyTo( XmlComment& target ) const
{
	XmlNode::CopyTo( target );
}

XmlNodePtr XmlComment::Clone() const
{
	XmlCommentPtr clone(new XmlComment());
	XmlNodePtr xclone = (XmlNodePtr)clone;
	clone->m_self = xclone;
	CopyTo( *clone );
	return clone;
}

void XmlComment::WriteTo( TextWriter& writer ) const
{
	writer.Write( *ToString() );
}

StringPtr XmlComment::ToString() const
{
	return String::Format( "<!--%s-->", m_value.GetChars() );
}

XmlCommentPtr XmlComment::ToComment() const
{ 
	return (XmlNodePtr)m_self; 
}

XmlText::XmlText (const String& initValue ) 
: XmlNode (XmlNode::TEXT)
{
	SetValue( initValue );
	m_cdata = false;
}

XmlText::XmlText( const XmlText& copy ) 
: XmlNode( XmlNode::TEXT )	
{ 
	copy.CopyTo( *this ); 
}

XmlText::~XmlText() 
{
}

String XmlText::Name() const
{
	if ( m_cdata )
	{
		return String("#cdata-section");
	}
	else
	{
		return String("#text");
	}
}

void XmlText::WriteTo( TextWriter& writer ) const
{
	if ( m_cdata )
	{
		writer.WriteFormat( "<![CDATA[%s]]>", m_value.GetChars() );	// unformatted output
	}
	else
	{
		StringBuffer buffer;
		_XmlEncodeString( *m_value.ToString(), buffer );
		writer.WriteFormat( "%s", buffer.GetChars() );
	}
}

StringPtr XmlText::ToString() const
{
	if ( m_cdata )
	{
		return String::Format( "<![CDATA[%s]]>", m_value.GetChars() );	// unformatted output
	}
	else
	{
		StringBuffer buffer;
		_XmlEncodeString( *m_value.ToString(), buffer );
		return buffer.ToString();
	}
}

void XmlText::CopyTo( XmlText& target ) const
{
	XmlNode::CopyTo( target );
	target.m_cdata = m_cdata;
}

XmlNodePtr XmlText::Clone() const
{	
	XmlTextPtr clone(new XmlText( "" ));
	XmlNodePtr xclone = (XmlNodePtr)clone;
	clone->m_self = xclone;
	CopyTo( *clone );
	return clone;
}

XmlTextPtr XmlText::ToText() const
{ 
	return (XmlNodePtr)m_self; 
} 

XmlTextPtr XmlText::CreateText(const String& val)
{
	XmlTextPtr text(new XmlText(val));
	XmlNodePtr xtext = (XmlNodePtr)text;
	text->m_self = xtext;
	return text;
}

XmlDeclaration::XmlDeclaration( const String& _version,
									const String& _encoding,
									const String& _standalone )
: 	XmlNode( XmlNode::DECLARATION ),
	version(_version),
	encoding(_encoding),
	standalone(_standalone)
{
}

XmlDeclaration::XmlDeclaration( const XmlDeclaration& copy )
: XmlNode( XmlNode::DECLARATION ), version(), encoding(), standalone()
{
	copy.CopyTo( *this );	
}

XmlDeclaration::~XmlDeclaration()	
{
}

void XmlDeclaration::operator =( const XmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( *this );
}

String XmlDeclaration::Name() const
{
	return String("#xml-declaration");
}

void XmlDeclaration::CopyTo( XmlDeclaration& target ) const
{
	XmlNode::CopyTo( target );

	target.version = version;
	target.encoding = encoding;
	target.standalone = standalone;
}

XmlNodePtr XmlDeclaration::Clone() const
{	
	XmlDeclarationPtr clone = new XmlDeclaration();
	XmlNodePtr xclone = (XmlNodePtr)clone;
	clone->m_self = xclone;
	CopyTo( *clone );
	return clone;
}

void XmlDeclaration::WriteTo( TextWriter& writer ) const
{
	writer.Write( "<?xml " );

	if ( version.Length() > 0 ) 
	{
		writer.WriteFormat("version=\"%s\" ", version.GetChars());
	}
	if ( encoding.Length() > 0 ) 
	{
		writer.WriteFormat("encoding=\"%s\" ", encoding.GetChars());
	}
	if ( standalone.Length() > 0 ) 
	{
		writer.WriteFormat("standalone=\"%s\" ", standalone.GetChars());
	}
	writer.Write( "?>" );
}

StringPtr XmlDeclaration::ToString() const
{
	StringBuffer buf;
	buf.Append( "<?xml " );

	if ( version.Length() > 0 ) 
	{
		buf.Append("version=\"");
		buf.Append(version.GetChars());
		buf.Append("\" "); 
	}
	if ( encoding.Length() > 0 ) 
	{
		buf.Append("encoding=\"");
		buf.Append(encoding.GetChars());
		buf.Append("\" "); 
	}
	if ( standalone.Length() > 0 ) 
	{
		buf.Append("standalone=\"");
		buf.Append(standalone.GetChars());
		buf.Append("\" "); 
	}
	buf.Append( "?>" );
	return buf.ToString();
}

XmlDeclarationPtr XmlDeclaration::ToDeclaration() const
{ 
	return (XmlNodePtr)m_self; 
}

#ifdef DEBUG
void XmlDeclaration::ValidateMem() const
{
	XmlNode::ValidateMem();
	version.ValidateMem();
	encoding.ValidateMem();
	standalone.ValidateMem();
}

void XmlDeclaration::CheckMem() const
{
	XmlNode::CheckMem();
	version.CheckMem();
	encoding.CheckMem();
	standalone.CheckMem();
}
#endif

String XmlUnknown::Name() const
{
	return String("#text");
}

void XmlUnknown::WriteTo( TextWriter& writer ) const
{
	writer.WriteFormat( "<%s>", m_value.GetChars() );
}

StringPtr XmlUnknown::ToString() const
{
	return String::Format( "<%s>", m_value.GetChars() );	
}

void XmlUnknown::CopyTo( XmlUnknown& target ) const
{
	XmlNode::CopyTo( target );
}

XmlNodePtr XmlUnknown::Clone() const
{
	XmlUnknownPtr clone(new XmlUnknown());
	XmlNodePtr xclone = (XmlNodePtr)clone;
	clone->m_self = xclone;
	CopyTo( *clone );
	return clone;
}

XmlUnknownPtr XmlUnknown::CreateUnknown()
{
	XmlUnknownPtr ukn(new XmlUnknown());
	XmlNodePtr xukn = (XmlNodePtr)ukn;
	ukn->m_self = xukn;
	return ukn;
}

XmlNodeList::XmlNodeList()
: m_nodes()
{
}

XmlNodeList::~XmlNodeList()
{
}

void XmlNodeList::Clear()
{
	m_nodes.Clear();
}

#ifdef DEBUG
void XmlNodeList::ValidateMem() const
{
	m_nodes.ValidateMem();
}

void XmlNodeList::CheckMem() const
{
	m_nodes.CheckMem();
}
#endif


XmlNamedNodeMap::XmlNamedNodeMap()
: m_map()
{
}

XmlNamedNodeMap::~XmlNamedNodeMap()
{
}

XmlNodePtr XmlNamedNodeMap::RemoveNamedItem(const String& name)
{
	XmlNodePtr node;
	int count = m_nodes.Count();
	for ( int x = 0; x < count; x++ )
	{
		node = m_nodes.ElementAt(x);
		if ( node->Name().Equals(name) )
		{
			m_nodes.RemoveAt(x);
			break;
		}
		else
		{
			node = XmlNodePtr();
		}
	}
	m_map.Remove(name);
	return node;
}

void XmlNamedNodeMap::SetNamedItem( XmlNodePtr node )
{
	node.ValidateMem();
	if ( m_map.ContainsKey( node->Name() ) )
	{
		throw new InvalidArgumentException("A node with that name already exists in the collection");
	}
	m_map.Set( node->Name(), node );
	m_nodes.Add( node );
}

#ifdef DEBUG
void XmlNamedNodeMap::ValidateMem() const
{
	m_map.ValidateMem();
	XmlNodeList::ValidateMem();
}

void XmlNamedNodeMap::CheckMem() const
{
	m_map.CheckMem();
	XmlNodeList::CheckMem();
}
#endif

XmlAttributeCollection::XmlAttributeCollection()
{
}

XmlAttributeCollection::~XmlAttributeCollection()
{
}

XmlAttributePtr XmlAttributeCollection::Find( const String& name ) const
{
	XmlAttributePtr attrib = (XmlAttributePtr)GetNamedItem(name);
	return attrib;
}

void XmlAttributeCollection::Add(XmlAttributePtr attrib)
{
	attrib.ValidateMem();
	SetNamedItem(attrib);
}

XmlAttributePtr XmlAttributeCollection::First() const
{
	if ( Count() == 0 )
	{
		return XmlAttributePtr();
	}
	return (XmlAttributePtr)Item(0);
}

void XmlAttributeCollection::Remove( XmlAttributePtr attrib )
{
	attrib.ValidateMem();
	RemoveNamedItem(attrib->Name());
}
		
StringPtr XmlAttributeCollection::ToString() const
{
	StringBuffer buf;
	int count = Count();
	
	for ( int x = 0; x < count; x++ )
	{
		XmlAttributePtr attrib = (XmlAttributePtr)m_nodes.ElementAt(x);
		if ( x != 0 )
		{
			buf.Append(' ');
		}
		buf.Append( attrib->ToString() );
	}
	return buf.ToString();
}

void XmlAttributeCollection::Write( TextWriter& writer ) const
{
	int count = Count();
	
	for ( int x = 0; x < count; x++ )
	{
		XmlAttributePtr attrib = (XmlAttributePtr)m_nodes.ElementAt(x);
		attrib.ValidateMem();
		if ( x != 0 )
		{
			writer.Write( " " );
		}
		attrib->WriteTo( writer );
	}
}

#ifdef DEBUG
void XmlAttributeCollection::ValidateMem() const
{
	XmlNamedNodeMap::ValidateMem();
}

void XmlAttributeCollection::CheckMem() const
{
	XmlNamedNodeMap::CheckMem();
}
#endif

