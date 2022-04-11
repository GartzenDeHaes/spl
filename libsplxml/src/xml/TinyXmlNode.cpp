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

#include <spl/xml/XmlAttribute.h>
#include <spl/xml/XmlComment.h>
#include <spl/xml/XmlDeclaration.h>
#include <spl/xml/XmlDocument.h>
#include <spl/xml/XmlElement.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlText.h>
#include <spl/xml/XmlUnknown.h>
#include <spl/xml/xpath/XPath.h>

using namespace spl;

XmlNode::XmlNode( XmlNodeType _type )
: m_value(25), m_type(_type), m_prev(), m_next(), m_firstChild(), m_lastChild(), m_parent(NULL)
{
}

XmlNode::~XmlNode()
{
	Clear();
	m_self.Clear();
}

void XmlNode::CopyTo( XmlNode& target ) const
{
	target.SetValue(m_value.ToString());
}

void XmlNode::Clear()
{
	XmlNodePtr node = m_firstChild;
	XmlNodePtr temp;

	while ( node.IsNotNull() )
	{
		temp = node->m_next;
		node->Clear();
		node = temp;
	}

	m_next.Release();
	m_prev.Release();
	m_firstChild.Release();
	m_lastChild.Release();

	m_parent = NULL;
}

XmlNodeListPtr XmlNode::ChildNodes() const
{
	XmlNodeListPtr nodes(new XmlNodeList());
	XmlNodePtr node(m_firstChild);
	
	if (node.IsNull())
	{
		return nodes;
	}
	
	for ( ; node.IsNotNull(); node = node->NextSibling() )
	{
		nodes->Add(node);
	}
	
	return nodes;
}

void XmlNode::AppendChild( XmlNodePtr node )
{
	node.ValidateMem();
	ASSERT( node->m_parent == NULL || node->m_parent == this );
	ASSERT( node.Get() != this );

	if ( node->NodeType() == XmlNode::DOCUMENT )
	{
		throw new XmlException("Cannot have a document as as child", -1, -1);
	}

	ASSERT( node->GetDocument().IsNull() || node->GetDocument().Get() == this->GetDocument().Get() );

	node->m_parent = this;

	node->m_prev = LastChild();
	node->m_next.Release();

	if ( m_lastChild.IsNotNull() )
	{
		m_lastChild->m_next = node;
	}
	else
	{
		m_firstChild = node;			// it was an empty list.
	}

	m_lastChild = node;
}

void XmlNode::InsertBefore( XmlNodePtr beforeThis, XmlNodePtr addThis )
{	
	if ( beforeThis.IsNull() || beforeThis->m_parent != this ) 
	{
		throw  new XmlException("InsertBeforeChild: Argument is not a child of this node", -1, -1);
	}
	if ( addThis->NodeType() == XmlNode::DOCUMENT )
	{
		throw new XmlException("InsertBeforeChild: Cannot add document childern", -1, -1);
	}

	addThis->m_parent = this;

	addThis->m_next = beforeThis;
	addThis->m_prev = beforeThis->m_prev;
	if ( beforeThis->m_prev.IsNotNull() )
	{
		beforeThis->m_prev->m_next = addThis;
	}
	else
	{
		ASSERT( m_firstChild.Get() == beforeThis.Get() );
		m_firstChild = addThis;
	}
	beforeThis->m_prev = addThis;
}

void XmlNode::InsertAfter( XmlNodePtr afterThis, XmlNodePtr addThis )
{
	if ( afterThis.IsNull() || afterThis->m_parent != this ) 
	{
		throw new XmlException("InsertAfterChild: Argument is not a child of this node", -1, -1);
	}
	if ( addThis->NodeType() == XmlNode::DOCUMENT )
	{
		throw new XmlException("InsertAfterChild: Cannot add document childern", -1, -1);
	}

	addThis->m_parent = this;

	addThis->m_prev = afterThis;
	addThis->m_next = afterThis->m_next;
	if ( afterThis->m_next.IsNotNull() )
	{
		afterThis->m_next->m_prev = addThis;
	}
	else
	{
		ASSERT( m_lastChild.Get() == afterThis.Get() );
		m_lastChild = addThis;
	}
	afterThis->m_next = addThis;
}

