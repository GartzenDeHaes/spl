#include <spl/interp/JsConsole.h>
#include <spl/interp/jsfunctions/ConsoleWriteln.h>

using namespace spl;

JsConsole::JsConsole()
{
	SetProperty("writeln", VariantPtr(new Variant(RefCountPtr<IJsObject> (new ConsoleWriteln()))));
	SetProperty("readln", VariantPtr(new Variant(RefCountPtr<IJsObject> (new ConsoleReadln()))));
}

JsConsole::~JsConsole()
{
}

VariantPtr JsConsole::GetProperty(const String& idx)
{
	return JsObject::GetProperty(idx);
}

bool JsConsole::HasProperty(const String& idx) const
{
	return JsObject::HasProperty(idx);
}

void JsConsole::SetProperty(const String& idx, VariantPtr obj)
{
	JsObject::SetProperty(idx, obj);
}

StringPtr JsConsole::ToString() const
{
	return StringPtr(new String("[Console]"));
}

StringPtr JsConsole::TypeName() const
{
	return StringPtr(new String("Console"));
}
