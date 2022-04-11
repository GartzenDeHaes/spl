#include <spl/Debug.h>

#ifdef DEBUG

extern void XSqlTestSqlLex();
extern void XSqlTestSqlSelect();

void TestXSql()
{
	XSqlTestSqlLex();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	XSqlTestSqlSelect();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