void XmlNode::ReplaceChild( XmlNodePtr replaceThis, XmlNodePtr withThis )
{
	replaceThis.ValidateMem();

	if ( replaceThis->m_parent != this )
	{
		throw new XmlException("ReplaceChild: argument is not a child of this node", -1, -1);
	}

	withThis->m_next = replaceThis->m_next;
	withThis->m_prev = replaceThis->m_prev;

	if ( replaceThis->m_next.IsNotNull() )
	{
		replaceThis->m_next->m_prev = withThis;
	}
	else
	{
		m_lastChild = withThis;
	}

	if ( replaceThis->m_prev.IsNotNull() )
	{
		replaceThis->m_prev->m_next = withThis;
	}
	else
	{
		m_firstChild = withThis;
	}

	withThis->m_parent = this;
}

bool XmlNode::RemoveChild( XmlNodePtr removeThis )
{
	removeThis.ValidateMem();

	if ( removeThis->m_parent != this )
	{	
		throw new XmlException("RemoveChild: Argument is not a child of this node", -1, -1);
	}

	if ( removeThis->m_next.IsNotNull() )
	{
		removeThis->m_next->m_prev = removeThis->m_prev;
	}
	else
	{
		m_lastChild = removeThis->m_prev;
	}

	if ( removeThis->m_prev.IsNotNull() )
	{
		removeThis->m_prev->m_next = removeThis->m_next;
	}
	else
	{
		m_firstChild = removeThis->m_next;
	}

	return true;
}

XmlNodePtr XmlNode::FirstChild( const String& _value ) const
{
	if (m_firstChild.IsNull())
	{
		return XmlNodePtr();
	}
	XmlNodePtr node;
	for ( node = m_firstChild; node.IsNotNull(); node = node->m_next )
	{
		if ( node->Name().Equals(_value) )
		{
			return node;
		}
	}
	return XmlNodePtr();
}

XmlNodePtr XmlNode::LastChild( const String& _value ) const
{
	if (m_lastChild.IsNull())
	{
		return XmlNodePtr();
	}
	XmlNodePtr node;
	for ( node = m_lastChild; node.IsNotNull(); node = node->m_prev )
	{
		if ( node->Name().Equals(_value) )
		{
			return node;
		}
	}
	return XmlNodePtr();
}

XmlNodePtr XmlNode::Child( int count ) const
{
	int i;
	XmlNodePtr child = FirstChild();
	for (	i=0;
			child.IsNotNull() && i< count;
			child = child->NextSibling(), ++i )
	{
		// nothing
	}
	return child;
}

RefCountPtr<XmlNodeList> XmlNode::Decendants()
{
	RefCountPtr<XmlNodeList> list(new XmlNodeList());
	
	for(XmlNode::Iterator iter(Begin()); iter.Next(); )
	{
		list->Add(iter.Current());
	}
	return list;
}

XmlNodePtr XmlNode::NextSibling( const String& _value ) 
{
	XmlNodePtr node;
	for ( node = m_next; node.IsNotNull(); node = node->m_next )
	{
		if ( node->Name().Equals(_value) )
		{
			return node;
		}
	}
	return XmlNodePtr();
}

XmlNodePtr XmlNode::PreviousSibling( const String& _value )
{
	XmlNodePtr node;
	for ( node = m_prev; node.IsNotNull(); node = node->m_prev )
	{
		if ( node->Name().Equals(_value) )
		{
			return node;
		}
	}
	return XmlNodePtr();
}

XmlElementPtr XmlNode::FirstChildElement()
{
	XmlNodePtr node;

	for ( node = FirstChild(); node.IsNotNull(); node = node->NextSibling() )
	{
		if ( node->IsElement() )
		{
			return node->ToElement();
		}
	}
	return XmlNodePtr();
}

XmlElementPtr XmlNode::FirstChildElement( const String& _value )
{
	XmlNodePtr node;

	for (node = FirstChild( _value ); node.IsNotNull();	node = node->NextSibling( _value ) )
	{
		if ( node->IsElement() )
		{
			return node->ToElement();
		}
	}
	return XmlNodePtr();
}

XmlElementPtr XmlNode::NextSiblingElement()
{
	XmlNodePtr node;

	for (node = NextSibling(); node.IsNotNull(); node = node->NextSibling() )
	{
		if ( node->IsElement() )
		{
			return node->ToElement();
		}
	}
	return XmlNodePtr();
}

