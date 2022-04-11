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
#include <spl/Int32.h>
#include <spl/interp/JsLex.h>
#include <spl/io/StringStream.h>

using namespace spl;

int JsLex::GetCh(  )
{
	int ch;
	
	if (m_pushBack != -1)
	{
		ch = m_pushBack;
		m_pushBack = -1;
	}
	else 
	{
		ch = m_text->ReadByte();
	}
	
	if ('\n' == ch)
	{
		m_lineNo++;
	}
	
	return ch;
}

void JsLex::UnGetCh( const int ch )
{
	ASSERT(m_pushBack == -1);
	
	if ('\n' == ch)
	{
		m_lineNo--;
	}
	m_pushBack = ch;
}

int JsLex::GetEscape(  )
{
	int ch = GetCh( );
	switch ( ch )
	{
	case 'n':
		ch = '\n';
		break;
	case 'r':
		ch = '\r';
		break;
	case 't':
		ch = '\t';
		break;
	case 'v':
		ch = '\v';
		break;
	case '\'':
		ch = '\'';
		break;
	case '\\':
		ch = '\\';
		break;
	default:
		ch = -1;
	}
	return ch;
}

void JsLex::StripWS()
{
	int ch = GetCh();

	while ( IsWs(ch) )
	{
		ch = GetCh();
	}
	UnGetCh( ch );
}

JsLex::JsLex()
:	m_text(),
	m_pushBack(-1),
	m_lineNo(1),
	m_lexum(),
	m_token(T_LEXERROR)
{
}

JsLex::JsLex(const String& text)
:	m_text(),
	m_lexum()
{
	Init(text);
}

JsLex::JsLex(spl::IStreamPtr stream)
:	m_text(),
	m_lexum()
{
	Init(stream);
}

JsLex::JsLex(const JsLex& lex)
:	m_text(lex.m_text),
	m_pushBack(lex.m_pushBack),
	m_lineNo(lex.m_lineNo),
	m_lexum(lex.m_lexum),
	m_token(lex.m_token)
{
}

JsLex::~JsLex()
{
}

JsLex& JsLex::operator =(const JsLex& lex)
{
	m_text = lex.m_text;
	m_pushBack = lex.m_pushBack;
	m_lineNo = lex.m_lineNo;
	m_lexum = lex.m_lexum;
	m_token = lex.m_token;
	
	return *this;
}

void JsLex::Init(const String& text)
{
	Init(StringStreamPtr(new StringStream(text)));
}

void JsLex::Init(spl::IStreamPtr text)
{
	m_text = text;
	m_pushBack = -1;
	m_lineNo = 1;
	m_lexum.Clear();
	m_token = T_LEXERROR;
	
	int ch = GetCh();
	if ('#' == ch)
	{
		//shebang support (#! /usr/local/bin/js)
		while ( '\r' != ch && '\n' != ch )
		{
			ch = GetCh();
		}
	}
	UnGetCh( ch );
}

JsLex::Token JsLex::Match(Token tok)
{
	if (tok != m_token)
	{
		throw new ParseException("Expected " + TokenLexum(tok) + " found " + m_lexum.ToString() + " on line " + Int32::ToString(m_lineNo));
	}
	return Next();
}

