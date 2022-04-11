#include "xsql/sql/dom/SqlExpr.h"

SqlExpr::SqlExpr()
: m_left(), m_right(), m_op()
{

}

SqlExpr::SqlExpr(SqlLex* lex)
: m_left(), m_right(), m_op()
{
	m_left = ISelector::Parse(lex);
	if (m_left.IsNotNull())
	{
		m_op = SqlOperator::Parse(lex);
		if (m_op.IsNotNull())
		{
			m_left = ISelector::Parse(lex);
		}
	}
}

SqlExpr::SqlExpr(SqlExpr& exp)
: m_left(exp.m_left), m_right(exp.m_right), m_op(exp.m_op)
{
}

SqlExpr::~SqlExpr()
{
}

SqlExpr& SqlExpr::operator = (const SqlExpr& expr)
{
	m_left = expr.m_left;
	m_right = expr.m_right;
	m_op = expr.m_op;
	return *this;
}

#ifdef DEBUG
void SqlExpr::ValidateMem() const
{
	m_left.ValidateMem();
	m_op.ValidateMem();
	m_right.ValidateMem();
}

void SqlExpr::CheckMem() const
{
	m_left.CheckMem();
	m_op.CheckMem();
	m_right.CheckMem();
}
#endif
