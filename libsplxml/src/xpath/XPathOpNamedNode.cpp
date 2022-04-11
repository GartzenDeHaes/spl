#include <spl/xml/xpath/private/XPathOpNamedNode.h>

using namespace spl;

XPathOpNamedNode::XPathOpNamedNode(const String& nodeName)
: m_nodeName(nodeName)
{
}

XPathOpNamedNode::XPathOpNamedNode(const XPathOpNamedNode& op)
: XPathOperator(op)
{
	m_nodeName = op.m_nodeName;
}

XPathOpNamedNode::~XPathOpNamedNode()
{
}

XPathOpNamedNode& XPathOpNamedNode::operator =(const XPathOpNamedNode& oproot)
{
	m_nodeName = oproot.m_nodeName;
	return *this;
}

bool XPathOpNamedNode::IsMatch(XmlNode& context)
{
	return context.Name() == m_nodeName;
}

XmlNodeListPtr XPathOpNamedNode::NextContext(XmlNodePtr context)
{
	ASSERT(IsMatch(context));
	XmlNodeListPtr nodes = XmlNodeListPtr(new XmlNodeList());
	nodes->Add(context);
	return nodes;
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOpNamedNode::CheckMem() const
{
	XPathOperator::CheckMem();
	m_nodeName.CheckMem();
}

void XPathOpNamedNode::ValidateMem() const
{
	XPathOperator::ValidateMem();
	m_nodeName.ValidateMem();
}
#endif
