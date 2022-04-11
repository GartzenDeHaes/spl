#ifndef _sqlexpr_h
#define _sqlexpr_h

#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/WeakReference.h>

#include "xsql/sql/dom/ISelector.h"
#include "xsql/sql/SqlLex.h"
#include "xsql/sql/dom/SqlOperator.h"

class SqlExpr;
typedef spl::RefCountPtr<SqlExpr> SqlExprPtr;
typedef spl::WeakReference<SqlExpr, SqlExprPtr> SqlExprRef;

class SqlExpr : public spl::IMemoryValidate
{
private:
	ISelectorPtr m_left;
	SqlOperatorPtr m_op;
	ISelectorPtr m_right;

public:
	SqlExpr();
	SqlExpr(SqlLex* lex);
	SqlExpr(SqlExpr& exp);
	virtual ~SqlExpr();

	SqlExpr& operator = (const SqlExpr& expr);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
