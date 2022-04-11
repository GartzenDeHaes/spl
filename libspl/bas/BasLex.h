#ifndef _baslex_h
#define _baslex_h

#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/io/TextReader.h>

#include "Tokens.h"

#define LEX_TOKE_BUF 512

/* lex errors */
#define LEX_INVALID_NUMBER_FORMAT -1
#define LEX_UNEXPECTED_ESCAPE -2
#define LEX_UNTERMINATED_CHAR -3
#define LEX_UNTERMINATED_STRING -4
#define LEX_ERROR -5

using namespace spl;

class BasLex : public IMemoryValidate
{
	private:

	String _buffer;
	int _pos;
	int _lineno;
	int _token;
	char _lex[LEX_TOKE_BUF];

	protected:

	int Getch();
	int Ungetch(const int ch);
	int Getescape();
	int Isws( const int ch );
	void StripWS();

	public:

	BasLex();
	virtual ~BasLex();

	void SetInput(const char *lexum);

	int Next();

	inline int NextIgnoreEol()
	{
		int tok = Next();
		if (EOL == tok)
		{
			return Next();
		}

		return tok;
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

};

#endif

