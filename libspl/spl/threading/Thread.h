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
#ifndef JTHREAD_H
#define JTHREAD_H

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/DateTime.h>
#include <spl/RefCountPtr.h>

#ifdef _WINDOWS
#define _WINSOCKAPI_
#include <spl/cleanwindows.h>
#endif
#if defined(HAVE_SPTHREAD_H) || defined(_TANDEM)
#include <sys/types.h>
#include <spt_types.h>
#include <sys/time.h>
#include <wchar.h>
#include <signal.h>
#define _tal
#define _extensible
#include <spthread.h>
#include <spt_extensions.h>
/* also need LDFLAGS="-spthread" */
#endif
#if defined(HAVE_PTHREAD_H) && !defined(_TANDEM)
#include <pthread.h>
#endif

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/threading/Event.h>

namespace spl
{
/**
 * @defgroup threading Threading
 * @{
 */

/**
 * @defgroup threads Threads
 * @ingroup threading
 * @{
 */

#define ERR_JTHREAD_CANTINITMUTEX						-1
#define ERR_JTHREAD_CANTSTARTTHREAD						-2
#define ERR_JTHREAD_THREADFUNCNOTSET						-3
#define ERR_JTHREAD_NOTRUNNING							-4
#define ERR_JTHREAD_ALREADYRUNNING						-5

extern void*TheThread(void *);

class Thread;
typedef RefCountPtr<Thread> ThreadPtr;
typedef WeakReference<Thread, ThreadPtr> ThreadRef;

REGISTER_TYPEOF( 478, ThreadPtr );
REGISTER_TYPEOF( 479, ThreadRef );

/** @brief Execution thread.  To start a thread, inherit this class or use on of the delegates.
 *	@ref ThreadStartDelegate
 *	@ref ThreadStartFunction
 */
class Thread : public IMemoryValidate
{
private:
	// forbid copy constructor
	inline Thread(const Thread& t) {}
	inline void operator =(const Thread& t) {}

private:

#ifdef _WINDOWS
	static DWORD WINAPI TheThread(void *param);
	Thread(HANDLE threadhandle);

	HANDLE m_threadhandle;
	DWORD m_threadid;
#else
	// pthread type threads
	friend void* TheThread(void *param);
	Thread(pthread_t thread);

	pthread_t m_threadid;
#endif

	bool m_running;
	bool m_hasrun;

	Event m_joinme;

public:
	enum ThreadPriority
	{
		PRIORITY_NORMAL = 0,
		PRIORITY_HI = 1,
		PRIORITY_LOW = 2
	};

	Thread();
	virtual ~Thread();

	void Start();
	void Kill();
	bool IsRunning() const;
	void Join();
	bool Join( int timeoutMs );
	void SetPriority( enum ThreadPriority prilv );

	virtual void Run() = 0;

	static void YYield();
	static void Sleep( long ms );

#if defined(DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

REGISTER_TYPEOF( 481, Thread );

/** @} */
/** @} */
}
#endif
