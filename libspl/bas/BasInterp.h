#ifndef _basinterp_h
#define _basinterp_h

#include <spl/Memory.h>

#include "BasLex.h"

class BasInterp : public IMemoryValidate
{
	protected:

	

	public:

	BasInterp();
	virtual ~BasInterp();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

#endif
