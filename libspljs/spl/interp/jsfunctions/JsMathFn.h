#ifndef _jsmathfn_h
#define _jsmathfn_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	extern VariantPtr JsMathAbs(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathAcos(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathAsin(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathAtan(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathAtan2(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathCeil(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathCos(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathExp(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathFloor(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathLog(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathMax(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathMin(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathPow(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathRandom(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathRound(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathSin(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathSqrt(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsMathTan(JsMethod *isthis, Vector<VariantPtr>& args);

	extern VariantPtr JsGlobalIsFinite(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsGlobalIsNan(JsMethod *isthis, Vector<VariantPtr>& args);
}
#endif
