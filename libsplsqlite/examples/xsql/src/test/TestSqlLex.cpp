#include "xsql/sql/SqlLex.h"

#ifdef DEBUG

#include <spl/io/log/Log.h>

using namespace spl;

static void _TestSqlLextNull()
{
	{
		SqlLex lex();
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	Log::SWriteOkFail( "SqlLex null ... " );
}

static void _TestSqlLexSimple()
{
	SqlLex lex("SELECT * FROM mytable WHERE X <= 1 ORDER BY Y;");
	UNIT_ASSERT("BOF", lex.Match(SQL_BOF));
	UNIT_ASSERT("SELECT", lex.Match(SQL_SELECT));
	UNIT_ASSERT("*", lex.Match(SQL_SPLAT));
	UNIT_ASSERT("FROM", lex.Match(SQL_FROM));
	UNIT_ASSERT("mytable", lex.Lexum() == "mytable");
	UNIT_ASSERT("mytable", lex.Match(SQL_ID));
	UNIT_ASSERT("WHERE", lex.Match(SQL_WHERE));
	UNIT_ASSERT("X", lex.Lexum() == "X");
	UNIT_ASSERT("X", lex.Match(SQL_ID));
	UNIT_ASSERT("<=", lex.Match(SQL_LTEQ));
	UNIT_ASSERT("1", lex.Lexum() == "1");
	UNIT_ASSERT("X", lex.Match(SQL_NUM));
	UNIT_ASSERT("ORDER", lex.Match(SQL_ORDER));
	UNIT_ASSERT("BY", lex.Match(SQL_BY));
	UNIT_ASSERT("Y", lex.Lexum() == "Y");
	UNIT_ASSERT("Y", lex.Match(SQL_ID));
	UNIT_ASSERT(";", lex.Match(SQL_SEMI));

	Log::SWriteOkFail( "SqlLex simple ... " );
}

void XSqlTestSqlLex()
{
	_TestSqlLextNull();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestSqlLexSimple();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
