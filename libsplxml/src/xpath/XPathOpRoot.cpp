#include <spl/xml/xpath/private/XPathOpRoot.h>

using namespace spl;

XPathOpRoot::XPathOpRoot()
{
}

XPathOpRoot::XPathOpRoot(const XPathOpRoot& op)
: XPathOperator(op)
{
}

XPathOpRoot::~XPathOpRoot()
{
}

XPathOpRoot& XPathOpRoot::operator =(const XPathOpRoot& oproot)
{
	return *this;
}

bool XPathOpRoot::IsMatch(XmlNode& context)
{
	return context.ParentNode().IsNull();
}

XmlNodeListPtr XPathOpRoot::NextContext(XmlNodePtr context)
{
	ASSERT(IsMatch(context));
	XmlNodeListPtr nodes = XmlNodeListPtr(new XmlNodeList());
	nodes->Add(context);
	return nodes;
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOpRoot::CheckMem() const
{
	XPathOperator::CheckMem();
}

void XPathOpRoot::ValidateMem() const
{
	XPathOperator::ValidateMem();
}
#endif
