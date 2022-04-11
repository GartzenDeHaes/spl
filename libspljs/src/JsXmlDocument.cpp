#include <spl/interp/JsFunctionDispatch.h>
#include <spl/interp/jsfunctions/JsXmlDocumentFn.h>
#include <spl/interp/JsXmlDocument.h>

using namespace spl;

JsXmlDocument::~JsXmlDocument()
{
}

void JsXmlDocument::SetFunctions()
{
	//SetProperty("charAt", VariantPtr(new Variant(RefCountPtr<IJsObject> (new JsFunctionDispatch(this, 0, JsStringCharAt)))));
}

VariantPtr JsXmlDocument::GetProperty(const String& idx)
{
	if (idx == "length")
	{
		return VariantPtr(new Variant());
	}
	return JsMethod::GetProperty(idx);
}

bool JsXmlDocument::HasProperty(const String& idx) const
{
	return idx == "length" || JsMethod::HasProperty(idx);
}

IJsObjectPtr JsXmlDocument::New()
{
	return IJsObjectPtr(new JsXmlDocument(*this));
}

bool JsXmlDocument::IsNative() const
{
	return true;
}

StringPtr JsXmlDocument::ToString() const
{
	return m_doc->ToString();
}

StringPtr JsXmlDocument::TypeName() const
{
	return StringPtr(new String("XmlDocument"));
}

VariantPtr JsXmlDocument::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	if (args.Count() > 0)
	{
		if (args.ElementAtRef(0)->TypeName() == "String")
		{
			*((JsXmlDocument *)isthis)->GetXmlDocument() = XmlDocument::ParseXml(*args.ElementAtRef(0)->ToString());
		}
		else if (args.ElementAtRef(0)->TypeName() == "Stream")
		{
		}
		else
		{
			Debug::Assert(false);
		}
	}
	return VariantPtr(new Variant());
}

#if defined(DEBUG) || defined(_DEBUG)
void JsXmlDocument::CheckMem() const
{
	m_doc.CheckMem();
	JsMethod::CheckMem();
}

void JsXmlDocument::ValidateMem() const
{
	m_doc.ValidateMem();
	JsMethod::ValidateMem();
}
#endif
