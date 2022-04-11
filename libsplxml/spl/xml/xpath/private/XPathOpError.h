#ifndef _xpathoperror_h
#define _xpathoperror_h

#include <spl/String.h>
#include <spl/RefCountPtrCast.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
class XPathOpError;
typedef RefCountPtrCast<XPathOpError, XPathOperator, XPathOperatorPtr> XPathOpErrorPtr;

/** @brief Selects the root node.
 */
class XPathOpError : public XPathOperator
{
private:
	String m_msg;
	
public:
	XPathOpError();
	XPathOpError(const String& msg);
	XPathOpError(const XPathOpError& op);
	virtual ~XPathOpError();
	
	XPathOpError& operator =(const XPathOpError& oproot);
	
	virtual bool IsMatch(XmlNode& context);
	virtual XmlNodeListPtr NextContext(XmlNodePtr context);
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}

#endif
