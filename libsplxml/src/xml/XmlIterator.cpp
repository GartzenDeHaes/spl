#include <spl/xml/XmlIterator.h>

using namespace spl;

XmlIterator::XmlIterator(XmlDocumentPtr doc)
: m_doc(doc), m_stack()
{
	m_stack.Add(doc);
}

bool XmlIterator::Next()
{
	if (m_stack.Count() == 0)
	{
		return false;
	}
	
	XmlNodePtr node(m_stack.Pop());

	XmlNodePtr chld(node->FirstChild());
	while( chld.IsNotNull() )
	{
		m_stack.Add(chld);
		chld = chld->NextSibling();
	}
	
	return m_stack.Count() > 0;
}

bool XmlIterator::Prev()
{
	throw new NotSupportedException("Prev not supported.");
	return false;
}

XmlNodePtr XmlIterator::Current()
{
	return m_stack.Peek();
}

XmlNodePtr& XmlIterator::CurrentRef()
{
	return m_stack.PeekRef();
}