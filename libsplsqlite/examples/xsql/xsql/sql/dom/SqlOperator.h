#ifndef _sqloperator_h
#define _sqloperator_h

#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/WeakReference.h>
#include <spl/collection/List.h>

#include "xsql/sql/dom/ISelector.h"
#include "xsql/sql/SqlLex.h"

class SqlOperator;
typedef spl::RefCountPtr<SqlOperator> SqlOperatorPtr;
typedef spl::WeakReference<SqlOperator, SqlOperatorPtr> SqlOperatorRef;

class SqlOperator : public spl::IMemoryValidate
{
private:
	inline SqlOperator(const SqlOperator& op) {}
	inline SqlOperator& operator = (const SqlOperator& expr) { return *this; }

protected:
	inline SqlOperator() {}

public:
	virtual ~SqlOperator();

	static SqlOperatorPtr Parse(SqlLex* lex);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

class SqlOpAnd : public SqlOperator
{
public:
	inline SqlOpAnd() { }
	virtual ~SqlOpAnd();
};

class SqlOpEqual : public SqlOperator
{
public:
	inline SqlOpEqual() { }
	virtual ~SqlOpEqual();
};

class SqlOpGt : public SqlOperator
{
public:
	inline SqlOpGt() { }
	virtual ~SqlOpGt();
};

class SqlOpGtEq : public SqlOperator
{
public:
	inline SqlOpGtEq() { }
	virtual ~SqlOpGtEq();
};

class SqlOpLt : public SqlOperator
{
public:
	inline SqlOpLt() { }
	virtual ~SqlOpLt();
};

class SqlOpLtEq : public SqlOperator
{
public:
	inline SqlOpLtEq() { }
	virtual ~SqlOpLtEq();
};

class SqlOpNotEq : public SqlOperator
{
public:
	inline SqlOpNotEq() { }
	virtual ~SqlOpNotEq();
};

class SqlOpOr : public SqlOperator
{
public:
	inline SqlOpOr() { }
	virtual ~SqlOpOr();
};

class SqlOpIn : public SqlOperator
{
private:
	spl::List<ISelectorPtr> m_terms;

public:
	SqlOpIn(SqlLex* lex);
	virtual ~SqlOpIn();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
