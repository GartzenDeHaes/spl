#ifndef _iselector_h
#define _iselector_h

#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/WeakReference.h>

#include "xsql/sql/SqlLex.h"

class ISelector;
typedef spl::RefCountPtr<ISelector> ISelectorPtr;
typedef spl::WeakReference<ISelector, ISelectorPtr> ISelectorRef;

class ISelector : public spl::IMemoryValidate
{
private:
	inline ISelector(const ISelector& sel) {}
	inline ISelector& operator = (const ISelector& sel) {}

protected:
	inline ISelector() {}

public:
	virtual ~ISelector();

	static ISelectorPtr Parse(SqlLex* lex);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
