/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SPL is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SPL.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _jsmethod_h
#define _jsmethod_h

#include <spl/interp/JsObject.h>
#include <spl/interp/Program.h>

namespace spl
{
	class JsMethod;
	typedef RefCountPtrCast<JsMethod, JsObject, JsObjectPtr> JsMethodPtr;

	class JsMethod : public JsObject
	{
	protected:
		String m_src;
		ProgramPtr m_prog;

	public:
		JsMethod();	
		JsMethod(ProgramPtr prog);
		JsMethod(const JsMethod& obj);
		virtual ~JsMethod();

		virtual IJsObjectPtr New();

		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
		
		virtual VariantPtr GetProperty(const String& idx);
		virtual bool HasProperty(const String& idx) const;
		virtual void SetProperty(const String& idx, VariantPtr obj);

		virtual bool Equals( const IComparable& a ) const;
		virtual int Compare( const IComparable& a ) const;

		virtual int32 MajicNumber() const;
		virtual uint32 HashCode() const;

		virtual StringPtr ToString() const;
		virtual StringPtr TypeName() const;
		
		virtual int ArgumentCount();
		
		inline Program& GetProgram() { return *m_prog; }
		inline ProgramPtr GetProgramPtr() { return m_prog; }
		
		inline String& SourceCode() { return m_src; }
		
	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}
#endif
