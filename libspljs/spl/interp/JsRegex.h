#ifndef _jsregex_h
#define _jsregex_h

#include <spl/text/Regex.h>
#include <spl/interp/JsMethod.h>

namespace spl
{
	class JsRegExp : public JsMethod
	{
	private:
		Regex m_regex;
		
		void SetFunctions();
		
	public:
		inline JsRegExp()
		: m_regex()
		{
			SetFunctions();
		}

		inline JsRegExp(const JsRegExp& obj)
		: JsMethod(obj), m_regex(obj.m_regex)
		{
			SetFunctions();
		}

		virtual ~JsRegExp();
		
		virtual IJsObjectPtr New();
		
		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;

		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		
		inline Regex& GetRegex() { return m_regex; }
		inline int LastMatch() { return m_regex.LastMatch(); }

		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}

#endif
