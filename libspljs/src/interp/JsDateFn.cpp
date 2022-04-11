#include <spl/interp/jsfunctions/JsDateFn.h>
#include <spl/interp/JsDate.h>
#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/JsString.h>

namespace spl
{
	VariantPtr JsDateGetDate(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsDate *)realThis)->GetDateTime().Day()));
	}

	VariantPtr JsDateGetDay(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsDate *)realThis)->GetDateTime().DayOfWeek()));
	}

	VariantPtr JsDateGetFullYear(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsDate *)realThis)->GetDateTime().Year()));
	}

	VariantPtr JsDateGetYear(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsDate *)realThis)->GetDateTime().Year()));
	}

	VariantPtr JsDateGetHours(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsDate *)realThis)->GetDateTime().Hour()));
	}

	VariantPtr JsDateGetMinutes(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsDate *)realThis)->GetDateTime().Minutes()));
	}

	VariantPtr JsDateGetSeconds(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsDate *)realThis)->GetDateTime().Seconds()));
	}

	VariantPtr JsDateGetTime(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant((int64)((JsDate *)realThis)->GetDateTime().ToSysTime()));
	}

	VariantPtr JsDateParse(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		if (args.Count() != 1)
		{
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate())));
		}
		
		StringPtr str(args.ElementAtRef(0)->ToString());
		if (DateTime::IsDateTime(*str))
		{
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime::Parse(*str)))));
		}
		return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate())));
	}

	VariantPtr JsDateSetDate(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int i;
		if (args.ElementAtRef(0)->ToInt32(i))
		{
			JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
			DateTime dtm(((JsDate *)realThis)->GetDateTime());
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime(dtm.Year(), dtm.Month(), i, dtm.Hour(), dtm.Minutes(), dtm.Seconds())))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateSetFullYear(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int i;
		if (args.ElementAtRef(0)->ToInt32(i))
		{
			JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
			DateTime dtm(((JsDate *)realThis)->GetDateTime());
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime(i, dtm.Month(), dtm.Day(), dtm.Hour(), dtm.Minutes(), dtm.Seconds())))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateSetYear(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int i;
		if (args.ElementAtRef(0)->ToInt32(i))
		{
			JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
			DateTime dtm(((JsDate *)realThis)->GetDateTime());
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime(i, dtm.Month(), dtm.Day(), dtm.Hour(), dtm.Minutes(), dtm.Seconds())))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateSetHours(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int i;
		if (args.ElementAtRef(0)->ToInt32(i))
		{
			JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
			DateTime dtm(((JsDate *)realThis)->GetDateTime());
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime(dtm.Year(), dtm.Month(), dtm.Day(), i, dtm.Minutes(), dtm.Seconds())))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateSetMinutes(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int i;
		if (args.ElementAtRef(0)->ToInt32(i))
		{
			JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
			DateTime dtm(((JsDate *)realThis)->GetDateTime());
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime(dtm.Year(), dtm.Month(), dtm.Day(), dtm.Hour(), i, dtm.Seconds())))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateSetMonth(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int i;
		if (args.ElementAtRef(0)->ToInt32(i))
		{
			JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
			DateTime dtm(((JsDate *)realThis)->GetDateTime());
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime(dtm.Year(), i, dtm.Day(), dtm.Hour(), dtm.Minutes(), dtm.Seconds())))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateSetSeconds(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int i;
		if (args.ElementAtRef(0)->ToInt32(i))
		{
			JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
			DateTime dtm(((JsDate *)realThis)->GetDateTime());
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime(dtm.Year(), dtm.Month(), dtm.Day(), dtm.Hour(), dtm.Minutes(), i)))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateSetTime(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int64 i;
		if (args.ElementAtRef(0)->ToInt64(i))
		{
			return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsDate(DateTime((time_t)i)))));
		}
		return VariantPtr(new Variant());
	}

	VariantPtr JsDateToDateString(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		DateTime dtm(((JsDate *)realThis)->GetDateTime());
		return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsString(dtm.ToString()))));
	}
}
