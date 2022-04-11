#ifndef _xpathopattrib_h
#define _xpathopattrib_h

#include <spl/Debug.h>
#include <spl/RefCountPtrCast.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
class XPathOpAttrib;
typedef RefCountPtrCast<XPathOpAttrib, XPathOperator, XPathOperatorPtr> XPathOpAttribPtr;

/** @brief Selects the root node.
 */
class XPathOpAttrib : public XPathOperator
{
public:
	XPathOpAttrib();
	XPathOpAttrib(const XPathOpAttrib& op);
	virtual ~XPathOpAttrib();
	
	XPathOpAttrib& operator =(const XPathOpAttrib& oproot);
	
	virtual bool IsMatch(XmlNode& context);
	virtual XmlNodeListPtr NextContext(XmlNodePtr context);
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}

#endif
