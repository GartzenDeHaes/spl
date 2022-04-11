#ifndef _sqlfactory_h
#define _sqlfactory_h

#include <spl/String.h>
#include "xsql/sql/IDML.h"
#include "xsql/sql/dom/SqlExpr.h"
#include "xsql/sql/SqlLex.h"

class SqlFactory
{
private:
	inline SqlFactory() {}

public:
	static IDMLPtr Parse(spl::String sql);
};

SqlExprPtr _SqlParseColumnList(SqlLex& lex);

#endif
