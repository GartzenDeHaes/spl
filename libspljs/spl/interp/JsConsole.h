#ifndef _jsconsole_h
#define _jsconsole_h

#include <spl/interp/JsObject.h>

namespace spl
{
	class JsConsole;
	typedef RefCountPtrCast<JsConsole, JsObject, JsObjectPtr> JsConsolePtr;

	class JsConsole : public JsObject
	{
	public:
		JsConsole();
		virtual ~JsConsole();
		
		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;
		virtual void SetProperty(const String& idx, VariantPtr obj);

		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;
	};
}
#endif
