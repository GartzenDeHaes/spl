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
#ifndef _xpathparser_h
#define _xpathparser_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/Memory.h>
#include <spl/collection/List.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>
#include <spl/xml/xpath/private/XPathLex.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
	class XPathParser : public IMemoryValidate
	{
	private:
		XPathLex m_lex;
		Vector<XPathOperatorPtr> *m_out;
		List<String> m_exprStack;
		
	public:
		XPathParser();
		XPathParser(const XPathParser& parser);
		virtual ~XPathParser();

		XPathParser& operator =(const XPathParser& parser);

		Array<XPathOperatorPtr> Parse(const String& expr);

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif

	private:
		void NodeSpec();
		void NodePath();
		void MorePath();
		bool Axis();
		bool Predicate();
		bool MoreMorePath();
		bool Expr();
		bool MoreExpr();
		void LogOp();
		bool MoreLogOp();
		bool MoreRelOps();
		void RelOp();
		void Term();
		bool MoreTerms();
		void Factor();
		bool MoreFactors();
		bool PathExpr();
		bool ArgList();
		bool MoreArgs();
	};
}

#endif
