#include <spl/Int64.h>
#include <spl/Double.h>

#include "xsql/sql/dom/SelectorNum.h"
#include "xsql/sql/SqlSyntaxException.h"

using namespace spl;

SelectorNum::SelectorNum(SqlLex* lex)
: m_val()
{
	if (! Numeric::IsNumeric(lex->Lexum()))
	{
		throw new Exception("Internal error, should be a number");
	}

	if (Int64::IsInt(lex->Lexum()))
	{
		m_val = NumericPtr(new Int64(Int64::Parse(lex->Lexum())));
	}
	else
	{
		m_val = NumericPtr(new Double(Double::Parse(lex->Lexum())));
	}

	lex->Match(SQL_NUM);
}

SelectorNum::~SelectorNum()
{
}

#ifdef DEBUG
void SelectorNum::ValidateMem() const
{
	m_val.ValidateMem();
}

void SelectorNum::CheckMem() const
{
	m_val.CheckMem();
}
#endif

