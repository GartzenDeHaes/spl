#ifndef _xpathoperator_h
#define _xpathoperator_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/collection/Vector.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlNodeList.h>

namespace spl
{
	class XPathOperator;
	typedef RefCountPtr<XPathOperator> XPathOperatorPtr;

	class XPathOperator : public IMemoryValidate
	{
	public:
		XPathOperator();
		XPathOperator(const XPathOperator& o);
		virtual ~XPathOperator();
		
		virtual XPathOperator& operator =(const XPathOperator& xp);
		
		virtual bool IsMatch(XmlNode& context);
		virtual XmlNodeListPtr NextContext(XmlNodePtr context);
		
	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}
#endif
