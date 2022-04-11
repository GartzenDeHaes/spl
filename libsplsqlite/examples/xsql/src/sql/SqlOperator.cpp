#include "xsql/sql/dom/SqlOperator.h"
#include "xsql/sql/SqlSyntaxException.h"

SqlOperator::~SqlOperator()
{

}

SqlOperatorPtr SqlOperator::Parse(SqlLex* lex)
{
	switch (lex->Token())
	{
		case SQL_AND:
			lex->Match(SQL_AND);
			return SqlOperatorPtr(new SqlOpAnd());
		case SQL_EQ:
			lex->Match(SQL_EQ);
			return SqlOperatorPtr(new SqlOpEqual());
		case SQL_GT:
			lex->Match(SQL_GT);
			return SqlOperatorPtr(new SqlOpGt());
		case SQL_GTEQ:
			lex->Match(SQL_GTEQ);
			return SqlOperatorPtr(new SqlOpGtEq());
		case SQL_LT:
			lex->Match(SQL_LT);
			return SqlOperatorPtr(new SqlOpLt());
		case SQL_LTEQ:
			lex->Match(SQL_LTEQ);
			return SqlOperatorPtr(new SqlOpLtEq());
		case SQL_NEQ:
			lex->Match(SQL_NEQ);
			return SqlOperatorPtr(new SqlOpNotEq());
		case SQL_OR:
			lex->Match(SQL_OR);
			return SqlOperatorPtr(new SqlOpOr());
		case SQL_IN:
			lex->Match(SQL_IN);
			return SqlOperatorPtr(new SqlOpIn(lex));
		case SQL_IS:
			lex->Match(SQL_IS);
			if (lex->Token() == SQL_NOT)
			{
				lex->Match(SQL_NOT);
				return SqlOperatorPtr(new SqlOpNotEq());
			}
			return SqlOperatorPtr(new SqlOpEqual());
		default:
			return SqlOperatorPtr();
	}
}

#ifdef DEBUG
void SqlOperator::ValidateMem() const
{

}

void SqlOperator::CheckMem() const
{

}
#endif

SqlOpAnd::~SqlOpAnd()
{
}

SqlOpEqual::~SqlOpEqual()
{
}

SqlOpGt::~SqlOpGt()
{
}

SqlOpGtEq::~SqlOpGtEq()
{
}

SqlOpLt::~SqlOpLt()
{
}

SqlOpLtEq::~SqlOpLtEq()
{
}

SqlOpNotEq::~SqlOpNotEq()
{
}

SqlOpOr::~SqlOpOr()
{
}

SqlOpIn::SqlOpIn(SqlLex* lex)
: m_terms()
{
	if (lex->Token() != SQL_LPAR)
	{
		throw new SqlSyntaxException("(", lex->Lexum(), lex->LineNumber());
	}

	while (lex->Token() != SQL_RPAR)
	{
		if (lex->IsEOF())
		{
			// this will throw a syntax exception
			lex->Match(SQL_RPAR);
		}

		m_terms.Add(ISelector::Parse(lex));
	}
}

SqlOpIn::~SqlOpIn()
{
}

#ifdef DEBUG
void SqlOpIn::ValidateMem() const
{
	SqlOperator::ValidateMem();
	m_terms.ValidateMem();
}

void SqlOpIn::CheckMem() const
{
	SqlOperator::CheckMem();
	m_terms.CheckMem();
}
#endif
