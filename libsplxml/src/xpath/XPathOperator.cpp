#include <spl/xml/xpath/private/XPathOperator.h>

using namespace spl;

XPathOperator::XPathOperator()
{
}

XPathOperator::XPathOperator(const XPathOperator& o)
{
}

XPathOperator::~XPathOperator()
{
}

XPathOperator& XPathOperator::operator =(const XPathOperator& xp)
{
	return *this;
}

bool XPathOperator::IsMatch(XmlNode& context)
{
	return false;
}

XmlNodeListPtr XPathOperator::NextContext(XmlNodePtr context)
{
	return XmlNodeListPtr();
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOperator::CheckMem() const
{
}

void XPathOperator::ValidateMem() const
{
}
#endif
