#ifndef _sellit_h
#define _sellit_h

#include <spl/String.h>

#include "xsql/sql/dom/ISelector.h"
#include "xsql/sql/SqlLex.h"

class SelectorLiteral : public ISelector
{
private:
	spl::String m_val;

public:
	SelectorLiteral(SqlLex* lex);
	virtual ~SelectorLiteral();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
