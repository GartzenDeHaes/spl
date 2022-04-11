#ifndef _jsdatefn_h
#define _jsdatefn_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	extern VariantPtr JsDateGetDate(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateGetDay(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateGetFullYear(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateGetYear(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateGetHours(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateGetMinutes(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateGetSeconds(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateGetTime(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateParse(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetDate(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetFullYear(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetYear(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetHours(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetMinutes(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetMonth(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetSeconds(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateSetTime(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsDateToDateString(JsMethod *isthis, Vector<VariantPtr>& args);
}
#endif
