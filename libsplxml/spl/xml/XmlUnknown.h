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
#ifndef _xmlunknown_h
#define _xmlunknown_h

#include <spl/xml/XmlNode.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into TiXmlUnknowns.
*/
class XmlUnknown : public XmlNode
{
protected:
	void CopyTo( XmlUnknown& target ) const;

public:
	inline XmlUnknown() : XmlNode( XmlNode::UNKNOWN ) {}
	inline XmlUnknown( const XmlUnknown& copy ) : XmlNode( XmlNode::UNKNOWN )	{ copy.CopyTo( *this ); }
	virtual ~XmlUnknown();

	void operator=( const XmlUnknown& copy ) { copy.CopyTo( *this ); }

	virtual String Name() const;
			
	/// Creates a copy of this Unknown and returns it.
	virtual XmlNodePtr Clone() const;

	virtual XmlUnknownPtr     ToUnknown() const; ///< Cast to a more defined type. Will return null not of the requested type.

	virtual StringPtr ToString() const;
	virtual void WriteTo(TextWriter& writer) const;

	static XmlUnknownPtr CreateUnknown();

	virtual const char* _Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );
};

REGISTER_TYPEOF( 546, XmlUnknown );

/** @} */
}
#endif
