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
#ifndef _xmlattribute_h
#define _xmlattribute_h

#include <spl/Double.h>
#include <spl/Int32.h>
#include <spl/String.h>
#include <spl/xml/XmlDocument.h>
#include <spl/xml/XmlNode.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

/** @brief XML attribute key/value.
 */
class XmlAttribute : public XmlNode
{
protected:
friend class XmlElement;
friend class XmlDeclaration;

	XmlDocumentRef	m_document;	// A pointer back to a document, for error reporting.
	String m_name;
			
	/*	Attribute parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/
	virtual const char* _Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

	/// Construct an empty attribute.
	XmlAttribute();

	/// Construct an attribute with a name and value.
	XmlAttribute( const String& _name, const String& _value );

public:
	virtual ~XmlAttribute();
			
	virtual XmlNodePtr Clone() const;
			
	/// Return the name of this attribute.
	virtual String Name() const;
	
	void SetName( const String& _name )	{ m_name = _name; }				///< Set the name of this attribute.
	void SetValue( const String& _value )	{ m_value = _value; }				///< Set the value.
	void SetValue( int _value ) { m_value = Int32::ToString(_value); }					///< Set the value from an integer.
	void SetValue( double _value ) { m_value = Double::ToString(_value); }				///< Set the value from a double.

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	inline void SetDocument( XmlDocumentRef doc )
	{ 
		m_document = doc; 
	}
	
	virtual StringPtr ToString() const;
	virtual void WriteTo(TextWriter& writer) const;
			
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 526, XmlAttribute );

/** @} */
}
#endif
