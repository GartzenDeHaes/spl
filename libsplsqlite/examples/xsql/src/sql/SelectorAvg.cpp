#include "xsql/sql/dom/SelectorAvg.h"

SelectorAvg::SelectorAvg(SqlLex* lex)
: ISelectorAggregate("AVG", lex)
{
}

SelectorAvg::~SelectorAvg()
{
}
