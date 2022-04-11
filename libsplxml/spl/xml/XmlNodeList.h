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
#ifndef _xmlnodelist_h
#define _xmlnodelist_h

#include <spl/Debug.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>
#include <spl/xml/XmlNode.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

class XmlNodeList;
typedef RefCountPtr<XmlNodeList> XmlNodeListPtr;
typedef WeakReference<XmlNodeList, XmlNodeListPtr> XmlNodeListRef;

REGISTER_TYPEOF( 522, XmlNodeListPtr );
REGISTER_TYPEOF( 524, XmlNodeListRef );

/** @brief List of nodes.
 *
 */
class XmlNodeList : public IMemoryValidate
{
protected:
	Vector<XmlNodePtr> m_nodes;
	
	friend class XmlElement;

public:
	XmlNodeList();
	virtual ~XmlNodeList();

	inline int Count() const 
	{ 
		return m_nodes.Count(); 
	}
	
	inline XmlNodePtr Item(int idx) const
	{
		return m_nodes.ElementAt(idx);
	}

	void Clear();
	
	inline void Add(XmlNodePtr node)
	{
		m_nodes.Add(node);
	}
	
	inline void Add(const XmlNodeList& list)
	{
		m_nodes.AddRange(list.m_nodes);
	}
	
	inline Vector<XmlNodePtr>::Iterator Begin()
	{
		return m_nodes.Begin();
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 527, XmlNodeList );

/** @} */
}
#endif
