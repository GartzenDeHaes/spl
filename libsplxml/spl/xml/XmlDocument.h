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
#ifndef _xmldocument_h
#define _xmldocument_h

#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/io/FileStream.h>
#include <spl/RefCountPtr.h>
#include <spl/io/StringStream.h>
#include <spl/io/TextStream.h>
#include <spl/xml/XmlNode.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class XmlDocument : public XmlNode
{
protected:	
	static bool m_condenseWhiteSpace;

	static int m_tabsize;
	
	bool m_useMicrosoftBOM;		///< the UTF-8 BOM were found when read. Note this, and try to write.public:

	/** Parse the given null terminated block of xml data. Passing in an encoding to this
		method (either TIXML_ENCODING_LEGACY or TIXML_ENCODING_UTF8 will force TinyXml
		to use that encoding, regardless of what TinyXml might otherwise try to detect.
	*/
	virtual const char *_Parse( const char *p, XmlParsingData* data = NULL, XmlEncoding encoding = TIXML_DEFAULT_ENCODING );
	
	void CopyTo( XmlDocument& target ) const;
	virtual void WriteTo(TextWriter& writer) const;

public:
	
	/// Create an empty document, that has no name.
	XmlDocument();
	XmlDocument( const XmlDocument& copy );
	virtual ~XmlDocument();
	
	void operator =( const XmlDocument& copy );

	virtual String Name() const;

	/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	void Write( TextWriter& xmlout ) const;

	inline void Write( const String& filename ) const
	{
		TextWriter tw(FileStreamPtr(new FileStream(filename, File::FILEMODE_Truncate, File::FILEACC_Write)));
		Write(tw);
	}

	static XmlDocumentPtr Parse( TextReader& xmlin, XmlParsingData* data = NULL, XmlEncoding encoding = TIXML_DEFAULT_ENCODING );

	inline static XmlDocumentPtr Parse( const String& filename )
	{
		TextReader tw(File::OpenRead(filename));
		return Parse(tw);
	}
	
	inline static XmlDocumentPtr ParseXml( const String& xml, XmlParsingData* data = NULL, XmlEncoding encoding = TIXML_DEFAULT_ENCODING )
	{
		TextReader tw(StringStreamPtr(new StringStream(xml)));
		return Parse(tw, data, encoding);
	}

	/** Get the root element -- the only top level element -- of the document.
		In well formed XML, there should only be one. TinyXml is tolerant of
		multiple elements at the document level.
	*/
	XmlElementPtr RootElement();

	/** SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol())
		to report the correct values for row and column. It does not change the output
		or input in any way.
		
		By calling this method, with a tab size
		greater than 0, the row and column of each node and attribute is stored
		when the file is loaded. Very useful for tracking the DOM back in to
		the source file.

		The tab size is required for calculating the location of nodes. If not
		set, the default of 4 is used. The tabsize is set per document. Setting
		the tabsize to 0 disables row/column tracking.

		Note that row and column tracking is not supported when using operator>>.

		The tab size needs to be enabled before the parse or load. Correct usage:
		@verbatim
		TiXmlDocument doc;
		doc.SetTabSize( 8 );
		doc.Load( "myfile.xml" );
		@endverbatim

		@sa Row, Column
	*/
	inline static void SetTabSize( int _tabsize ) { m_tabsize = _tabsize; }
	inline static int TabSize()	{ return m_tabsize; }
	
	inline static void SetCondenseWhiteSpace(bool condense) { m_condenseWhiteSpace = condense; }
	inline static bool CondenseWhiteSpace() { return m_condenseWhiteSpace; }
	static inline bool IsWhiteSpaceCondensed() { return m_condenseWhiteSpace; }

	virtual XmlDocumentPtr    ToDocument()    const; ///< Cast to a more defined type. Will return null not of the requested type.

	virtual StringPtr InnerXml() const;
	virtual StringPtr ToString() const;
						
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

	// [internal use]
	virtual XmlNodePtr Clone() const;

};

REGISTER_TYPEOF( 536, XmlDocument );

/** @} */
}
#endif
