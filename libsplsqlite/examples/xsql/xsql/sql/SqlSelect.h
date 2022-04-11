#ifndef _select_h
#define _select_h

#include <spl/collection/List.h>
#include "xsql/sql/IDML.h"
#include "xsql/sql/dom/SqlExpr.h"
#include "xsql/sql/SqlLex.h"

class SqlSelect : public IDML
{
protected:
	spl::List<SqlExpr> m_selects;

public:
	SqlSelect(SqlLex* lex);
	virtual ~SqlSelect();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
