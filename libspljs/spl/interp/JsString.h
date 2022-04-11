#ifndef _jsstring_h
#define _jsstring_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	class JsString : public JsMethod
	{
	private:
		String m_str;
		
		void SetFunctions();
		
	public:
		inline JsString()
		: m_str()
		{
			SetFunctions();
		}
		
		inline JsString(const JsString& obj)
		: JsMethod(obj), m_str(obj.m_str)
		{
			SetFunctions();
		}
		
		virtual ~JsString();
		
		inline JsString(const String& str)
		: m_str(str)
		{
			SetFunctions();
		}

		virtual IJsObjectPtr New();
		
		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;

		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;

		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		
		inline const String& GetString() const { return m_str; }	

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}
#endif
