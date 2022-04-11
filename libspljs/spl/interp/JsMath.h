#ifndef _jsmath_h
#define _jsmath_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	class JsMath : public JsMethod
	{
	private:	
		void SetFunctions();
		
	public:
		inline JsMath()
		{
			SetFunctions();
		}
		
		inline JsMath(const JsMath& obj)
		: JsMethod(obj)
		{
			SetFunctions();
		}
		
		virtual ~JsMath();
		
		virtual IJsObjectPtr New();
		
		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;

		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;

		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		
	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}
#endif
