#ifndef _jsdate_h
#define _jsdate_h

#include <spl/DateTime.h>
#include <spl/interp/JsMethod.h>

namespace spl
{
	class JsDate : public JsMethod
	{
	private:
		DateTime m_date;
		
		void SetFunctions();
		
	public:
		inline JsDate()
		: m_date()
		{
			SetFunctions();
		}

		inline JsDate(const JsDate& obj)
		: JsMethod(obj), m_date(obj.m_date)
		{
			SetFunctions();
		}

		virtual ~JsDate();
		
		inline JsDate(const DateTime dtm)
		: m_date(dtm)
		{
			SetFunctions();
		}

		virtual IJsObjectPtr New();
		
		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;

		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		
		inline DateTime GetDateTime() const { return m_date; }	

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}
#endif
