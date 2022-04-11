#include "xsql/sql/ColumnName.h"
#include "xsql/sql/SqlSyntaxException.h"

using namespace spl;

ColumnName::ColumnName()
: m_db(), m_table(), m_col()
{
}

ColumnName::ColumnName(const ColumnName& name)
: m_db(name.m_db), m_table(name.m_table), m_col(name.m_col)
{
}

ColumnName::ColumnName(String db, String table, String col)
: m_db(db), m_table(table), m_col(col)
{

}

ColumnName::~ColumnName()
{

}

ColumnName& ColumnName::operator =(const ColumnName& name)
{
	m_db = name.m_db;
	m_table = name.m_table;
	m_col = name.m_col;

	return *this;
}

ColumnName ColumnName::Parse(SqlLex* lex)
{
	String s1(lex->Lexum());
	lex->Match(SQL_ID);

	if (lex->Token() == SQL_DOT)
	{
		lex->Match(SQL_DOT);

		String s2(lex->Lexum());
		lex->Match(SQL_ID);

		if (lex->Token() == SQL_DOT)
		{
			lex->Match(SQL_DOT);

			String s3(lex->Lexum());
			lex->Match(SQL_ID);

			return ColumnName(s1, s2, s3);
		}
		else
		{
			return ColumnName("", s1, s2);
		}
	}
	else
	{
		return ColumnName("", "", s1);
	}
}

#ifdef DEBUG
void ColumnName::ValidateMem() const
{
	m_db.ValidateMem();
	m_table.ValidateMem();
	m_col.ValidateMem();
}

void ColumnName::CheckMem() const
{
	m_db.CheckMem();
	m_table.CheckMem();
	m_col.CheckMem();
}
#endif
