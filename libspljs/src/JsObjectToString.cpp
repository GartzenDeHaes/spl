#include <spl/interp/jsfunctions/JsObjectToString.h>

using namespace spl;

JsObjectToString::JsObjectToString()
{
	m_prog->NoteArgument();
}

JsObjectToString::JsObjectToString(const JsObjectToString& obj)
{
}

JsObjectToString::~JsObjectToString()
{
}

bool JsObjectToString::IsNative() const
{
	return true;
}

VariantPtr JsObjectToString::Call(IJsObjectPtr isthis, Vector<VariantPtr>& args)
{
	ASSERT(args.Count() == 1);
	//if (! args.ElementAtRef(0)->IsUndefined())
	//{
	//	int radix = args.ElementAtRef(0)->ToInt32();
	//}
	return VariantPtr(new Variant(isthis->ToString()));
}