XmlElementPtr XmlNode::NextSiblingElement( const String& _value )
{
	XmlNodePtr node;

	for (node = NextSibling( _value ); node.IsNotNull(); node = node->NextSibling( _value ) )
	{
		if ( node->IsElement() )
		{
			return node->ToElement();
		}
	}
	return XmlNodePtr();
}

XmlDocumentPtr XmlNode::GetDocument()
{
	XmlNode* node;

	for( node = this; NULL != node; node = node->m_parent )
	{
#ifdef DEBUG2
		ASSERT_PTR(node);
		node->ValidateMem();
#endif
		if ( node->IsDocument() )
		{
			return (XmlNodePtr)node->m_self;
		}
	}
	return XmlDocumentPtr();
}

StringPtr XmlNode::InnerText() const
{
	return m_value.ToString();
}

StringPtr XmlNode::InnerXml() const
{
	return StringPtr(new String());
}

XmlNodeListPtr XmlNode::SelectNodes(const String& xpath)
{
	XPath xp(xpath);
	return xp.SelectNodes(m_self);
}

XmlNodePtr XmlNode::SelectSingleNode(const String& xpath)
{
	XPath xp(xpath);
	return xp.SelectSingleNode(m_self);
}

XmlDocumentPtr    XmlNode::ToDocument()    const { return XmlNodePtr(); } ///< Cast to a more defined type. Will return null if not of the requested type.
XmlElementPtr     XmlNode::ToElement()     const { return XmlNodePtr(); } ///< Cast to a more defined type. Will return null if not of the requested type.
XmlCommentPtr     XmlNode::ToComment()     const { return XmlNodePtr(); } ///< Cast to a more defined type. Will return null if not of the requested type.
XmlUnknownPtr     XmlNode::ToUnknown()     const { return XmlNodePtr(); } ///< Cast to a more defined type. Will return null if not of the requested type.
XmlTextPtr        XmlNode::ToText()        const { return XmlNodePtr(); } ///< Cast to a more defined type. Will return null if not of the requested type.
XmlDeclarationPtr XmlNode::ToDeclaration() const { return XmlNodePtr(); } ///< Cast to a more defined type. Will return null if not of the requested type.

#ifdef DEBUG
void XmlNode::ValidateMem() const
{
	m_value.ValidateMem();

#ifdef DEBUG2
	m_firstChild.ValidateMem();
	XmlNodePtr node = m_firstChild;
	while ( node.IsNotNull() )
	{
		node.ValidateMem();
		node = node->m_next;
	}
#endif
}

void XmlNode::CheckMem() const
{
	m_value.CheckMem();
}
#endif

XmlNode::Iterator::Iterator(XmlNodePtr root)	//< root is not iterated
: m_stack(), m_current()
{
	XmlNodeListPtr childern(root->ChildNodes());
	for (int x = 0; x < childern->Count(); x++)
	{
		m_stack.Add(childern->Item(x));
	}
}

XmlNode::Iterator::Iterator(const XmlNode::Iterator& iter)
: m_stack(iter.m_stack), m_current(iter.m_current)
{
}

XmlNode::Iterator::~Iterator()
{
}

XmlNode::Iterator& XmlNode::Iterator::operator =(const XmlNode::Iterator iter)
{
	m_stack = iter.m_stack;
	m_current = iter.m_current;
	
	return *this;
}

bool XmlNode::Iterator::Next()
{
	if (m_stack.Count() == 0)
	{
		return false;
	}
	
	m_current = m_stack.Tail();
	m_stack.RemoveTail();
	
	XmlNodeListPtr childern(m_current->ChildNodes());
	for (int x = 0; x < childern->Count(); x++)
	{
		m_stack.Add(childern->Item(x));
	}

	return true;
}

bool XmlNode::Iterator::Prev( )
{
	throw new NotImplementedException();
}

XmlNodePtr XmlNode::Iterator::Current()
{
	return m_current;	
}

XmlNodePtr& XmlNode::Iterator::CurrentRef()
{
	return m_current;
}

XmlUnknown::~XmlUnknown() 
{
}

XmlUnknownPtr XmlUnknown::ToUnknown() const 
{ 
	return (XmlNodePtr)m_self; 
} 

