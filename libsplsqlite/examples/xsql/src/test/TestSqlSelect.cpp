#include <spl/io/log/Log.h>

#ifdef DEBUG

#include "xsql/sql/SqlFactory.h"
#include "xsql/sql/SqlSelect.h"

using namespace spl;

static void _TestSqlSelectNull()
{
	{
		IDMLPtr sel(SqlFactory::Parse("SELECT 1;"));
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	Log::SWriteOkFail( "SqlSelect null ... " );
}

void XSqlTestSqlSelect()
{
	_TestSqlSelectNull();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}



#endif
