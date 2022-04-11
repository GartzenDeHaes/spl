#ifndef _selavg_h
#define _selavg_h

#include "xsql/sql/dom/ISelectorAggregate.h"
#include "xsql/sql/SqlLex.h"

class SelectorAvg : public ISelectorAggregate
{
public:
	SelectorAvg(SqlLex* lex);
	virtual ~SelectorAvg();
};

#endif
