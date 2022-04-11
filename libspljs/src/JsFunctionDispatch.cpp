#include <spl/interp/JsFunctionDispatch.h>

using namespace spl;

JsFunctionDispatch::JsFunctionDispatch
(
	JsMethod *reallyThis,
	int argCount, 
	VariantPtr (*call)(JsMethod *isthis, Vector<VariantPtr>& args)
)
: m_call(call), m_reallyThis(reallyThis)
{
}

JsFunctionDispatch::~JsFunctionDispatch()
{
}

VariantPtr JsFunctionDispatch::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	return m_call(isthis, args);
}

bool JsFunctionDispatch::IsNative() const
{
	return true;
}
