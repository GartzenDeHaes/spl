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
#include <spl/io/TextStream.h>
#include <spl/xml/XmlDocument.h>
#include <spl/xml/XmlElement.h>
#include <spl/xml/XmlIterator.h>

using namespace spl;

int XmlDocument::m_tabsize;
bool XmlDocument::m_condenseWhiteSpace = true;

XmlDocument::XmlDocument() 
:	XmlNode( XmlNode::DOCUMENT ), 
	m_useMicrosoftBOM(false)
{
	m_tabsize = 4;
	m_useMicrosoftBOM = false;
}

XmlDocument::XmlDocument( const XmlDocument& copy ) 
:	XmlNode( XmlNode::DOCUMENT ),
	m_useMicrosoftBOM(false)
{
	copy.CopyTo( *this );
}

XmlDocument::~XmlDocument() 
{
}

void XmlDocument::operator=( const XmlDocument& copy )
{
	Clear();
	copy.CopyTo( *this );
}

XmlElementPtr XmlDocument::RootElement()
{ 
	return FirstChildElement(); 
}

String XmlDocument::Name() const
{
	return String("#document");
}

/// Save a file using the given filename. Returns true if successful.
void XmlDocument::Write( TextWriter& xmlout ) const
{
	if ( m_useMicrosoftBOM ) 
	{
		const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

		xmlout.Write( (byte)TIXML_UTF_LEAD_0 );
		xmlout.Write( (byte)TIXML_UTF_LEAD_1 );
		xmlout.Write( (byte)TIXML_UTF_LEAD_2 );
	}
	WriteTo( xmlout );
}

/// Load a file using the given filename. Returns true if successful.
XmlDocumentPtr XmlDocument::Parse( TextReader& xmlin, XmlParsingData* args, XmlEncoding encoding )
{
	if ( !xmlin.CanRead() ) 
	{
		throw new XmlException("Stream is not readable", -1, -1);
	}

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = xmlin.Length();

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		throw new XmlException( "Stream is empty", -1, -1 );
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	StringBuffer data( length+1 );
		
	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	const int buflen = 256;
	Array<byte> buf(buflen);
	int readlen;
	
	while ( (readlen = xmlin.ReadBlock(buf, 0, buflen)) > 0 )
	{
		int crpos = spl::IndexofchfromWithLen((const char *)buf.Data(), 0xD, 0, readlen);
		if ( crpos < 0 )
		{
			data.Append(buf, readlen);
			continue;
		}
		for ( int x = 0; x < readlen; x++ )
		{
			char ch = buf[x];
			if ( ch != 0xD )
			{
				data.Append(ch);
			}
			else if ( x < readlen )
			{
				if ( x == readlen-1 || buf[x+1] != 0xA )
				{
					data.Append((char)0xA);
				}
			}
		}
	}

	XmlDocumentPtr doc(new XmlDocument());
	XmlNodePtr xdoc = (XmlNodePtr)doc;
	doc->m_self = xdoc;

	doc->_Parse( data.GetChars(), args, encoding );
	
	return doc;
}

void XmlDocument::CopyTo( XmlDocument& target ) const
{
	XmlNode::CopyTo( target );

	target.m_tabsize = m_tabsize;
	target.m_useMicrosoftBOM = m_useMicrosoftBOM;

	if (m_firstChild.IsNotNull())
	{
		XmlNodePtr node;
		for ( node = m_firstChild; node.IsNotNull(); node = node->NextSibling() )
		{
			node.ValidateMem();
			target.AppendChild( node->Clone() );
		}
	}
}

XmlNodePtr XmlDocument::Clone() const
{
	XmlDocumentPtr clone = XmlDocumentPtr(new XmlDocument());
	XmlNodePtr xclone = (XmlNodePtr)clone;
	clone->m_self = xclone;
	CopyTo( *clone );
	return clone;
}

void XmlDocument::WriteTo( TextWriter& writer ) const
{
	for ( XmlNodePtr node = FirstChild(); node.IsNotNull(); node = node->NextSibling() )
	{
		node->WriteTo( writer );
	}
}

StringPtr XmlDocument::InnerXml() const
{
	return ToString();
}

StringPtr XmlDocument::ToString() const
{
	MemoryStreamPtr ms(new MemoryStream());
	TextWriter writer(ms);
	WriteTo(writer);
	return ms->ToString();		
}

XmlDocumentPtr  XmlDocument::ToDocument()    const
{
	return (XmlNodePtr)m_self;
}

#ifdef DEBUG
void XmlDocument::ValidateMem() const
{
	XmlNode::ValidateMem();

	XmlIterator iter(ToDocument());
	while ( iter.Next() )
	{
		iter.CurrentRef().ValidateMem();
	}
}

void XmlDocument::CheckMem() const
{
	XmlNode::CheckMem();

	XmlIterator iter(ToDocument());
	while ( iter.Next() )
	{
		iter.CurrentRef().CheckMem();
	}
}
#endif
