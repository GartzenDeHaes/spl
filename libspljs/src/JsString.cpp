#include <spl/interp/JsString.h>
#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/jsfunctions/JsStringFn.h>

using namespace spl;

JsString::~JsString()
{
}

void JsString::SetFunctions()
{
	SetProperty("charAt", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsStringCharAt)))));
	SetProperty("charCodeAt", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsStringCharCodeAt)))));
	SetProperty("concat", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsStringConcat)))));
	SetProperty("fromCharCode", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsStringFromCharCode)))));
	SetProperty("indexOf", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsStringIndexOf)))));
	SetProperty("lastIndexOf", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsStringLastIndexOf)))));
	SetProperty("split", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsStringSplit)))));
	SetProperty("substr", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 2, JsStringSubstr)))));
	SetProperty("substring", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 2, JsStringSubString)))));
	SetProperty("toLowerCase", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsStringToLowerCase)))));
	SetProperty("toUpperCase", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsStringToUpperCase)))));
}

VariantPtr JsString::GetProperty(const String& idx)
{
	if (idx == "length")
	{
		return VariantPtr(new Variant(m_str.Length()));
	}
	return JsMethod::GetProperty(idx);
}

bool JsString::HasProperty(const String& idx) const
{
	return idx == "length" || JsMethod::HasProperty(idx);
}

IJsObjectPtr JsString::New()
{
	return IJsObjectPtr(new JsString(*this));
}

bool JsString::IsNative() const
{
	return true;
}

StringPtr JsString::ToString() const
{
	return StringPtr(new String(m_str));
}

StringPtr JsString::TypeName() const
{
	return StringPtr(new String("String"));
}

VariantPtr JsString::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	if (!args.ElementAtRef(0)->IsUndefined())
	{
		m_str = *args.ElementAtRef(0)->ToString();
	}
	return VariantPtr(new Variant());
}

#if defined(DEBUG) || defined(_DEBUG)
void JsString::CheckMem() const
{
	m_str.CheckMem();
	JsMethod::CheckMem();
}

void JsString::ValidateMem() const
{
	m_str.ValidateMem();
	JsMethod::ValidateMem();
}
#endif
