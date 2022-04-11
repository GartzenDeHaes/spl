#ifndef _selexpr_h
#define _selexpr_h

#include "xsql/sql/dom/ISelector.h"
#include "xsql/sql/dom/SqlExpr.h"
#include "xsql/sql/SqlLex.h"

class SelectorExpr : public ISelector
{
private:
	SqlExprPtr m_expr;

public:
	SelectorExpr(SqlLex* lex);
	virtual ~SelectorExpr();
};

#endif
