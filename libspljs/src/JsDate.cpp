#include <spl/interp/JsDate.h>
#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/jsfunctions/JsDateFn.h>

using namespace spl;

JsDate::~JsDate()
{
}

void JsDate::SetFunctions()
{
	SetProperty("getDate", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetDate)))));
	SetProperty("getDay", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetDay)))));
	SetProperty("getFullYear", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetFullYear)))));
	SetProperty("getYear", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetYear)))));
	SetProperty("getHours", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetHours)))));
	SetProperty("getMinutes", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetMinutes)))));
	SetProperty("getSeconds", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetSeconds)))));
	SetProperty("getTime", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateGetTime)))));
	SetProperty("parse", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateParse)))));
	SetProperty("setDate", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetDate)))));
	SetProperty("setFullYear", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetFullYear)))));
	SetProperty("setYear", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetYear)))));
	SetProperty("setHours", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetHours)))));
	SetProperty("setMinutes", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetMinutes)))));
	SetProperty("setMonth", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetMonth)))));
	SetProperty("setSeconds", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetSeconds)))));
	SetProperty("setTime", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsDateSetTime)))));
	SetProperty("toDateString", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsDateToDateString)))));
}

IJsObjectPtr JsDate::New()
{
	JsDate *obj = new JsDate(*this);
	return IJsObjectPtr(obj);
}

bool JsDate::IsNative() const
{
	return true;
}

StringPtr JsDate::ToString() const
{
	return m_date.ToString();
}

StringPtr JsDate::TypeName() const
{
	return StringPtr(new String("Date"));
}

VariantPtr JsDate::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	if(args.Count() == 1)
	{
		if (! args.ElementAtRef(0)->IsUndefined())
		{
			StringPtr str(args.ElementAtRef(0)->ToString());
			if (DateTime::IsDateTime(*str))
			{
				m_date = DateTime::Parse(*str);
			}
		}
	}
	else if (args.Count() > 2)
	{
		int y, m, d, h, min, sec;
		if (args.ElementAtRef(0)->ToInt32(y))
		{
			if (args.ElementAtRef(1)->ToInt32(m))
			{
				if (args.ElementAtRef(2)->ToInt32(d))
				{
					if (args.Count() == 6)
					{
						if (args.ElementAtRef(3)->ToInt32(h))
						{
							if (args.ElementAtRef(4)->ToInt32(min))
							{
								if (args.ElementAtRef(5)->ToInt32(sec))
								{
									m_date = DateTime(y, m, d, h, min, sec);
								}
							}
						}
					}
					else
					{
						m_date = DateTime(y, m, d);
					}
				}
			}			
		}
	}
	return VariantPtr(new Variant());
}

#if defined(DEBUG) || defined(_DEBUG)
void JsDate::CheckMem() const
{
	JsMethod::CheckMem();
}

void JsDate::ValidateMem() const
{
	JsMethod::ValidateMem();
}
#endif