JsLex::Token JsLex::Next()
{
	m_lexum.Clear();
	
	StripWS();

	int ch = GetCh();
	if ( ch <= 0 )
	{
		m_token = T_JEOF;
		return m_token;
	}
	if ( isdigit( ch ) || ch == '.' )
	{
		Token toke = T_INT;

		if ( ch == '.' )
		{
			ch = GetCh();
			if ( ! isdigit( ch ) )
			{
				UnGetCh( ch );
				m_token = T_DOT;
				return m_token;
			}
			toke = T_REAL;
			UnGetCh( ch );
			ch = '.';
		}

		/*
		 * read some sort of number
		 */
		while ( isdigit(ch) )
		{
			m_lexum.Append((char)ch);
			ch = GetCh();
		}
		if ( ch == 'x' )
		{
			if ( m_lexum.Length() != 1 )
			{
				m_lexum.Set("Invalid number format.");
				m_token = T_LEXERROR;
				return m_token;
			}
			if ( m_lexum.CharAt(0) != '0' )
			{
				m_lexum.Set("Invalid number format.");
				m_token = T_LEXERROR;
				return m_token;
			}
			m_lexum.Append((char)ch);
			ch = GetCh();
			while ( isdigit(ch) )
			{
				m_lexum.Append((char)ch);
				ch = GetCh();
			}
			UnGetCh(ch);
			m_token = T_HEX;
			return m_token;
		}
		if ( ch == '.' )
		{
			toke = T_REAL;

			m_lexum.Append((char)ch);
			ch = GetCh();
			while ( isdigit(ch) )
			{
				m_lexum.Append((char)ch);
				ch = GetCh();
			}
		}
		if ( ch == 'e' || ch == 'E' )
		{
			toke = T_REAL;
			m_lexum.Append((char)ch);
			ch = GetCh();
			if ( ch == '+' || ch == '-' )
			{
				m_lexum.Append((char)ch);
				ch = GetCh();
			}
			while ( isdigit(ch) )
			{
				m_lexum.Append((char)ch);
				ch = GetCh();
			}
		}
		UnGetCh(ch);
		m_token = toke;
		return m_token;
	}
	else if ( isalpha( ch ) || ch == '_' )
	{
		/*
		 * identifier
		 */
		while ( isalnum( ch ) || ch == '_' )
		{
			m_lexum.Append((char)ch);
			ch = GetCh();
		}
		UnGetCh(ch);
		if ( m_lexum.Equals("var") )
		{
			m_token = T_VAR;
			return m_token;
		}
		if ( m_lexum.Equals("if") )
		{
			m_token = T_IF;
			return m_token;
		}
		if ( m_lexum.Equals("else") )
		{
			m_token = T_ELSE;
			return m_token;
		}
		if ( m_lexum.Equals("return") )
		{
			m_token = T_RETURN;
			return m_token;
		}
		if ( m_lexum.Equals("while") )
		{
			m_token = T_WHILE;
			return m_token;
		}
		if ( m_lexum.Equals("do") )
		{
			m_token = T_DO;
			return m_token;
		}
		if ( m_lexum.Equals("for") )
		{
			m_token = T_FOR;
			return m_token;
		}
		if ( m_lexum.Equals("break") )
		{
			m_token = T_BREAK;
			return m_token;
		}
		if ( m_lexum.Equals("static") )
		{
			m_token = T_STATIC;
			return m_token;
		}
		if ( m_lexum.Equals("public") )
		{
			m_token = T_PUBLIC;
			return m_token;
		}
		if ( m_lexum.Equals("private") )
		{
			m_token = T_PRIVATE;
			return m_token;
		}
		if ( m_lexum.Equals("protected") )
		{
			m_token = T_PROTECTED;
			return m_token;
		}
		if ( m_lexum.Equals("continue") )
		{
			m_token = T_CONTINUE;
			return m_token;
		}
		if ( m_lexum.Equals("switch") )
		{
			m_token = T_SWITCH;
			return m_token;
		}
		if ( m_lexum.Equals("case") )
		{
			m_token = T_CASE;
			return m_token;
		}
		if ( m_lexum.Equals("default") )
		{
			m_token = T_DEFAULT;
			return m_token;
		}
		if ( m_lexum.Equals("new") )
		{
			m_token = T_NEW;
			return m_token;
		}
		if ( m_lexum.Equals("delete") )
		{
			m_token = T_DELETE;
			return m_token;
		}
		if ( m_lexum.Equals("const") )
		{
			m_token = T_CONST;
			return m_token;
		}
		if ( m_lexum.Equals("null") )
		{
			m_token = T_JNULL;
			return m_token;
		}
		if ( m_lexum.Equals("try") )
		{
			m_token = T_TRY;
			return m_token;
		}
		if ( m_lexum.Equals("catch") )
		{
			m_token = T_CATCH;
			return m_token;
		}
		if ( m_lexum.Equals("finally") )
		{
			m_token = T_FINALLY;
			return m_token;
		}
		if ( m_lexum.Equals("throw") )
		{
			m_token = T_THROW;
			return m_token;
		}
		if ( m_lexum.Equals("true") )
		{
			m_token = T_YYTRUE;
			return m_token;
		}
		if ( m_lexum.Equals("false") )
		{
			m_token = T_YYFALSE;
			return m_token;
		}
		if ( m_lexum.Equals("function") || m_lexum.Equals("class") )
		{
			m_token = T_FUNCTION;
			return m_token;
		}
		if ( m_lexum.Equals("this") )
		{
			m_token = T_THIS;
			return m_token;
		}
		
		m_token = T_ID;
		return m_token;
	}
	switch ( ch )
	{
	case '\'':
		/*
		 * string
		 */
		while ( (ch = GetCh()) != -1 && ch != '\'' )
		{
			if ( ch == '\\' )
			{
				ch = GetEscape();
				if ( -1 == ch )
				{
					m_lexum.Set("Unexpected escape");
					m_token = T_LEXERROR;
					return m_token;
				}
			}
			m_lexum.Append((char)ch);
		}
		if ( ch == -1 )
		{
			m_lexum.Set("Unterminated escape");
			m_token = T_LEXERROR;
			return m_token;
		}
		m_token = T_STRING;
		return m_token;

	case '"':
		/*
		 * string
		 */
		while ( (ch = GetCh()) != -1 && ch != '"' )
		{
			if ( ch == '\\' )
			{
				ch = GetEscape();
				if ( -1 == ch )
				{
					m_lexum.Set("Unexpected escape");
					m_token = T_LEXERROR;
					return m_token;
				}
			}
			m_lexum.Append((char)ch);
		}
		if ( ch == -1 )
		{
			m_lexum.Set("Unterminated escape");
			m_token = T_LEXERROR;
			return m_token;
		}
		m_token = T_STRING;
		return m_token;

	case '|':
		/*
		 * or
		 */
		ch = GetCh();
		if ( '|' == ch )
		{
			m_token = T_OR;
			return m_token;
		}
		if ( '=' == ch )
		{
			m_token = T_OREQ;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_PIPE;
		return m_token;

	case '&':
		/*
		 * and
		 */
		ch = GetCh();
		if ( '&' == ch )
		{
			m_token = T_AND;
			return m_token;
		}
		if ( '=' == ch )
		{
			m_token = T_ANDEQ;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_AMPR;
		return m_token;

	case '{':
		m_token = T_LBRACE;
		return m_token;

	case '}':
		m_token = T_RBRACE;
		return m_token;

	case '(':
		m_token = T_LPAR;
		return m_token;

	case ')':
		m_token = T_RPAR;
		return m_token;

	case '[':
		m_token = T_LBRAC;
		return m_token;

	case ']':
		m_token = T_RBRAC;
		return m_token;

	case '!':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_ISNEQ;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_BANG;
		return m_token;

	case '=':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_ISEQUAL;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_ASSIGN;
		return m_token;

	case '+':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_PLUSEQ;
			return m_token;
		}
		if ( '+' == ch )
		{
			m_token = T_INC;
			return m_token;
		}
		UnGetCh( ch );
		m_token = T_PLUS;
		return m_token;

	case '-':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_MINUSEQ;
			return m_token;
		}
		if ( '-' == ch )
		{
			m_token = T_DEC;
			return m_token;
		}
		UnGetCh( ch );
		m_token = T_MINUS;
		return m_token;

	case '*':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_TIMESEQ;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_STAR;
		return m_token;

	case '/':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_DIVEQ;
			return m_token;
		}
		if ( '/' == ch )
		{
			/*
			 * EOL comment
			 */
			while ( ch != '\n' && ch > 0 )
			{
				ch = GetCh();
			}
			return Next();
		}
		if ( '*' == ch )
		{
			/*
			 *  Mult-line comment
			 */
			int opencount = 1;
			while ( opencount > 0 && ch > 0 )
			{
				while ( ch != '*' && ch != '/' && ch > 0 )
				{
					ch = GetCh();
				}
				if ( ch == '*' )
				{
					ch = GetCh();
					if ( ch == '/' )
					{
						opencount--;
					}
				}
				if ( ch == '/' )
				{
					ch = GetCh();
					if ( ch == '*' )
					{
						opencount++;
					}
				}
			}
			return Next();
		}
		UnGetCh(ch);
		m_token = T_SLASH;
		return m_token;

	case '%':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_MODEQ;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_MOD;
		return m_token;
	
	case '^':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_XOREQ;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_XOR;
		return m_token;
	
	case '~':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_COMPEQ;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_COMP;
		return m_token;

	case ':':
		ch = GetCh();
		if ( ':' == ch )
		{
			m_token = T_SCOPE;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_COLON;
		return m_token;

	case ';':
		m_token = T_SEMI;
		return m_token;

	case ',':
		m_token = T_COMMA;
		return m_token;

	case '<':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_LTEQ;
			return m_token;
		}
		else if ( '<' == ch )
		{
			m_token = T_LSHIFT;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_LT;
		return m_token;

	case '>':
		ch = GetCh();
		if ( '=' == ch )
		{
			m_token = T_GTEQ;
			return m_token;
		}
		else if ( '>' == ch )
		{
			m_token = T_RSHIFT;
			return m_token;
		}
		UnGetCh(ch);
		m_token = T_GT;
		return m_token;

	case -1:
		m_token = T_JEOF;
		return m_token;

	default:
		m_lexum.Set("Internal lexer error");
		m_token = T_LEXERROR;
		return m_token;
	}
#ifdef _WINDOWS
	m_token = T_LEXERROR;		/* make certian compilers happy */
	return m_token;
#endif
}

