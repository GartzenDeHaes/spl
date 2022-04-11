#ifndef _consolewriteln_h
#define _consolewriteln_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	class ConsoleWriteln : public JsMethod
	{
	public:
		ConsoleWriteln();
		virtual ~ConsoleWriteln();
		
		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
	};

	class ConsoleReadln : public JsMethod
	{
	public:
		ConsoleReadln();
		virtual ~ConsoleReadln();
		
		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
	};
}
#endif
