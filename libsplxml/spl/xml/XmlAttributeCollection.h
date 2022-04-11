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
#ifndef _xmlattributecollection_h
#define _xmlattributecollection_h

#include <spl/collection/List.h>
#include <spl/xml/XmlAttribute.h>
#include <spl/xml/XmlNamedNodeMap.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

class XmlAttributeCollection;
typedef RefCountPtrCast<XmlAttributeCollection, XmlNamedNodeMap, XmlNamedNodeMapPtr> XmlAttributeCollectionPtr;

REGISTER_TYPEOF( 528, XmlAttributeCollectionPtr );

/** @brief XML tag attribute collection.
 *
 */
class XmlAttributeCollection : public XmlNamedNodeMap
{			
public:
	XmlAttributeCollection();
	virtual ~XmlAttributeCollection();

	void Add(XmlAttributePtr node);
			
	XmlAttributePtr Find( const String& name ) const;
	XmlAttributePtr First() const;
	
	void Remove( XmlAttributePtr attrib );
			
	virtual StringPtr ToString() const;
	virtual void Write(TextWriter& writer) const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 530, XmlAttributeCollection );

/** @} */
}
#endif
