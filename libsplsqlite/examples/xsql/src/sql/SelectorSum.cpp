#include "xsql/sql/dom/SelectorSum.h"

SelectorSum::SelectorSum(SqlLex* lex)
: ISelectorAggregate("SUM", lex)
{
}

SelectorSum::~SelectorSum()
{
}

