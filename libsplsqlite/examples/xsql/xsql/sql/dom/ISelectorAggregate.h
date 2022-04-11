#ifndef _iselaggr_h
#define _iselaggr_h

#include "xsql/sql/dom/SqlExpr.h"
#include "xsql/sql/dom/ISelector.h"
#include "xsql/sql/SqlLex.h"

class ISelectorAggregate : public ISelector
{
private:
	SqlExprPtr m_expr;

protected:
	ISelectorAggregate();
	ISelectorAggregate(const char* keyword, SqlLex* lex);

public:
	virtual ~ISelectorAggregate();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
