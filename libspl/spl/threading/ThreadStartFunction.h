/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SPL is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SPL.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _threadstartfunction_h
#define _threadstartfunction_h

#include <spl/threading/Thread.h>

namespace spl
{
/** @brief Start a thread on function.
 *
 */
class ThreadStartFunction : public Thread
{
private:
	void (*m_func)();

public:
	ThreadStartFunction(void (* func)())
	: m_func(func)
	{
	}

	virtual ~ThreadStartFunction();

private:
	virtual void Run();
};

inline void TypeValidate(ThreadStartFunction& t)
{
}

inline void TypeValidate(ThreadStartFunction *t)
{
	if ( NULL != t )
	{
		ASSERT_MEM(t, sizeof(ThreadStartFunction));
	}
}

inline void TypeCheckMem(ThreadStartFunction& t)
{
}

inline void TypeCheckMem(ThreadStartFunction *t)
{
	if ( NULL != t )
	{
		DEBUG_NOTE_MEM(t);
	}
}

}
#endif
