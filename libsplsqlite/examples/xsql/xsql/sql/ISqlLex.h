#ifndef _isqllex_h
#define _isqllex_h

#include <spl/Debug.h>
#include <spl/String.h>

#include "xsql/sql/SqlToken.h"

class ISqlLex : public spl::IMemoryValidate
{
public:
	inline ISqlLex() {}
	virtual ~ISqlLex();

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#endif
};

#endif
