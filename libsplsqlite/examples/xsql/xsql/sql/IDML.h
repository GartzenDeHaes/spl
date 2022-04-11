#ifndef _idml_h
#define _idml_h

#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/WeakReference.h>

class IDML;
typedef spl::RefCountPtr<IDML> IDMLPtr;
typedef spl::WeakReference<IDML, IDMLPtr> IDMLRef;

class IDML : public spl::IMemoryValidate
{
public:
	inline IDML() {}
	virtual ~IDML();

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#endif
};

#endif
