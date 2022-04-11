#include "xsql/sql/SqlSelect.h"
#include "xsql/sql/SqlSyntaxException.h"

using namespace spl;

SqlSelect::SqlSelect(SqlLex* lex)
: m_selects()
{
	if (! lex->Match(SQL_SELECT))
	{
		throw new SqlSyntaxException("<select list>", "", 1);
	}

	
}

SqlSelect::~SqlSelect()
{

}

#ifdef DEBUG
void SqlSelect::ValidateMem() const
{

}

void SqlSelect::CheckMem() const
{

}
#endif
