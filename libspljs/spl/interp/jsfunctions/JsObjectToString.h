#ifndef _jsobjecttostring_h
#define _jsobjecttostring_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	class JsObjectToString : public JsMethod
	{
	public:
		JsObjectToString();
		JsObjectToString(const JsObjectToString& obj);
		virtual ~JsObjectToString();
		
		virtual bool IsNative() const;
		virtual VariantPtr Call(IJsObjectPtr isthis, Vector<VariantPtr>& args);
	};
}
#endif
