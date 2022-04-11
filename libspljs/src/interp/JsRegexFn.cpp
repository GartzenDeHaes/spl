#include <spl/interp/JsArray.h>
#include <spl/interp/jsfunctions/JsRegexFn.h>
#include <spl/interp/JsRegex.h>
#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/JsString.h>

namespace spl
{
	VariantPtr JsRegExpCompile(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		Debug::AssertMem((JsRegExp *)realThis, sizeof(JsRegExp));
		realThis->ValidateMem();
		
		int flags = RegexOptions::NONE;	
		if (args.Count() > 1)
		{
			StringPtr fa(args.ElementAtRef(1)->ToString());
			if (fa->IndexOf('g') < 0)
			{
				//flags |= (int)RegexOptions::EXPLICIT_CAPTURE;
			}
			if (fa->IndexOf('i') > -1)
			{
				flags |= (int)RegexOptions::IGNORE_CASE;		
			}
			if (fa->IndexOf('m') > -1)
			{
				flags |= (int)RegexOptions::MULTILINE;	
			}
		}
		
		((JsRegExp *)realThis)->GetRegex() = Regex(*args.ElementAtRef(0)->ToString(), (RegexOptions::Options)flags);
		return VariantPtr(new Variant());
	}

	VariantPtr JsRegExpExec(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		Debug::AssertMem((JsRegExp *)realThis, sizeof(JsRegExp));
		realThis->ValidateMem();
		
		StringPtr txt(args.ElementAtRef(0)->ToString());
		RefCountPtr<Array<StringPtr> > s(((JsRegExp *)realThis)->GetRegex().Matches(txt));
		if (s->Count() == 0)
		{
			return VariantPtr(new Variant(Null::Instance()));
		}
		
		JsArray *arr = new JsArray();
		arr->SetProperty("index", VariantPtr(new Variant(((JsRegExp *)realThis)->LastMatch())));
		arr->SetProperty("input", args.ElementAtRef(0));

		for (int x = 0; x < s->Count(); x++)
		{
			arr->SetProperty(Int32::ToString(x), VariantPtr(new Variant(s->ElementAtRef(x))));
		}
		
		return VariantPtr(new Variant(RefCountPtr<IJsObject> (arr)));
	}

	VariantPtr JsRegExpTest(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		JsMethod *realThis = ((JsFunctionDispatch *)isthis)->ReallyThis();
		Debug::AssertMem((JsRegExp *)realThis, sizeof(JsRegExp));
		realThis->ValidateMem();

		return VariantPtr(new Variant(((JsRegExp *)realThis)->GetRegex().IsMatch(args.ElementAtRef(0)->ToString())));
	}
}
