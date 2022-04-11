/*
 *  Much of this code is extracted from the tinyxml project, attributed below.
 */
/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

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
#include <spl/io/Stream.h>
#include <spl/text/StringBuffer.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlElement.h>
#include <spl/xml/XmlText.h>

using namespace spl;

void XmlElement::RemoveAttribute( const String& name )
{
	if ( m_attribs.IsNull() )
	{
		return;
	}
	XmlAttributePtr node = m_attribs->Find( name );
	if ( node.IsNotNull() )
	{
		m_attribs->Remove( node );
	}
}

XmlElement::XmlElement (const String& _name)
: XmlNode( XmlNode::ELEMENT ), m_name(_name), m_attribs()
{
}

XmlElement::XmlElement( const XmlElement& copy)
: XmlNode( XmlNode::ELEMENT ), m_name(copy.m_name), m_attribs()
{
	copy.CopyTo( *this );
}

void XmlElement::operator=( const XmlElement& base )
{
	ClearThis();
	base.CopyTo( *this );
	m_name = base.m_name;
}

XmlElement::~XmlElement()
{
	ClearThis();
}

String XmlElement::Name() const
{
	return m_name;
}

XmlElementPtr XmlElement::ChildElement( int count )
{
	int i;
	XmlElementPtr child = FirstChildElement(  );
	for (	i=0;
			child.IsNotNull() && i<count;
			child = child->NextSiblingElement(  ), ++i )
	{
		// nothing
	}

	return child;
}

XmlElementPtr XmlElement::ChildElement( const String& _value, int count )
{
	int i;
	XmlElementPtr child = FirstChildElement( _value );
	for (	i=0;
			child.IsNotNull() && i<count;
			child = child->NextSiblingElement( _value ), ++i )
	{
		// nothing
	}
	return child;
}

void XmlElement::ClearThis()
{
	Clear();
	if ( m_attribs.IsNull() )
	{
		return;
	}
	if( m_attribs.IsNotNull() )
	{
		m_attribs->Clear();
	}
}

void XmlElement::SetAttribute( const String& cname, const String& cvalue )
{
	if ( m_attribs.IsNull() )
	{
		m_attribs = XmlAttributeCollectionPtr(new XmlAttributeCollection());
	}

	XmlAttributePtr node = m_attribs->Find( cname );
	if ( node.IsNotNull() )
	{
		node->SetValue( cvalue );
		return;
	}

	XmlAttributePtr attrib = new XmlAttribute( cname, cvalue );
	if ( attrib.IsNull() )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ENCODING_UNKNOWN], 0, 0 );
	}
	m_attribs->Add( attrib );
}

void XmlElement::WriteTo( TextWriter& writer ) const
{
	writer.WriteFormat( "<%s", m_name.GetChars() );

	if ( m_attribs.IsNotNull() )
	{
		for ( XmlAttributePtr attrib = m_attribs->First(); attrib.IsNotNull(); attrib = (XmlAttributePtr)attrib->NextSibling() )
		{
			attrib.ValidateMem();
			writer.Write( " " );
			attrib->WriteTo( writer );
		}
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	XmlNodePtr node;
	if ( m_firstChild.IsNull() )
	{
		writer.Write( " />" );
	}
	else if ( m_firstChild.Get() == m_lastChild.Get() && m_firstChild->IsText() )
	{
		writer.Write( ">" );
		m_firstChild.ValidateMem();
		m_firstChild->WriteTo( writer );
		writer.WriteFormat( "</%s>", m_name.GetChars() );
	}
	else
	{
		writer.Write( ">" );

		for ( node = m_firstChild; node.IsNotNull(); node = node->NextSibling() )
		{
			m_firstChild.ValidateMem();
			if ( node.Get() != m_firstChild.Get() )
			{
				node->PreviousSibling()->ValidateMem();
				if ( /*NULL == node->ToText()  &&*/ ! node->PreviousSibling()->IsText() )
				{
					writer.Write( "\n" );
				}				
			}
			node->WriteTo( writer );
		}
		writer.WriteFormat( "</%s>", m_name.GetChars() );
	}
}

void XmlElement::CopyTo( XmlElement& target ) const
{
	// superclass:
	XmlNode::CopyTo( target );

	// Element class: 
	// Clone the attributes, then clone the children.
	if ( m_attribs.IsNotNull() )
	{
		for ( int x = 0; x < m_attribs->Count(); x++ )
		{
			XmlAttributePtr attribute = (XmlAttributePtr)m_attribs->Item(x);
			target.SetAttribute( attribute->Name(), *attribute->Value() );
		}
	}

	if (m_firstChild.IsNotNull())
	{
		XmlNodePtr node;
		for ( node = m_firstChild; node.IsNotNull(); node = node->NextSibling() )
		{
			target.AppendChild( node->Clone() );
		}
	}
}

XmlNodePtr XmlElement::Clone() const
{
	XmlElementPtr clone = XmlElementPtr(new XmlElement( Name() ));
	XmlNodePtr xclone = (XmlNodePtr)clone;
	clone->m_self = xclone;
	CopyTo( *clone );
	return clone;
}

/*String XmlElement::GetText() const
{
	const XmlNode* child = this->FirstChild();
	if ( child ) 
	{
		const XmlText* childText = child->ToText();
		if ( childText ) 
		{
			return childText->Value();
		}
	}
	return String();
}*/

StringPtr XmlElement::InnerText() const
{
	StringBuffer buf;
	XmlNodePtr child = FirstChild();
	while ( child.IsNotNull() ) 
	{
		buf.Append( child->InnerText() );
		child = child->NextSibling();
	}
	return buf.ToString();
}

StringPtr XmlElement::InnerXml() const
{
	if ( m_firstChild.IsNull() )
	{
		return StringPtr(new String());
	}

	MemoryStreamPtr ms(new MemoryStream());
	TextWriter writer(ms);
	XmlNodePtr node;

	if ( m_firstChild.Get() == m_lastChild.Get() && m_firstChild->IsText() )
	{
		m_firstChild.ValidateMem();
		m_firstChild->WriteTo( writer );
	}
	else
	{
		for ( node = m_firstChild; node.IsNotNull(); node = node->NextSibling() )
		{
			m_firstChild.ValidateMem();
			if ( node.Get() != m_firstChild.Get() )
			{
				node->PreviousSibling()->ValidateMem();
				if ( /*NULL == node->ToText()  &&*/ ! node->PreviousSibling()->IsText() )
				{
					writer.Write( "\n" );
				}				
			}
			node->WriteTo( writer );
		}
	}
	return ms->ToString();	
}

StringPtr XmlElement::ToString() const
{
	MemoryStreamPtr ms(new MemoryStream());
	TextWriter writer(ms);
	WriteTo(writer);
	return ms->ToString();	
}

XmlElementPtr XmlElement::ToElement() const 
{ 
	return (XmlNodePtr)m_self; 
} 

XmlElementPtr XmlElement::CreateElement(const String& name)
{
	XmlElementPtr element(new XmlElement(name));
	XmlNodePtr xe = (XmlNodePtr)element;
	element->m_self = xe;
	return element;
}

#ifdef DEBUG
void XmlElement::ValidateMem() const
{
	XmlNode::ValidateMem();
	m_name.ValidateMem();
	m_attribs.ValidateMem();
}

void XmlElement::CheckMem() const
{
	XmlNode::CheckMem();
	m_name.CheckMem();
	m_attribs.CheckMem();
}
#endif
