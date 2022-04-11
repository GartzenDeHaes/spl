#include <spl/xml/xpath/private/XPathOpAttrib.h>
#include <spl/xml/XmlElement.h>
#include <spl/xml/XmlNode.h>

using namespace spl;

XPathOpAttrib::XPathOpAttrib()
{
}

XPathOpAttrib::XPathOpAttrib(const XPathOpAttrib& op)
: XPathOperator(op)
{
}

XPathOpAttrib::~XPathOpAttrib()
{
}

XPathOpAttrib& XPathOpAttrib::operator =(const XPathOpAttrib& oproot)
{
	return *this;
}

bool XPathOpAttrib::IsMatch(XmlNode& context)
{
	return context.IsElement() && context.ToElement()->Attributes()->Count() > 0;
}

XmlNodeListPtr XPathOpAttrib::NextContext(XmlNodePtr context)
{
	ASSERT(IsMatch(context));
	XmlNodeListPtr nodes(new XmlNodeList());

	XmlElementPtr node(context->ToElement());
	int count = node->Attributes()->Count();

	for (int x = 0; x < count; x++)
	{
		nodes->Add(node->Attribute(x));
	}

	return nodes;
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOpAttrib::CheckMem() const
{
	XPathOperator::CheckMem();
}

void XPathOpAttrib::ValidateMem() const
{
	XPathOperator::ValidateMem();
}
#endif
