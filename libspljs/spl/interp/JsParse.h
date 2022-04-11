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
#ifndef _jsparse_h
#define _jsparse_h

#include <spl/Memory.h>
#include <spl/collection/Hashtable.h>
#include <spl/interp/JsLex.h>
#include <spl/interp/JsObject.h>
#include <spl/interp/Program.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
	class JsParse : public IMemoryValidate
	{
	private:
		bool m_isDebugBuild;
		JsLex m_lex;
		int m_lastLineNo;
		ProgramPtr m_prog;
		
		List<RefCountPtr<Hashtable<String, int> > > m_locals;
		Vector<String> m_fnNameStk;

		JsLex::Token Match(JsLex::Token tok);
		
	public:
		JsParse(bool isDebugBuild = false);
		virtual ~JsParse();
		
		/** @brief Parse javascript source code.
		 *	@return A VariantPtr containing a JsMethod.
		 */
		ProgramPtr Parse(const String& src);

	#if defined(DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif

	private:
		void Stmts();
		void Stmt();
		void Function();
		void Expr();
		void MoreVar();
		void MoreIf();
		void MoreWhile();
		void MoreFor();
		void MoreSwitch();
		void MoreReturn();
		void MoreTry();
		int StmtBlock();
		void MoreMoreIf();
		void SwitchBlock(int pc, JsObject *disp);
		void ParamList();
		void LogOp();
		void MoreLogOps();
		void RelOp();
		void MoreRelOps();
		void Term();
		void MoreTerms();
		void Factor();
		void MoreFactors();
		void Single();
		void MoreSingles();
		void Literal();
		void ArgList();	
	};
}
#endif
