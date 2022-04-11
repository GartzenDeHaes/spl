#include "xsql/sql/dom/ISelectorAggregate.h"

ISelectorAggregate::ISelectorAggregate()
: m_expr()
{
}

ISelectorAggregate::ISelectorAggregate(const char* keyword, SqlLex* lex)
: m_expr()
{
	lex->Match(keyword);
	lex->Match(SQL_LPAR);

	m_expr = new SqlExpr(lex);

	lex->Match(SQL_RPAR);
}

ISelectorAggregate::~ISelectorAggregate()
{
}

#ifdef DEBUG
void ISelectorAggregate::ValidateMem() const
{
	m_expr.ValidateMem();
}

void ISelectorAggregate::CheckMem() const
{
	m_expr.CheckMem();
}
#endif
