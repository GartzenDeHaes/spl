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
#ifndef _xmlelement_h
#define _xmlelement_h

#include <spl/Debug.h>
#include <spl/Double.h>
#include <spl/Int32.h>
#include <spl/xml/XmlAttributeCollection.h>
#include <spl/xml/XmlNode.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

/** @brief Interior node.
 *
 */
class XmlElement : public XmlNode
{
protected:
	friend class XmlNode;
	friend class XmlText;
	friend class XmlAttribute;

	String m_name;
	XmlAttributeCollectionPtr m_attribs;
	
	const char* ReadValue( const char* in, XmlParsingData* prevData, XmlEncoding encoding );
	void ClearThis();	// like clear, but initializes 'this' object as well
	void CopyTo( XmlElement& target ) const;

	/// Construct an element.
	XmlElement (const String& in_value);

public:
	XmlElement( const XmlElement& );
	virtual ~XmlElement();

	virtual String Name() const;
			
	void operator =( const XmlElement& base );
	
	XmlElementPtr ChildElement( const String& value, int index );
	XmlElementPtr ChildElement( int index );

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
	*/
	//const XmlAttribute* Attribute( const String *name ) const;
	inline XmlAttributePtr Attribute( const String& name ) const
	{
		if ( m_attribs.IsNull() )
		{
			return XmlAttributePtr();
		}
		return m_attribs->Find( name );
	}

	inline XmlAttributePtr Attribute( int index ) const
	{
		if ( m_attribs.IsNull() )
		{
			return XmlAttributePtr();
		}
		return (XmlAttributePtr)m_attribs->Item(index);
	}

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const String& name, const String& _value );

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	inline void SetAttribute( const String& name, int val )
	{	
		SetAttribute( name, *Int32::ToString(val) );
	}
	
	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	inline void SetAttribute( const String& name, double value )
	{
		SetAttribute( name, *Double::ToString(value) );
	}

	/** Deletes an attribute with the given name.
	*/
	void RemoveAttribute( const String& name );

	inline XmlAttributeCollectionPtr Attributes() { return m_attribs; } 

	/** Convenience function for easy access to the text inside an element. Although easy
		and concise, GetText() is limited compared to getting the XmlText child
		and accessing it directly.
	
		If the first child of 'this' is a TiXmlText, the GetText()
		returns the character string of the Text node, else null is returned.

		This is a convenient method for getting the text of simple contained text:
		@verbatim
		<foo>This is text</foo>
		const char* str = fooElement->GetText();
		@endverbatim

		'str' will be a pointer to "This is text". 
		
		Note that this function can be misleading. If the element foo was created from
		this XML:
		@verbatim
		<foo><b>This is text</b></foo> 
		@endverbatim

		then the value of str would be null. The first child node isn't a text node, it is
		another element. From this XML:
		@verbatim
		<foo>This is <b>text</b></foo> 
		@endverbatim
		GetText() will return "This is ".

		WARNING: GetText() accesses a child node - don't become confused with the 
				 similarly named TiXmlHandle::Text() and TiXmlNode::ToText() which are 
				 safe type casts on the referenced node.
	*/
	/*String GetText() const;*/
	virtual StringPtr InnerText() const;

	/// Creates a new Element and returns it - the returned element is a copy.
	virtual XmlNodePtr Clone() const;

	virtual XmlElementPtr     ToElement()     const; ///< Cast to a more defined type. Will return null not of the requested type.
	
	virtual StringPtr InnerXml() const;
	virtual StringPtr ToString() const;
	virtual void WriteTo(TextWriter& writer) const;

	static XmlElementPtr CreateElement(const String& name);
			
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

	/*	Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* _Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

};

REGISTER_TYPEOF( 538, XmlElement );

/** @} */
}
#endif
