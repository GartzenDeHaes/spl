#include <spl/Int32.h>
#include <spl/text/StringBuffer.h>
#include <spl/interp/jsfunctions/JsStringFn.h>
#include <spl/interp/JsArray.h>
#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/JsString.h>

namespace spl
{
	static VariantPtr _StringIze(const char *txt)
	{
		return VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsString(txt))));
	}

	VariantPtr JsStringCharAt(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		char buf[2];
		buf[1] = '\0';
		buf[0] = ((JsString *)realThis)->GetString().CharAt(args.ElementAt(0)->ToInt32());
		return _StringIze(buf);
	}

	VariantPtr JsStringCharCodeAt(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		char ch = ((JsString *)realThis)->GetString().CharAt(args.ElementAt(0)->ToInt32());
		return VariantPtr(new Variant((int)ch));
	}

	VariantPtr JsStringConcat(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		StringBuffer buf;
		for (int x = 0; x < args.Count(); x++ )
		{
			buf.Append(args.ElementAt(x)->ToString());
		}
		return _StringIze(buf.GetChars());
	}

	VariantPtr JsStringFromCharCode(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		char buf[2];
		buf[1] = '\0';
		buf[0] = args.ElementAt(0)->ToInt32();
		return _StringIze(buf);
	}

	VariantPtr JsStringIndexOf(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		StringPtr stxt(args.ElementAt(0)->ToString());
		int pos = 0;
		if (args.Count() > 1)
		{
			pos = args.ElementAt(1)->ToInt32();
		}
		
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsString *)realThis)->GetString().IndexOf(stxt, pos)));
	}

	VariantPtr JsStringLastIndexOf(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		StringPtr stxt(args.ElementAt(0)->ToString());
		int pos = 0;
		if (args.Count() > 1)
		{
			pos = args.ElementAt(1)->ToInt32();
		}
		
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return VariantPtr(new Variant(((JsString *)realThis)->GetString().LastIndexOf(stxt, pos)));
	}

	VariantPtr JsStringSplit(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsArray *arr = new JsArray();
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		RefCountPtr<Vector<StringPtr> > splits(((JsString *)realThis)->GetString().Split(args.ElementAt(0)->ToString()));
		
		for (int x = 0; x < splits->Count(); x++)
		{
			arr->SetProperty(Int32::ToString(x), _StringIze(splits->ElementAtRef(x)->GetChars()));
		}
		
		return VariantPtr(new Variant(RefCountPtr<IJsObject> (arr)));
	}

	VariantPtr JsStringSubstr(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		int start = args.ElementAtRef(0)->ToInt32();
		int len = args.ElementAtRef(1)->ToInt32();
		return _StringIze(((JsString *)realThis)->GetString().Substring(start, len)->GetChars());
	}

	VariantPtr JsStringSubString(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		int start = args.ElementAtRef(0)->ToInt32();
		if (args.Count() > 1)
		{
			int stop = args.ElementAtRef(1)->ToInt32();
			return _StringIze(((JsString *)realThis)->GetString().Mid(start, stop)->GetChars());
		}
		else
		{
			return _StringIze(((JsString *)realThis)->GetString().Substring(start)->GetChars());
		}
	}

	VariantPtr JsStringToLowerCase(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return _StringIze(((JsString *)realThis)->GetString().ToLower()->GetChars());
	}

	VariantPtr JsStringToUpperCase(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		return _StringIze(((JsString *)realThis)->GetString().ToUpper()->GetChars());
	}
}
