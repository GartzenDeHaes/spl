#ifndef _selcount_h
#define _selcount_h

#include "xsql/sql/dom/ISelectorAggregate.h"
#include "xsql/sql/SqlLex.h"

class SelectorCount : public ISelectorAggregate
{
public:
	SelectorCount(SqlLex* lex);
	virtual ~SelectorCount();
};

#endif
