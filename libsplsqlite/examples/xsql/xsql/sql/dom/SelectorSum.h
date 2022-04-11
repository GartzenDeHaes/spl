#ifndef _selsum_h
#define _selsum_h

#include "xsql/sql/dom/ISelectorAggregate.h"

class SelectorSum : public ISelectorAggregate
{
public:
	SelectorSum(SqlLex* lex);
	virtual ~SelectorSum();
};

#endif
