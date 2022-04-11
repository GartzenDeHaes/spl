#include "xsql/sql/dom/ISelector.h"
#include "xsql/sql/dom/SelectorAvg.h"
#include "xsql/sql/dom/SelectorCount.h"
#include "xsql/sql/dom/SelectorExpr.h"
#include "xsql/sql/dom/SelectorLiteral.h"
#include "xsql/sql/dom/SelectorNum.h"
#include "xsql/sql/dom/SelectorSum.h"

ISelector::~ISelector()
{
}

ISelectorPtr ISelector::Parse(SqlLex* lex)
{
	if(lex->Token() == SQL_NUM)
	{
		return ISelectorPtr(new SelectorNum(lex));
	}
	if (lex->Token() == SQL_STRLIT)
	{
		return ISelectorPtr(new SelectorLiteral(lex));
	}
	if (lex->Token() == SQL_LPAR)
	{
		return ISelectorPtr(new SelectorExpr(lex));
	}
	if (lex->Token() == SQL_ID)
	{
		if (lex->Lexum().EqualsIgnoreCase("AVG"))
		{
			return ISelectorPtr(new SelectorAvg(lex));
		}
		if (lex->Lexum().EqualsIgnoreCase("COUNT"))
		{
			return ISelectorPtr(new SelectorCount(lex));
		}
		if (lex->Lexum().EqualsIgnoreCase("SUM"))
		{
			return ISelectorPtr(new SelectorSum(lex));
		}
	}
	return ISelectorPtr();
}

#ifdef DEBUG
void ISelector::ValidateMem() const
{

}

void ISelector::CheckMem() const
{

}
#endif
