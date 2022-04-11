#include "xsql/sql/dom/SelectorExpr.h"

SelectorExpr::SelectorExpr(SqlLex* lex)
: m_expr()
{
	lex->Match(SQL_LPAR);



	lex->Match(SQL_RPAR);
}

SelectorExpr::~SelectorExpr()
{
}
