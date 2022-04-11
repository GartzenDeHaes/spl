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
#include <spl/Char.h>
#include <spl/Exception.h>
#include <spl/Int32.h>
#include <spl/text/StringBuffer.h>
#include <spl/xml/xpath/private/XPathLex.h>

using namespace spl;

XPathLex::XPathLex()
:	m_tokens(), m_lexums(), m_pos(0), m_state(XPS_START), m_lexCharPoses()
{
}

XPathLex::XPathLex(const XPathLex& lex)
:	m_tokens(lex.m_tokens), m_lexums(lex.m_lexums), m_pos(lex.m_pos), m_state(lex.m_state), m_lexCharPoses(lex.m_lexCharPoses)
{
}

XPathLex::~XPathLex()
{
}

XPathLex& XPathLex::operator =(const XPathLex& lex)
{
	m_tokens = lex.m_tokens;
	m_lexums = lex.m_lexums;
	m_pos = lex.m_pos;
	m_state = lex.m_state;
	m_lexCharPoses = lex.m_lexCharPoses;

	return *this;
}

XPathLex::XPathLexToken XPathLex::Match(XPathLex::XPathLexToken token)
{
	if (! HasMoreTokens())
	{
		throw new IndexOutOfBoundsException("No more tokens");
	}
	if (token != m_tokens[m_pos++])
	{
		throw new InvalidArgumentException("Match failure");
	}
	if (! HasMoreTokens())
	{
		return XP_EOF;
	}
	return m_tokens[m_pos];
}

