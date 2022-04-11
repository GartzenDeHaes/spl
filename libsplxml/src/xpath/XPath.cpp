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
#include <spl/xml/xpath/XPath.h>
#include <spl/xml/xpath/private/XPathParser.h>

using namespace spl;

XPath::XPath()
: m_ops()
{
}

XPath::XPath(const String& xp)
: m_ops()
{
	XPathParser parser;
	m_ops = parser.Parse(xp);
}

XPath::XPath(const XPath& xp)
: m_ops(xp.m_ops)
{
}

XPath::~XPath()
{
}

XPath& XPath::operator =(const XPath& xp)
{
	m_ops = xp.m_ops;
	return *this;
}

XmlNodeListPtr XPath::SelectNodes(XmlNodePtr context)
{
	return SelectNodes(context, true);
}

XmlNodePtr XPath::SelectSingleNode(XmlNodePtr context)
{
	XmlNodeListPtr nodes = SelectNodes(context, false);
	if (nodes->Count() == 0)
	{
		return XmlNodePtr();
	}
	return nodes->Item(0);
}

XmlNodeListPtr XPath::SelectNodes(Array<XPathOperatorPtr>&ops, XmlNodePtr context)
{
	return SelectNodes(ops, context, true);
}

/** @TODO Need to shortcut evalution is findAll is false.
 */
XmlNodeListPtr XPath::SelectNodes(Array<XPathOperatorPtr>&ops, XmlNodePtr context, bool findAll)
{
	XmlNodeListPtr contextList(new XmlNodeList());
	contextList->Add(context);
	
	for(Array<XPathOperatorPtr>::Iterator iterOps(ops.Begin()); iterOps.Next();)
	{
		XmlNodeListPtr innerCtxList(new XmlNodeList());
		
		for(Vector<XmlNodePtr>::Iterator iterCtx(contextList->Begin()); iterCtx.Next();)
		{
			if (iterOps.CurrentRef()->IsMatch(*iterCtx.Current()))
			{
				innerCtxList->Add(iterOps.CurrentRef()->NextContext(iterCtx.Current()));
			}
		}
		contextList = innerCtxList;
	}

	return contextList;
}

#if defined(DEBUG) || defined(_DEBUG)
void XPath::CheckMem() const
{
	m_ops.CheckMem();
}

void XPath::ValidateMem() const
{
	m_ops.ValidateMem();
}
#endif
