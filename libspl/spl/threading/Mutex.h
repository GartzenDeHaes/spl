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
#ifndef JMUTEX_H
#define JMUTEX_H

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>

#ifdef _WINDOWS
#define _WINSOCKAPI_
#include <spl/cleanwindows.h>
#endif

#ifdef HAVE_SPTHREAD_H
#include <sys/types.h>
#include <spt_types.h>
#include <sys/time.h>
#include <wchar.h>
#include <signal.h>
#include <spt_extensions.h>
/* also need LDFLAGS="-spthread" */
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef HAVE_SEMAPHORE_H
#include <semaphore.h>
#endif

#include <spl/Debug.h>
#include <spl/WeakReference.h>

#ifndef INFINITE
#define INFINITE 0
#endif

namespace spl
{
/** 
 * @defgroup sync Synchronization Primitives
 * @ingroup threading
 * @{
 */

class Mutex;
typedef RefCountPtr<Mutex> MutexPtr;
typedef WeakReference<Mutex, MutexPtr> MutexRef;

REGISTER_TYPEOF( 460, MutexPtr );
REGISTER_TYPEOF( 462, MutexRef );

/** @brief Mutually exclusive locking mechnism. */
class Mutex
{
private:
	// forbid copy constructor
	inline Mutex(const Mutex& m) {}
	inline void operator =(const Mutex& m) {}

private:

#ifdef _WINDOWS
	HANDLE mutex;
#else 
	// pthread mutex
	pthread_mutex_t mutex;
#endif 

	bool initialized;
	int m_timeoutMs;

	void Init();

public:
	Mutex();
	Mutex(int timeoutMs);
	virtual ~Mutex();

	bool Lock();
	void Unlock();

	inline bool IsInitialized() { return initialized; }

	///@brief Set the mutex lock timeout in milliseconds (use zero for infinite).
	inline void SetTimeout( int timeoutMs ) { m_timeoutMs = timeoutMs; }
};

inline void TypeValidate(const Mutex& mtx)
{
}

inline void TypeValidate(const Mutex *mtx)
{
	if ( NULL != mtx )
	{
		ASSERT_MEM(mtx, sizeof(Mutex));
	}
}

inline void TypeCheckMem(const Mutex& mtx)
{
}

inline void TypeCheckMem(const Mutex *mtx)
{
	if ( NULL != mtx )
	{
		DEBUG_NOTE_MEM(mtx);
	}
}

REGISTER_TYPEOF( 464, Mutex );

/** @} */
}
#endif
