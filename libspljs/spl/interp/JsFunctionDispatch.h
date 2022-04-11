#ifndef _jsfunctiondispatch_h
#define _jsfunctiondispatch_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	class JsFunctionDispatch : public JsMethod
	{
	private:
		JsMethod *m_reallyThis;
		VariantPtr (*m_call)(JsMethod *isthis, Vector<VariantPtr>& args);
		
	public:
		JsFunctionDispatch(JsMethod *reallyThis, int argCount, VariantPtr (*call)(JsMethod *isthis, Vector<VariantPtr>& args));
		virtual ~JsFunctionDispatch();
		
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		virtual bool IsNative() const;
		
		inline JsMethod *ReallyThis()
		{
			return m_reallyThis;
		}
	};
}
#endif
