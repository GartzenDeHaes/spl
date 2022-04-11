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
#ifndef _xmldeclaration_h
#define _xmldeclaration_h

#include <spl/Debug.h>
#include <spl/xml/XmlNode.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

/** @brief Declaration section.
 *
 */
class XmlDeclaration : public XmlNode
{
protected:
	void CopyTo( XmlDeclaration& target ) const;

	String version;
	String encoding;
	String standalone;	
	
public:
	/// Construct an empty declaration.
	inline XmlDeclaration()   : XmlNode( XmlNode::DECLARATION ) {}

	/// Construct.
	XmlDeclaration(	const String& _version,
						const String& _encoding,
						const String& _standalone );

	virtual ~XmlDeclaration();

	XmlDeclaration( const XmlDeclaration& copy );
	void operator=( const XmlDeclaration& copy );

	virtual String Name() const;
			
	/// Version. Will return an empty string if none was found.
	inline const String& Version() const			{ return version; }
	/// Encoding. Will return an empty string if none was found.
	inline const String& Encoding() const		{ return encoding; }
	/// Is this a standalone document?
	inline const String &Standalone() const		{ return standalone; }

	/// Creates a copy of this Declaration and returns it.
	virtual XmlNodePtr Clone() const;

	virtual XmlDeclarationPtr ToDeclaration() const; ///< Cast to a more defined type. Will return null not of the requested type.

	virtual StringPtr ToString() const;
	virtual void WriteTo(TextWriter& writer) const; 
			
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

	virtual const char* _Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );
};

REGISTER_TYPEOF( 534, XmlDeclaration );

/** @} */
}
#endif
