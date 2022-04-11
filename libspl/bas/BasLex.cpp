#include <spl/Debug.h>
#include <spl/Char.h>

#include "BasLex.h"
#include <stdio.h>

BasLex::BasLex()
: _buffer()
{
}

BasLex::~BasLex()
{
}

void BasLex::SetInput( const char *lexum )
{
	_buffer.Set(lexum);

	_lex[0] = '\0';
	_lineno = 1;
	_pos = 0;
}

int BasLex::Getch()
{
	int ch;
	if ( _pos >= _buffer.Length() )
	{
		ASSERT( _pos == _buffer.Length() );
		ASSERT( _buffer.CharAt(_pos) == '\0' );
		return -1;
	}
	ch = _buffer.CharAt(_pos++);
	if ( '\n' == ch )
	{
		_lineno++;
	}
	return ch;
}

int BasLex::Ungetch( const int ch )
{
	if ( _buffer.Length() == 0 )
	{
		ASSERT( ch == -1 );
		return -1;
	}
	if ( ch == -1 )
	{
		ASSERT( _pos == _buffer.Length() );
		return -1;
	}
	else if ( ch == '\n' )
	{
		_lineno--;
	}
	ASSERT( _buffer.CharAt(_pos-1) == ch );
	_pos--;
	return _buffer.CharAt(_pos-1);
}

