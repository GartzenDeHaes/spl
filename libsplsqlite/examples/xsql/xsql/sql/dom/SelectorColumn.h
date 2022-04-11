#ifndef _selcolumn_h
#define _sqlcolumn_h

#include "xsql/sql/ColumnName.h"
#include "xsql/sql/dom/ISelector.h"

class SelectorColumn : public ISelector
{
private:
	ColumnName m_name;

public:
	SelectorColumn(SqlLex* lex);
	virtual ~SelectorColumn();

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

#endif
