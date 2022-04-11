#include <spl/interp/JsRegex.h>
#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/jsfunctions/JsRegexFn.h>

using namespace spl;

JsRegExp::~JsRegExp()
{
}

void JsRegExp::SetFunctions()
{
	SetProperty("compile", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsRegExpCompile)))));
	SetProperty("exec", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsRegExpExec)))));
	SetProperty("test", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsRegExpTest)))));
}

IJsObjectPtr JsRegExp::New()
{
	JsRegExp *obj = new JsRegExp(*this);
	return IJsObjectPtr(obj);
}

bool JsRegExp::IsNative() const
{
	return true;
}

StringPtr JsRegExp::ToString() const
{
	return m_regex.Expression().ToString();
}

StringPtr JsRegExp::TypeName() const
{
	return StringPtr(new String("RegExp"));
}

VariantPtr JsRegExp::GetProperty(const String& idx)
{
	if (idx == "global")
	{
		return VariantPtr(new Variant(m_regex.IsGlobalSet()));
	}
	if (idx == "ignoreCase")
	{
		return VariantPtr(new Variant(m_regex.IsIgnoreCaseSet()));
	}
	if (idx == "lastIndex")
	{
		return VariantPtr(new Variant(LastMatch()));
	}
	if (idx == "multiline")
	{
		return VariantPtr(new Variant(m_regex.IsMultilineSet()));
	}
	if (idx == "source")
	{
		return VariantPtr(new Variant(m_regex.Expression()));
	}
	return JsMethod::GetProperty(idx);
}

bool JsRegExp::HasProperty(const String& idx) const
{
	return idx == "global" ||
		idx == "ignoreCase" ||
		idx == "lastIndex" ||
		idx == "multiline" ||
		idx == "source" ||
		JsMethod::HasProperty(idx);
}

VariantPtr JsRegExp::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	int flags = RegexOptions::NONE;	
	if (args.Count() > 1)
	{
		StringPtr fa(args.ElementAtRef(1)->ToString());
		if (fa->IndexOf('g') < 0)
		{
			//flags |= (int)RegexOptions::EXPLICIT_CAPTURE;
			flags |= REG_UNGREEDY;
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
	m_regex = Regex(args.ElementAtRef(0)->ToString(), (RegexOptions::Options)flags);
	return VariantPtr(new Variant());
}

#if defined(DEBUG) || defined(_DEBUG)
void JsRegExp::CheckMem() const
{
	m_regex.CheckMem();
	JsMethod::CheckMem();
}

void JsRegExp::ValidateMem() const
{
	m_regex.ValidateMem();
	JsMethod::ValidateMem();
}
#endif

