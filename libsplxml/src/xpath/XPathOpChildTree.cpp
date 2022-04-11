#include <spl/xml/xpath/private/XPathOpChildTree.h>

using namespace spl;

XPathOpChildTree::XPathOpChildTree()
{
}

XPathOpChildTree::XPathOpChildTree(const XPathOpChildTree& op)
: XPathOperator(op)
{
}

XPathOpChildTree::~XPathOpChildTree()
{
}

XPathOpChildTree& XPathOpChildTree::operator =(const XPathOpChildTree& oproot)
{
	return *this;
}

bool XPathOpChildTree::IsMatch(XmlNode& context)
{
	return context.HasChildNodes();
}

XmlNodeListPtr XPathOpChildTree::NextContext(XmlNodePtr context)
{
	ASSERT(IsMatch(context));
	XmlNodeListPtr nodes = XmlNodeListPtr(new XmlNodeList());

	for(XmlNode::Iterator iter(context->Begin()); iter.Next(); )
	{
		nodes->Add(iter.Current());
	}

	return nodes;
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOpChildTree::CheckMem() const
{
	XPathOperator::CheckMem();
}

void XPathOpChildTree::ValidateMem() const
{
	XPathOperator::ValidateMem();
}
#endif
