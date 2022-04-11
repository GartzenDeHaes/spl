#ifndef _selectornum_h
#define _selectornum_h

#include <spl/Numeric.h>

#include "xsql/sql/dom/ISelector.h"

class SelectorNum : public ISelector
{
private:
	spl::NumericPtr m_val;

public:
	SelectorNum(SqlLex* lex);
	virtual ~SelectorNum();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
