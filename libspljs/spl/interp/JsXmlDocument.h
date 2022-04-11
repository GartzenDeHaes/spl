#ifndef _jsxmldocument_h
#define _jsxmldocument_h

#include <spl/interp/JsMethod.h>
#include <spl/xml/XmlDocument.h>

namespace spl
{
	class JsXmlDocument : public JsMethod
	{
	private:
		XmlDocumentPtr m_doc;
		
		void SetFunctions();
		
	public:
		inline JsXmlDocument()
		: m_doc(new XmlDocument())
		{
			SetFunctions();
		}
		
		inline JsXmlDocument(const JsXmlDocument& obj)
		: JsMethod(obj), m_doc(new XmlDocument())
		{
			*m_doc = *obj.m_doc;
			SetFunctions();
		}
		
		virtual ~JsXmlDocument();
		
		virtual IJsObjectPtr New();
		
		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;

		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;

		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		
		inline const XmlDocumentPtr& GetXmlDocument() const { return m_doc; }	

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}

#endif
