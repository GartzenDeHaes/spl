#include <spl/String.h>
#include "xsql/XSqlServer.h"

using namespace spl;

extern void TestXSql();

int main(int argc, const char** argv)
{
	if (argc == 2)
	{
		if (StrCmp(argv[1], "--test", 8) == 0)
		{
			TestXSql();
			return 0;
		}
	}
	if (argc != 1)
	{
		return 20;
	}

	return 0;
}