#if defined(DEBUG) || defined(_DEBUG)
void JsLex::CheckMem() const
{
	m_text.CheckMem();
	m_lexum.CheckMem();
}

void JsLex::ValidateMem() const
{
	m_text.ValidateMem();
	m_lexum.ValidateMem();
}
#endif

String JsLex::TokenLexum(Token tok)
{
	switch(tok)
	{
		case JsLex::T_LEXERROR: return "<<INTERNAL ERROR>>";
		case JsLex::T_IF: return "if";
		case JsLex::T_WHILE: return "while";
		case JsLex::T_FOR: return "for";
		case JsLex::T_VAR: return "var";
		case JsLex::T_RETURN: return "return";
		case JsLex::T_PUBLIC: return "public";
		case JsLex::T_PRIVATE: return "private";
		case JsLex::T_PROTECTED: return "protected";
		case JsLex::T_LBRACE: return "{";
		case JsLex::T_RBRACE: return "}";
		case JsLex::T_LPAR: return "(";
		case JsLex::T_RPAR: return ")";
		case JsLex::T_LBRAC: return "[";
		case JsLex::T_RBRAC: return "]";
		case JsLex::T_ID: return "<<ID>>";
		case JsLex::T_STRING: return "<<STRING>>";
		case JsLex::T_INT: return "<<INT>>";
		case JsLex::T_REAL: return "<<REAL>>";
		case JsLex::T_ISNEQ: return "!=";
		case JsLex::T_BANG: return "!";
		case JsLex::T_ISEQUAL: return "==";
		case JsLex::T_PLUSEQ: return "+=";
		case JsLex::T_MINUSEQ: return "-=";
		case JsLex::T_TIMESEQ: return "*=";
		case JsLex::T_DIVEQ: return "/=";
		case JsLex::T_MODEQ: return "%=";
		case JsLex::T_XOREQ: return "^=";
		case JsLex::T_ASSIGN: return "=";
		case JsLex::T_INC: return "++";
		case JsLex::T_DEC: return "--";
		case JsLex::T_PLUS: return "+";
		case JsLex::T_MINUS: return "-";
		case JsLex::T_STAR: return "*";
		case JsLex::T_SLASH: return "/";
		case JsLex::T_AND: return "&&";
		case JsLex::T_AMPR: return "@";
		case JsLex::T_OR: return "||";
		case JsLex::T_PIPE: return "|";
		case JsLex::T_XOR: return "^";
		case JsLex::T_MOD: return "%";
		case JsLex::T_SCOPE: return "::";
		case JsLex::T_COLON: return ":";
		case JsLex::T_SEMI: return ";";
		case JsLex::T_DOT: return ".";
		case JsLex::T_COMMA: return ",";
		case JsLex::T_JEOF: return "<<EOF>>";
		case JsLex::T_STATIC: return "static";
		case JsLex::T_ELSE: return "else";
		case JsLex::T_BREAK: return "break";
		case JsLex::T_LT: return "<";
		case JsLex::T_GT: return ">";
		case JsLex::T_LTEQ: return "<=";
		case JsLex::T_GTEQ: return ">=";
		case JsLex::T_COMP: return "~";
		case JsLex::T_COMPEQ: return "~=";
		case JsLex::T_DO: return "do";
		case JsLex::T_CONTINUE: return "continue";
		case JsLex::T_OREQ: return "|=";
		case JsLex::T_ANDEQ: return "&=";
		case JsLex::T_HEX: return "<<HEX INT>>";
		case JsLex::T_SWITCH: return "switch";
		case JsLex::T_CASE: return "case";
		case JsLex::T_NEW: return "new";
		case JsLex::T_DELETE: return "delete";
		case JsLex::T_CONST: return "const";
		case JsLex::T_JNULL: return "null";
		case JsLex::T_TRY: return "try";
		case JsLex::T_CATCH: return "catch";
		case JsLex::T_FINALLY: return "finally";
		case JsLex::T_YYFALSE: return "false";
		case JsLex::T_YYTRUE: return "true";
		case JsLex::T_DEFAULT: return "default";
		case JsLex::T_THROW: return "throw";
		case JsLex::T_LSHIFT: return "<<";
		case JsLex::T_RSHIFT: return ">>";
		case JsLex::T_FUNCTION: return "function";
	}
	return "";
}
