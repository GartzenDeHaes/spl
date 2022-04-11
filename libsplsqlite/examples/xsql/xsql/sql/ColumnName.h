#ifndef _columnname_h
#define _columnname_h

#include <spl/Memory.h>
#include <spl/String.h>

#include "xsql/sql/SqlLex.h"

class ColumnName : public spl::IMemoryValidate
{
private:
	spl::String m_db;
	spl::String m_table;
	spl::String m_col;

public:
	ColumnName();
	ColumnName(const ColumnName& name);
	ColumnName(spl::String db, spl::String table, spl::String col);
	virtual ~ColumnName();

	ColumnName& operator =(const ColumnName& name);

	static ColumnName Parse(SqlLex* lex);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
