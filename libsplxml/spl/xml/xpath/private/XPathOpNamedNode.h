#ifndef _xpathopnamednode_h
#define _xpathopnamednode_h

#include <spl/RefCountPtrCast.h>
#include <spl/String.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
class XPathOpNamedNode;
typedef RefCountPtrCast<XPathOpNamedNode, XPathOperator, XPathOperatorPtr> XPathOpNamedNodePtr;

/** @brief Selects the root node.
 */
class XPathOpNamedNode : public XPathOperator
{
private:
	String m_nodeName;
	
public:
	XPathOpNamedNode(const String& nodeName);
	XPathOpNamedNode(const XPathOpNamedNode& op);
	virtual ~XPathOpNamedNode();
	
	XPathOpNamedNode& operator =(const XPathOpNamedNode& oproot);
	
	virtual bool IsMatch(XmlNode& context);
	virtual XmlNodeListPtr NextContext(XmlNodePtr context);
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}

#endif
