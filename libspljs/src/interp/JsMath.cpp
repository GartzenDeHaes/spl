#include <spl/math/Math.h>
#include <spl/interp/JsMath.h>
#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/jsfunctions/JsMathFn.h>

using namespace spl;

JsMath::~JsMath()
{
}

void JsMath::SetFunctions()
{
	Math::InitRandom();
	
	SetProperty("abs", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathAbs)))));
	SetProperty("acos", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathAcos)))));
	SetProperty("asin", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathAsin)))));
	SetProperty("atan", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathAtan)))));
	SetProperty("atan2", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 2, JsMathAtan2)))));
	SetProperty("ceil", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathCeil)))));
	SetProperty("cos", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathCos)))));
	SetProperty("exp", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathExp)))));
	SetProperty("floor", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathFloor)))));
	SetProperty("log", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathLog)))));
	SetProperty("max", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 2, JsMathMax)))));
	SetProperty("min", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 2, JsMathMin)))));
	SetProperty("pow", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathPow)))));
	SetProperty("random", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsMathRandom)))));
	SetProperty("round", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathRound)))));
	SetProperty("sin", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathSin)))));
	SetProperty("sqrt", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathSqrt)))));
	SetProperty("tan", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 1, JsMathTan)))));
}

VariantPtr JsMath::GetProperty(const String& idx)
{
	if (idx == "E")
	{
		return VariantPtr(new Variant(Math::E()));
	}
	if (idx == "LN2")
	{
		return VariantPtr(new Variant(Math::LN2()));
	}
	if (idx == "LN10")
	{
		return VariantPtr(new Variant(Math::LN10()));
	}
	if (idx == "LOG2E")
	{
		return VariantPtr(new Variant(Math::LOG2E()));
	}
	if (idx == "LOG10E")
	{
		return VariantPtr(new Variant(Math::LOG10E()));
	}
	if (idx == "PI")
	{
		return VariantPtr(new Variant(Math::PI()));
	}
	if (idx == "SQRT1_2")
	{
		return VariantPtr(new Variant(Math::SQRT2()));
	}
	if (idx == "SQRT2")
	{
		return VariantPtr(new Variant(Math::SQRT1_2()));
	}
	return JsMethod::GetProperty(idx);
}

bool JsMath::HasProperty(const String& idx) const
{
	return idx == "E" ||
		idx == "LN2" ||
		idx == "LN10" ||
		idx == "LOG2E" ||
		idx == "LOG10E" ||
		idx == "PI" ||
		idx == "SQRT1_2" ||
		idx == "SQRT2" ||
		JsMethod::HasProperty(idx);
}

IJsObjectPtr JsMath::New()
{
	return IJsObjectPtr(new JsMath(*this));
}

bool JsMath::IsNative() const
{
	return true;
}

StringPtr JsMath::ToString() const
{
	return StringPtr(new String());
}

StringPtr JsMath::TypeName() const
{
	return StringPtr(new String("Math"));
}

VariantPtr JsMath::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	return VariantPtr(new Variant());
}

#if defined(DEBUG) || defined(_DEBUG)
void JsMath::CheckMem() const
{
	JsMethod::CheckMem();
}

void JsMath::ValidateMem() const
{
	JsMethod::ValidateMem();
}
#endif

