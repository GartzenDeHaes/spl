#include <spl/xml/xpath/private/XPathOpError.h>

using namespace spl;

XPathOpError::XPathOpError(const String& msg)
: m_msg(msg)
{
}

XPathOpError::XPathOpError()
: m_msg("Unsupported operation")
{
}

XPathOpError::XPathOpError(const XPathOpError& op)
: XPathOperator(op), m_msg(op.m_msg)
{
}

XPathOpError::~XPathOpError()
{
}

XPathOpError& XPathOpError::operator =(const XPathOpError& oproot)
{
	m_msg = oproot.m_msg;
	return *this;
}

bool XPathOpError::IsMatch(XmlNode& context)
{
	throw new Exception(m_msg);
}

XmlNodeListPtr XPathOpError::NextContext(XmlNodePtr context)
{
	throw new Exception(m_msg);
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOpError::CheckMem() const
{
	XPathOperator::CheckMem();
	m_msg.CheckMem();
}

void XPathOpError::ValidateMem() const
{
	XPathOperator::ValidateMem();
	m_msg.ValidateMem();
}
#endif
