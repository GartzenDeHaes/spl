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
#ifndef _xmlnamednodemap_h
#define _xmlnamednodemap_h

#include <spl/Debug.h>
#include <spl/collection/Hashtable.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlNodeList.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

class XmlNamedNodeMap;
typedef RefCountPtrCast<XmlNamedNodeMap, XmlNodeList, XmlNodeListPtr> XmlNamedNodeMapPtr;

REGISTER_TYPEOF( 540, XmlNamedNodeMapPtr );

/** @brief Key value node map.
 *
 */
class XmlNamedNodeMap : public XmlNodeList
{
protected:
	Hashtable<String, XmlNodePtr> m_map;
	
public:
	XmlNamedNodeMap();
	virtual ~XmlNamedNodeMap();
	
	XmlNodePtr GetNamedItem(const String& name) const { return m_map.Get(name); }
		
	XmlNodePtr RemoveNamedItem(const String& name);
	
	void SetNamedItem(XmlNodePtr item);
			
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 542, XmlNamedNodeMap );

/** @} */
}
#endif
