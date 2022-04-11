#include "xsql/sql/dom/SelectorColumn.h"

SelectorColumn::SelectorColumn(SqlLex* lex)
: m_name()
{
	m_name = ColumnName::Parse(lex);
}

SelectorColumn::~SelectorColumn()
{
}

#ifdef DEBUG
void SelectorColumn::ValidateMem() const
{
	m_name.ValidateMem();
}

void SelectorColumn::CheckMem() const
{
	m_name.CheckMem();
}
#endif
