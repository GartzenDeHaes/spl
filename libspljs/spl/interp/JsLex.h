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
#ifndef _jslex_h
#define _jslex_h

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/io/IStream.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>

namespace spl
{
	class JsLex : public IMemoryValidate
	{
	public:
		typedef enum _Token
		{
			T_LEXERROR = 0,
			T_IF = 257,
			T_WHILE = 258,
			T_FOR = 259,
			T_VAR = 260,
			T_RETURN = 261,
			T_PUBLIC = 263,
			T_PRIVATE = 264,
			T_PROTECTED = 265,
			T_LBRACE = 266,
			T_RBRACE = 267,
			T_LPAR = 268,
			T_RPAR = 269,
			T_LBRAC = 270,
			T_RBRAC = 271,
			T_ID = 272,
			T_STRING = 274,
			T_INT = 275,
			T_REAL = 276,
			T_ISNEQ = 277,
			T_BANG = 278,
			T_ISEQUAL = 279,
			T_PLUSEQ = 280,
			T_MINUSEQ = 281,
			T_TIMESEQ = 282,
			T_DIVEQ = 283,
			T_MODEQ = 284,
			T_XOREQ = 285,
			T_ASSIGN = 286,
			T_INC = 287,
			T_DEC = 288,
			T_PLUS = 289,
			T_MINUS = 290,
			T_STAR = 291,
			T_SLASH = 293,
			T_AND = 294,
			T_AMPR = 295,
			T_OR = 296,
			T_PIPE = 297,
			T_XOR = 298,
			T_MOD = 299,
			T_SCOPE = 300,
			T_COLON = 301,
			T_SEMI = 302,
			T_DOT = 303,
			T_COMMA = 304,
			T_JEOF = 305,
			T_STATIC = 306,
			T_ELSE = 307,
			T_BREAK = 308,
			T_LT = 309,
			T_GT = 310,
			T_LTEQ = 311,
			T_GTEQ = 312,
			T_COMP = 313,
			T_COMPEQ = 314,
			T_DO = 315,
			T_CONTINUE = 316,
			T_OREQ = 317,
			T_ANDEQ = 318,
			T_HEX = 319,
			T_SWITCH = 320,
			T_CASE = 321,
			T_NEW = 322,
			T_DELETE = 323,
			T_CONST = 324,
			T_JNULL = 325,
			T_TRY = 326,
			T_CATCH = 327,
			T_FINALLY = 329,
			T_YYFALSE = 330,
			T_YYTRUE = 331,
			T_DEFAULT = 332,
			T_THROW = 333,
			T_LSHIFT = 334,
			T_RSHIFT = 335,
			T_FUNCTION = 336,
			T_THIS = 337
		} Token;

	private:
		spl::IStreamPtr m_text;
		int m_pushBack;
		int m_lineNo;
		StringBuffer m_lexum;
		Token m_token;
		
		inline bool IsWs(const int ch)
		{
			return ! (isalnum(ch) || ispunct( ch ) || ch == -1);
		}
		
		int GetCh();
		void UnGetCh(const int ch);
		int GetEscape();
		void StripWS();
		
		static String TokenLexum(Token tok);
		
	public:
		JsLex();
		JsLex(spl::IStreamPtr stream);
		JsLex(const String& text);
		JsLex(const JsLex& lex);
		virtual ~JsLex();
	  
		JsLex& operator =(const JsLex& lex);
		
		void Init(const String& text);
		void Init(spl::IStreamPtr text);
		
		Token Match(Token tok);
		Token Next();
		
		inline Token CurrentToken() const { return m_token; }
		inline const StringBuffer& Lexum() { return m_lexum; }
		
		inline int LineNumber() const { return m_lineNo; }
		
		inline bool IsEOF() const { return JsLex::T_JEOF == m_token; }

	#if defined(DEBUG) || defined(_DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}
#endif