int BasLex::Getescape()
{
	int ch = Getch();
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

int BasLex::Isws( const int ch )
{
	return ! (Char::IsLetterOrDigit(ch) || Char::IsPunctuation(ch) || ch == -1 || ch == '\n');
}

void BasLex::StripWS()
{
	int ch = Getch( );

	while ( Isws(ch) )
	{
		ch = Getch( );
	}
	Ungetch( ch );
}

int BasLex::Next()
{
	int ch;
	int _lexpos = 0;

	StripWS(  );

	ch = Getch(  );
	if ( ch <= 0 )
	{
		_token = JEOF;
		return _token;
	}
	if (ch == '\n')
	{
		_token = EOL;
		return _token;
	}
	if ( Char::IsDigit( ch ) || ch == '.' )
	{
		int toke = INT;

		if ( ch == '.' )
		{
			ch = Getch(  );
			if ( ! Char::IsDigit( ch ) )
			{
				Ungetch( ch );
				_token = DOT;
				return _token;
			}
			toke = REAL;
			ch = Ungetch( ch );
		}

		/*
		 * read some sort of number
		 */
		while ( Char::IsDigit(ch) )
		{
			_lex[_lexpos++] = (char)ch;
			ch = Getch(  );
		}
		if ( ch == 'x' )
		{
			if ( _lexpos != 1 )
			{
				sprintf( _lex, "%d", LEX_INVALID_NUMBER_FORMAT );
				_token = LEXERROR;
				return _token;
			}
			if ( _lex[0] != '0' )
			{
				sprintf( _lex, "%d", LEX_INVALID_NUMBER_FORMAT );
				_token = LEXERROR;
				return _token;
			}
			_lex[_lexpos++] = (char)ch;
			ch = Getch(  );
			while ( Char::IsDigit(ch) )
			{
				_lex[_lexpos++] = (char)ch;
				ch = Getch( );
			}
			_lex[_lexpos] = '\0';
			Ungetch( ch );
			_token = HEX;
			return _token;
		}
		if ( ch == '.' )
		{
			toke = REAL;

			_lex[_lexpos++] = (char)ch;
			ch = Getch( );
			while ( Char::IsDigit(ch) )
			{
				_lex[_lexpos++] = (char)ch;
				ch = Getch( );
			}
		}
		if ( ch == 'e' || ch == 'E' )
		{
			toke = REAL;
			_lex[_lexpos++] = (char)ch;
			ch = Getch( );
			if ( ch == '+' || ch == '-' )
			{
				_lex[_lexpos++] = (char)ch;
				ch = Getch( );
			}
			while ( isdigit(ch) )
			{
				_lex[_lexpos++] = (char)ch;
				ch = Getch( );
			}
		}
		Ungetch( ch );
		_lex[_lexpos] = '\0';
		_token = toke;
		return _token;
	}
	else if ( Char::IsLetterOrDigit( ch ) || ch == '_' )
	{
		/*
		 * identifier
		 */
		while ( isalnum( ch ) || ch == '_' )
		{
			_lex[_lexpos++] = (char)ch;
			ch = Getch( );
		}
		
		Ungetch( ch );
		_lex[_lexpos] = '\0';
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "rem") == 0 )
		{
			_token = VAR;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "var") == 0 )
		{
			_token = VAR;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "if") == 0 )
		{
			_token = IF;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "else") == 0 )
		{
			_token = ELSE;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "gosub") == 0 )
		{
			_token = RETURN;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "sub") == 0 )
		{
			_token = RETURN;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "function") == 0 )
		{
			_token = RETURN;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "return") == 0 )
		{
			_token = RETURN;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "while") == 0 )
		{
			_token = WHILE;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "do") == 0 )
		{
			_token = DO;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "for") == 0 )
		{
			_token = FOR;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "break") == 0 )
		{
			_token = BREAK;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "package") == 0 )
		{
			_token = CLASS;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "continue") == 0 )
		{
			_token = CONTINUE;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "switch") == 0 )
		{
			_token = SWITCH;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "case") == 0 )
		{
			_token = CASE;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "default") == 0 )
		{
			_token = DEFAULT;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "new") == 0 )
		{
			_token = NEW;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "null") == 0 )
		{
			_token = JNULL;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "try") == 0 )
		{
			_token = TRY;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "catch") == 0 )
		{
			_token = CATCH;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "finally") == 0 )
		{
			_token = FINALLY;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "throw") == 0 )
		{
			_token = THROW;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "true") == 0 )
		{
			_token = YYTRUE;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "false") == 0 )
		{
			_token = YYFALSE;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "begin") == 0 )
		{
			_token = RETURN;
			return _token;
		}
		if ( StrCmpNoCase(_lex, LEX_TOKE_BUF, "end") == 0 )
		{
			_token = RETURN;
			return _token;
		}
		_token = ID;
		return _token;
	}
	switch ( ch )
	{
	case '\'':
		/*
		 * char
		 */
		ch = Getch( );
		if ( ch == '\\' )
		{
			ch = Getescape( );
			if ( -1 == ch )
			{
				sprintf( _lex, "%d", LEX_UNEXPECTED_ESCAPE );
				_token = LEXERROR;
				return _token;
			}
		}
		_lex[_lexpos++] = (char)ch;
		_lex[_lexpos] = '\0';
		ch = Getch( );
		if ( ch != '\'' )
		{
			sprintf( _lex, "%d", LEX_UNTERMINATED_CHAR );
			_token = LEXERROR;
			return _token;
		}
		_token = CHAR;
		return _token;

	case '"':
		/*
		 * string
		 */
		while ( (ch = Getch( )) != -1 && ch != '"' && _lexpos < LEX_TOKE_BUF )
		{
			if ( ch == '\\' )
			{
				ch = Getescape( );
				if ( -1 == ch )
				{
					sprintf( _lex, "%d", LEX_UNEXPECTED_ESCAPE );
					_token = LEXERROR;
					return _token;
				}
			}
			_lex[_lexpos++] = (char)ch;
		}
		if ( _lexpos == LEX_TOKE_BUF || ch == -1 )
		{
			sprintf( _lex, "%d", LEX_UNTERMINATED_STRING );
			_token = LEXERROR;
			return _token;
		}
		_lex[_lexpos] = '\0';
		_token = STRING;
		return _token;

	case '|':
		/*
		 * or
		 */
		ch = Getch( );
		if ( '|' == ch )
		{
			_token = OR;
			return _token;
		}
		if ( '=' == ch )
		{
			_token = OREQ;
			return _token;
		}
		Ungetch( ch );
		_token = PIPE;
		return _token;

	case '&':
		/*
		 * and
		 */
		ch = Getch( );
		if ( '&' == ch )
		{
			_token = AND;
			return _token;
		}
		if ( '=' == ch )
		{
			_token = ANDEQ;
			return _token;
		}
		Ungetch( ch );
		_token = AMPR;
		return _token;

	case '{':
		_token = LBRACE;
		return _token;

	case '}':
		_token = RBRACE;
		return _token;

	case '(':
		_token = LPAR;
		return _token;

	case ')':
		_token = RPAR;
		return _token;

	case '[':
		_token = LBRAC;
		return _token;

	case ']':
		_token = RBRAC;
		return _token;

	case '!':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = ISNEQ;
			return _token;
		}
		Ungetch( ch );
		_token = BANG;
		return _token;

	case '=':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = ISEQUAL;
			return _token;
		}
		Ungetch( ch );
		_token = ASSIGN;
		return _token;

	case '+':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = PLUSEQ;
			return _token;
		}
		if ( '+' == ch )
		{
			_token = INC;
			return _token;
		}
		Ungetch( ch );
		_token = PLUS;
		return _token;

	case '-':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = MINUSEQ;
			return _token;
		}
		if ( '-' == ch )
		{
			_token = DEC;
			return _token;
		}
		Ungetch( ch );
		_token = MINUS;
		return _token;

	case '*':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = TIMESEQ;
			return _token;
		}
		Ungetch( ch );
		_token = STAR;
		return _token;

	case '/':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = DIVEQ;
			return _token;
		}
		if ( '/' == ch )
		{
			/*
			 * EOL comment
			 */
			while ( ch != '\n' && ch > 0 )
			{
				ch = Getch( );
			}
			_token = Next(  );
			return _token;
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
					ch = Getch( );
				}
				if ( ch == '*' )
				{
					ch = Getch( );
					if ( ch == '/' )
					{
						opencount--;
					}
				}
				if ( ch == '/' )
				{
					ch = Getch( );
					if ( ch == '*' )
					{
						opencount++;
					}
				}
			}
			_token = Next();
			return _token;
		}
		Ungetch( ch );
		_token = SLASH;
		return _token;

	case '%':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = MODEQ;
			return _token;
		}
		Ungetch( ch );
		_token = MOD;
		return MOD;
	
	case '^':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = XOREQ;
			return _token;
		}
		Ungetch( ch );
		_token = XOR;
		return _token;
	
	case '~':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = COMPEQ;
			return _token;
		}
		Ungetch( ch );
		_token = COMP;
		return _token;

	case ':':
		ch = Getch( );
		if ( ':' == ch )
		{
			_token = SCOPE;
			return _token;
		}
		Ungetch( ch );
		_token = COLON;
		return _token;

	case ';':
		_token = SEMI;
		return _token;

	case ',':
		_token = COMMA;
		return _token;

	case '<':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = LTEQ;
			return _token;
		}
		else if ( '<' == ch )
		{
			_token = LSHIFT;
			return _token;
		}
		Ungetch( ch );
		_token = LT;
		return _token;

	case '>':
		ch = Getch( );
		if ( '=' == ch )
		{
			_token = GTEQ;
			return _token;
		}
		else if ( '>' == ch )
		{
			_token = RSHIFT;
			return _token;
		}
		Ungetch( ch );
		_token = GT;
		return _token;

	case -1:
		_token = JEOF;
		return _token;

	default:
		sprintf( _lex, "%d", LEX_ERROR );
		_token = LEXERROR;
		return _token;
	}
#ifdef _WIN32
	_token = LEXERROR;		/* make certian compilers happy */
	return _token;
#endif
}

#ifdef DEBUG
void BasLex::ValidateMem() const
{
	_buffer.ValidateMem();
}

void BasLex::CheckMem() const
{
	_buffer.ValidateMem();
}
#endif