XPathLex::XPathLexToken XPathLex::GetRelativeToken(int distance) const
{
	distance += m_pos;
	if (distance >= m_tokens.Count() || 0 > distance)
	{
		return XP_EOF;
	}
	return m_tokens[distance];
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathLex::CheckMem() const
{
	m_lexums.CheckMem();
	m_tokens.CheckMem();
	m_lexCharPoses.CheckMem();
}

void XPathLex::ValidateMem() const
{
	m_lexums.ValidateMem();
	m_tokens.ValidateMem();
	m_lexCharPoses.ValidateMem();
}
#endif

void XPathLex::TokenizeString(StringBuffer& token, char ch, char termchar, int pos)
{
	if (ch == termchar)
	{
		m_tokens.Add(XP_STRING);
		m_lexums.Add(token.ToString());
		m_lexCharPoses.Add(pos - token.Length());
		token.SetLength(0);
		m_state = XPS_START;
	}
	else
	{
		token.Append(ch);
	}
}

void XPathLex::TokenizeChar2(StringBuffer& token, int& pos, char ch, char chChar2, XPathLexToken char2Token, XPathLexToken char1Token)
{
	if (chChar2 == ch)
	{
		m_tokens.Add(char2Token);
		token.Append(ch);
		m_lexums.Add(token.ToString());
		m_lexCharPoses.Add(pos - token.Length());
	}
	else
	{
		m_tokens.Add(char1Token);
		m_lexums.Add(token.ToString());
		pos--;
		m_lexCharPoses.Add(pos - token.Length());
	}
	token.SetLength(0);
	m_state = XPS_START;
}

void XPathLex::Tokenize(const String& text)
{
	m_tokens.Clear();
	m_lexums.Clear();
	m_lexCharPoses.Clear();
	m_state = XPS_START;
	m_pos = 0;

	StringBuffer token;

	for ( int x = 0; x < text.Length(); x++ )
	{
		char ch = text.CharAt(x);
		bool complete = false;

		switch( m_state )
		{
		case XPS_START:
			if (Char::IsWhiteSpace(ch))
			{
				break;
			}
			token.Append(ch);
			if (Char::IsDigit(ch))
			{
				m_state = XPS_NUMERIC;
				break;
			}
			else if (Char::IsLetter(ch) || '_' == ch)
			{
				m_state = XPS_NAME;
				break;
			}
			switch (ch)
			{
			case '(':
				m_tokens.Add(XP_LPAR);
				complete = true;
				break;
			case ')':
				m_tokens.Add(XP_RPAR);
				complete = true;
				break;
			case '[':
				m_tokens.Add(XP_LBRAC);
				complete = true;
				break;
			case ']':
				m_tokens.Add(XP_RBRAC);
				complete = true;
				break;
			case '.':
				m_state = XPS_DOT;
				break;
			case '@':
				m_tokens.Add(XP_AT);
				complete = true;
				break;
			case ',':
				m_tokens.Add(XP_COMMA);
				complete = true;
				break;
			case ':':
				m_state = XPS_COLON;
				break;
			case '\'':
				m_state = XPS_TICK;
				token.Clear();
				break;
			case '"':
				m_state = XPS_QUOTE;
				token.Clear();
				break;
			case '*':
				m_tokens.Add(XP_STAR);
				complete = true;
				break;
			case '/':
				m_state = XPS_SLASH;
				break;
			case '|':
				m_tokens.Add(XP_PIPE);
				complete = true;
				break;
			case '+':
				m_tokens.Add(XP_PLUS);
				complete = true;
				break;
			case '-':
				m_state = XPS_NEG;
				break;
			case '=':
				m_state = XPS_EQ;
				break;
			case '!':
				m_state = XPS_BANG;
				break;
			case '<':
				m_state = XPS_LT;
				break;
			case '>':
				m_state = XPS_GT;
				break;
			case '$':
				m_tokens.Add(XP_DOLLAR);
				complete = true;
				break;
			case ' ':
			case '\n':
			case '\r':
			case '\t':
				break;

			default:
				throw new InvalidArgumentException("Invalid char '" + *Char::ToString(ch) + "' at position " + *Int32::ToString(x));
			}
			if (complete)
			{
				m_lexums.Add(token.ToString());
				m_lexCharPoses.Add(x - token.Length());
				token.SetLength(0);
			}
			break;

		case XPS_NUMERIC:
			// int or float
			if ('.' == ch)
			{
				token.Append(ch);
				m_state = XPS_FLOAT;
				break;
			}
			else if (Char::IsWhiteSpace(ch) || ispunct(ch) || x == text.Length() - 1)
			{
				if (x != text.Length() - 1)
				{
					x--;
				}
				else
				{
					token.Append(ch);
				}
				m_tokens.Add(XP_INT);
				m_lexums.Add(token.ToString());
				m_lexCharPoses.Add(x - token.Length());
				token.SetLength(0);
				m_state = XPS_START;
				break;
			}
			else if (Char::IsDigit(ch))
			{
				token.Append(ch);
			}
			else if ('e' == ch || 'E' == ch)
			{
				token.Append(ch);
				m_state = XPS_FLOAT_EXP;
				break;
			}
			else
			{
				throw new InvalidArgumentException("Invalid char '" + *Char::ToString(ch) + "' at position " + *Int32::ToString(x));
			}
			break;

		case XPS_FLOAT:
			if (Char::IsWhiteSpace(ch) || ispunct(ch) || x == text.Length() - 1)
			{
				if (x != text.Length() - 1)
				{
					x--;
				}
				else
				{
					token.Append(ch);
				}
				m_tokens.Add(XP_FLOAT);
				m_lexums.Add(token.ToString());
				m_lexCharPoses.Add(x - token.Length());
				token.SetLength(0);
				m_state = XPS_START;
				break;
			}
			else if (Char::IsDigit(ch))
			{
				token.Append(ch);
			}
			else if ('e' == ch || 'E' == ch)
			{
				token.Append(ch);
				m_state = XPS_FLOAT_EXP;
				break;
			}
			else
			{
				throw new InvalidArgumentException("Invalid char '" + *Char::ToString(ch) + "' at position " + *Int32::ToString(x));
			}
			break;

		case XPS_FLOAT_EXP:
			if (Char::IsDigit(ch))
			{
				token.Append(ch);
			}
			if (!Char::IsDigit(ch) || x == text.Length() - 1)
			{
				if (x != text.Length() - 1)
				{
					x--;
				}
				else
				{
					token.Append(ch);
				}
				m_tokens.Add(XP_FLOAT);
				m_lexums.Add(token.ToString());
				m_lexCharPoses.Add(x - token.Length());
				token.SetLength(0);
				m_state = XPS_START;
				break;
			}			
			break;

		case XPS_NAME:
			if ( !IsNameChar(ch) || x == text.Length() - 1)
			{
				if (!IsNameChar(ch))
				{
					x--;
				}
				else
				{
					token.Append(ch);
				}
				m_tokens.Add(XP_LITERAL);
				m_lexums.Add(token.ToString());
				m_lexCharPoses.Add(x - token.Length());
				token.SetLength(0);
				m_state = XPS_START;
			}
			else
			{
				token.Append(ch);
			}
			break;

		case XPS_DOT:
			// dot or dotdot or number
			if (Char::IsNumber(ch))
			{
				token.Append(ch);
				m_state = XPS_FLOAT;
				break;
			}
			if (!Char::IsNumber(ch) || x == text.Length() - 1)
			{
				TokenizeChar2(token, x, ch, '.', XP_DOTDOT, XP_DOT);
			}
			break;

		case XPS_COLON:
			// colon or colon colon
			TokenizeChar2(token, x, ch, ':', XP_COLONCOLON, XP_COLON);
			break;

		case XPS_TICK:
			// string
			TokenizeString(token, ch, '\'', x);
			break;

		case XPS_QUOTE:
			// string
			TokenizeString(token, ch, '"', x);
			break;

		case XPS_SLASH:
			// slash or slashslash
			TokenizeChar2(token, x, ch, '/', XP_SLASHSLASH, XP_SLASH);
			break;
		
		case XPS_BANG:
			// !=
			if ('=' != ch)
			{
				throw new InvalidArgumentException("Invalid char '" + *Char::ToString(ch) + "' at position " + *Int32::ToString(x));
			}
			m_tokens.Add(XP_NEQ);
			token.Append(ch);
			m_lexums.Add(token.ToString());
			m_lexCharPoses.Add(x - token.Length());
			token.SetLength(0);
			m_state = XPS_START;			
			break;

		case XPS_LT:
			// < or <=
			TokenizeChar2(token, x, ch, '=', XP_LTEQ, XP_LT);
			break;

		case XPS_GT:
			// > or >=
			TokenizeChar2(token, x, ch, '=', XP_GTEQ, XP_GT);
			break;
	
		case XPS_EQ:
			// = or ==
			TokenizeChar2(token, x, ch, '=', XP_EQEQ, XP_EQ);
			break;

		case XPS_NEG:
			if (Char::IsDigit(ch))
			{
				token.Append(ch);
				m_state = XPS_NUMERIC;
			}
			else
			{
				m_tokens.Add(XP_MIN);
				m_lexums.Add(token.ToString());
				m_lexCharPoses.Add(x - token.Length());
				token.SetLength(0);
				x--;
				m_state = XPS_START;
			}
			break;

		default:
			throw new StateException("Internal error, invalid XPathLex state");
		}
	}
	
	if (XPS_NUMERIC == m_state)
	{
		m_tokens.Add(XP_INT);
		m_lexums.Add(token.ToString());
		m_lexCharPoses.Add(text.Length());
	}
	else if (XPS_NAME == m_state)
	{
		m_tokens.Add(XP_STRING);
		m_lexums.Add(token.ToString());
		m_lexCharPoses.Add(text.Length());
	}
	else if (XPS_SLASH == m_state)
	{
		m_tokens.Add(XP_SLASH);
		m_lexums.Add(token.ToString());
		m_lexCharPoses.Add(text.Length());
	}

	m_tokens.Add(XP_EOF);
	m_lexums.Add(StringPtr(new String()));
}
