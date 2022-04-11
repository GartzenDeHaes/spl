#include "xsql/sql/dom/SelectorLiteral.h"

SelectorLiteral::SelectorLiteral(SqlLex* lex)
: m_val(lex->Lexum())
{
	lex->Match(SQL_STRLIT);
}

SelectorLiteral::~SelectorLiteral()
{
}

#ifdef DEBUG
void SelectorLiteral::ValidateMem() const
{
	m_val.ValidateMem();
}

void SelectorLiteral::CheckMem() const
{
	m_val.CheckMem();
}
#endif
