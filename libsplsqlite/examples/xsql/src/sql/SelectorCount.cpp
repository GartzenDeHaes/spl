#include "xsql/sql/dom/SelectorCount.h"
#include "xsql/sql/SqlSyntaxException.h"

SelectorCount::SelectorCount(SqlLex* lex)
{
	lex->Match("COUNT");
	lex->Match(SQL_LPAR);
	if (lex->Token() != SQL_RPAR)
	{
		throw new SqlSyntaxException("COUNT does not accept arguments", lex->Lexum(), lex->LineNumber());
	}
	lex->Match(SQL_RPAR);
}

SelectorCount::~SelectorCount()
{
}
