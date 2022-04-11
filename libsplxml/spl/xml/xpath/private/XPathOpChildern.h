#ifndef _xpathopchildern_h
#define _xpathopchildern_h

#include <spl/Debug.h>
#include <spl/RefCountPtrCast.h>
#include <spl/String.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
class XPathOpChildern;
typedef RefCountPtrCast<XPathOpChildern, XPathOperator, XPathOperatorPtr> XPathOpChildernPtr;

/** @brief Selects the root node.
 */
class XPathOpChildern : public XPathOperator
{
private:
	String m_name;
	
public:
	XPathOpChildern();
	XPathOpChildern(const String& name);
	XPathOpChildern(const XPathOpChildern& op);
	virtual ~XPathOpChildern();
	
	XPathOpChildern& operator =(const XPathOpChildern& oproot);
	
	virtual bool IsMatch(XmlNode& context);
	virtual XmlNodeListPtr NextContext(XmlNodePtr context);
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}

#endif
