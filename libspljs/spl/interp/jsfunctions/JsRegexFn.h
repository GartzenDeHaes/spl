#ifndef _jsregexfn_h
#define _jsregexfn_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	extern VariantPtr JsRegExpCompile(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsRegExpExec(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsRegExpTest(JsMethod *isthis, Vector<VariantPtr>& args);
}

#endif
