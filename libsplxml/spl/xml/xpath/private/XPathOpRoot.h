#ifndef _xpathoproot_h
#define _xpathoproot_h

#include <spl/RefCountPtrCast.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
class XPathOpRoot;
typedef RefCountPtrCast<XPathOpRoot, XPathOperator, XPathOperatorPtr> XPathOpRootPtr;

/** @brief Selects the root node.
 */
class XPathOpRoot : public XPathOperator
{
public:
	XPathOpRoot();
	XPathOpRoot(const XPathOpRoot& op);
	virtual ~XPathOpRoot();
	
	XPathOpRoot& operator =(const XPathOpRoot& oproot);
	
	virtual bool IsMatch(XmlNode& context);
	virtual XmlNodeListPtr NextContext(XmlNodePtr context);
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}

#endif
