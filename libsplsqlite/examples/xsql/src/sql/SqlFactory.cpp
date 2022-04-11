#include "xsql/sql/SqlSyntaxException.h"
#include "xsql/sql/SqlFactory.h"
#include "xsql/sql/SqlLex.h"
#include "xsql/sql/SqlSelect.h"

using namespace spl;

IDMLPtr SqlFactory::Parse(String sql)
{
	SqlLex lex(sql);

	if (! lex.Next())
	{
		return IDMLPtr();
	}

	if (lex.Token() == SQL_CREATE)
	{
	}
	else if (lex.Token() == SQL_SELECT)
	{
		IDMLPtr dml(new SqlSelect(&lex));

		if (lex.Token() != SQL_EOF)
		{

		}

		return dml;
	}
	else if (lex.Token() == SQL_INSERT)
	{

	}
	else if (lex.Token() == SQL_UPDATE)
	{

	}
	else if (lex.Token() == SQL_DELETE)
	{

	}
	else if (lex.Token() == SQL_BEGIN)
	{

	}
	else if (lex.Token() == SQL_COMMIT)
	{

	}
	else if (lex.Token() == SQL_ABORT)
	{

	}
	else
	{
		throw new SqlSyntaxException("<<CREATE, SELECT, ...>>", lex.Lexum(), lex.LineNumber());
	}

	return IDMLPtr();
}

SqlExprPtr _SqlParseColumnList(SqlLex& lex)
{
	return SqlExprPtr();
}
