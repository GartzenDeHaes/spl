#ifndef _jsstringfn_h
#define _jsstringfn_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	extern VariantPtr JsStringCharAt(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringCharCodeAt(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringConcat(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringFromCharCode(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringIndexOf(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringLastIndexOf(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringSplit(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringSubstr(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringSubString(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringToLowerCase(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsStringToUpperCase(JsMethod *isthis, Vector<VariantPtr>& args);
}
#endif
