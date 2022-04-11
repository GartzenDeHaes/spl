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
#ifndef _event_h
#define _event_h

#include <spl/types.h>
#include <spl/threading/Mutex.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup sync Synchronization Primitives
 * @ingroup threading
 * @{
 */

class Event;
typedef RefCountPtr<Event> EventPtr;
typedef WeakReference<Event, EventPtr> EventRef;

/** @brief Implements wait/notify (uses Event on windoze and a condition on posix. */
class Event
{
private:
	// forbid copy constructor
	inline Event(const Event& e) {}
	inline void operator =(const Event& e) {}

public:
	Event();
	Event( int timeout );
	virtual ~Event();
	void Wait();
	void Notify();
	void SetTimeOut( int ms );

protected:
	int m_timeoutMs;

#ifdef _WINDOWS
	HANDLE hevent;
#else
	pthread_cond_t cond;
	pthread_mutex_t mtx;
#endif
};

inline void TypeValidate(const Event& e)
{
}

inline void TypeValidate(const Event *e)
{
	if ( NULL != e )
	{
		ASSERT_MEM(e, sizeof(Event));
	}
}

inline void TypeCheckMem(const Event& e)
{
}

inline void TypeCheckMem(const Event *e)
{
	if ( NULL != e )
	{
		DEBUG_NOTE_MEM(e);
	}
}

/** @} */
}
#endif
