#ifndef _ixsqlcommand_h
#define _ixsqlcommand_h

#include <spl/Debug.h>
#include <spl/GUID.h>
#include <spl/Memory.h>

class ICommand : public spl::IMemoryValidate
{
protected:
	spl::Guid m_id;

public:
	inline ICommand() {}
	virtual ~ICommand();

	inline spl::Guid& ID()
	{
		return m_id;
	}

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

#endif
