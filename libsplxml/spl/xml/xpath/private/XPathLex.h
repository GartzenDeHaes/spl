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
#ifndef _xpathlex_h
#define _xpathlex_h

#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>
#include <spl/collection/Vector.h>

namespace spl
{
/**
 *	XPath lexical analyizer.
 *
 *	XPaths are short, so this class analyzes all of the tokens in one pass.
 */
class XPathLex : public IMemoryValidate
{
public:
	typedef enum _XPathLexToken
	{
		XP_UNKNOWN = 0,
		XP_EOF = 1,
		XP_STRING = 2,
		XP_LPAR = 3,
		XP_RPAR = 4,
		XP_LBRAC = 5,
		XP_RBRAC = 6,
		XP_DOT = 7,
		XP_DOTDOT = 8,
		XP_AT = 9,
		XP_COMMA = 10,
		XP_COLONCOLON = 11,
		XP_INT = 12,
		XP_FLOAT = 13,
		XP_SLASH = 14,
		XP_SLASHSLASH = 15,
		XP_PIPE = 16,
		XP_PLUS = 17,
		XP_MIN = 18,
		XP_EQ = 19,
		XP_NEQ = 20,
		XP_LT = 21,
		XP_LTEQ = 22,
		XP_GT = 23,
		XP_GTEQ = 24,
		XP_AND = 25,
		XP_OR = 26,
		XP_MOD = 27,
		XP_DIV = 28,
		XP_STAR = 29,
		XP_DOLLAR = 30,
		XP_COLON = 31,
		XP_EQEQ = 32,
		XP_LITERAL = 33
	} XPathLexToken;

private:
	enum XPathLexState
	{
		XPS_START = 0,
		XPS_NUMERIC = 1,
		XPS_NAME = 2,
		XPS_DOT = 3,
		XPS_COLON = 4,
		XPS_TICK = 5,
		XPS_QUOTE = 6,
		XPS_SLASH = 7,
		XPS_BANG = 8,
		XPS_LT = 9,
		XPS_GT = 10,
		XPS_FLOAT = 11,
		XPS_FLOAT_EXP = 12,
		XPS_NEG = 13,
		XPS_EQ = 14
	};

	Vector<XPathLexToken> m_tokens;
	Vector<StringPtr> m_lexums;
	Vector<int> m_lexCharPoses;
	int m_pos;
	XPathLexState m_state;

	void TokenizeString(StringBuffer& token, char ch, char termchar, int pos);
	void TokenizeChar2(StringBuffer& token, int& pos, char ch, char chChar2, XPathLexToken char2Token, XPathLexToken char1Token);

public:
	XPathLex();
	XPathLex(const XPathLex& lex);
	virtual ~XPathLex();

	XPathLex& operator =(const XPathLex& lex);

	/**
	 *	Tokenize the text; any existing tokens are replaced and the position is reset.
	 */
	void Tokenize(const String& text);

	XPathLexToken Match(XPathLex::XPathLexToken token);

	XPathLexToken GetRelativeToken(int distance) const;

	inline int Count() const { return m_tokens.Count(); }
	
	inline XPathLexToken CurrentToken() const { return m_tokens.ElementAt(m_pos); }

	inline const String& CurrentLexum() const { return *m_lexums.ElementAtRef(m_pos); }

	inline int CurrentLexumInputCharPos() const { return m_lexCharPoses[m_pos]; }

	inline bool HasMoreTokens() const { return m_pos < m_tokens.Count() - 1; }

	inline bool IsNameChar(char ch) const
	{
		return Char::IsLetterOrDigit(ch) ||
			'_' == ch || 
			'-' == ch ||
			'.' == ch;
	}
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}
#endif
