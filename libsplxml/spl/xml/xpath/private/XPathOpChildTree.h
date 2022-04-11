#ifndef _xpathopchildtree_h
#define _xpathopchildtree_h

#include <spl/RefCountPtrCast.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
class XPathOpChildTree;
typedef RefCountPtrCast<XPathOpChildTree, XPathOperator, XPathOperatorPtr> XPathOpChildTreePtr;

/** @brief Selects the root node.
 */
class XPathOpChildTree : public XPathOperator
{
public:
	XPathOpChildTree();
	XPathOpChildTree(const XPathOpChildTree& op);
	virtual ~XPathOpChildTree();
	
	XPathOpChildTree& operator =(const XPathOpChildTree& oproot);
	
	virtual bool IsMatch(XmlNode& context);
	virtual XmlNodeListPtr NextContext(XmlNodePtr context);
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}

#endif
