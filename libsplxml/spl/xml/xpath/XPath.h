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
#ifndef _xpath_h
#define _xpath_h

#include <spl/types.h>
#include <spl/collection/Array.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/collection/List.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlNodeList.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

class XPath;
typedef RefCountPtr<XPath> XPathPtr;

/** @brief Simple XPATH node selection.
 *	@ref XmlNode
 *	Implemented symbols are /, //, @, [], &lt;, &lt;=, &gt;,&gt;=, =, !=. Examples that should work:
 *	<ul>
 *	<li>"/"</li>
 *	<li>"/bookstore"</li>
 *	<li>"/bookstore/book"</li>
 *	<li>"/bookstore/*"</li>
 *	<li>"/bookstore//price"</li>
 *	<li>"/bookstore/book/title@lang"</li>
 *	<li>"/bookstore/@"</li>
 *	<li>"/bookstore/book[@category = 'WEB']"</li>
 *	</ul>
 */
class XPath : public IMemoryValidate
{
private:
	Array<XPathOperatorPtr> m_ops;
	
protected:
	static XmlNodeListPtr SelectNodes(Array<XPathOperatorPtr>&ops, XmlNodePtr context, bool findAll);
	
	inline XmlNodeListPtr SelectNodes(XmlNodePtr context, bool findAll)
	{
		return SelectNodes(m_ops, context, findAll);
	}
	
public:
	XPath();
	XPath(const String& xp);
	XPath(const XPath& xp);
	virtual ~XPath();
	
	XPath& operator =(const XPath& xp);
	
	XmlNodeListPtr SelectNodes(XmlNodePtr context);
	XmlNodePtr SelectSingleNode(XmlNodePtr context);

	static XmlNodeListPtr SelectNodes(Array<XPathOperatorPtr>&ops, XmlNodePtr context);
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

/** @} */
}
#endif
