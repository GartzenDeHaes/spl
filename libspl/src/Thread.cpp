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

#include <spl/threading/Thread.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <spl/Exception.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifndef _WINDOWS
void *TheThread(void *param);
#endif

Thread::Thread()
: m_joinme(), m_hasrun(false)
{
	DEBUG_ENABLE_HEAP_LOCK();
	m_running = false;
}

#ifdef _WINDOWS
Thread::Thread(HANDLE threadhandle)
: m_joinme(), m_hasrun(false)
{
	m_running = false;
	m_threadhandle = threadhandle;
	m_threadid = 0;
}
#endif

#ifndef _WINDOWS
Thread::Thread(pthread_t thread)
: m_joinme(), m_hasrun(false)
{
	DEBUG_ENABLE_HEAP_LOCK();
	m_running = false;
	m_threadid = thread;
}
#endif

Thread::~Thread()
{
	if ( m_running )
	{
		Kill();
		m_running = false;
	}
#ifdef _WINDOWS
	if ( NULL != m_threadhandle && m_running )
	{
		CloseHandle(m_threadhandle);
		m_threadhandle = NULL;
		m_running = false;
	}
#else
	// do nothing
#endif
}

void Thread::Start()
{
	if (m_running)
	{
		throw new Exception("Thread has already started");
	}

#ifdef _WINDOWS
	m_threadhandle = CreateThread(NULL,0,TheThread,this,0,&m_threadid);
	if (m_threadhandle == NULL)
	{
		throw new ThreadStartException();
	}
#else
	int ret = pthread_create(&m_threadid, NULL, TheThread, this);
	if (0 != ret )
	{
		throw new ThreadStartException();
	}
#endif
	/* Wait until 'running' is set */

	while (!m_running && !m_hasrun)
	{
		Thread::YYield();
	}
}

void Thread::Kill()
{
	Thread::YYield();
	if (!m_running)
	{
		return;
	}
	m_running = false;
	m_hasrun = true;
#ifdef _WINDOWS
	TerminateThread( m_threadhandle, 0 );
	m_threadhandle = NULL;
#else
	pthread_cancel( m_threadid );
	memset(&m_threadid, 0, sizeof(pthread_t));
#endif
}

bool Thread::IsRunning() const
{
	return m_running;
}

void Thread::Join()
{
	if (m_running)
	{
		m_joinme.Wait();
	}
}

bool Thread::Join( int timeoutMs )
{
	if (m_running)
	{
		m_joinme.SetTimeOut( timeoutMs );
		m_joinme.Wait();
	}
	return true;
}

void Thread::SetPriority( enum ThreadPriority prilv )
{
#ifdef _WINDOWS
	if ( Thread::PRIORITY_NORMAL == prilv )
	{
		SetThreadPriority(m_threadhandle, THREAD_PRIORITY_NORMAL);
	}
	else if ( Thread::PRIORITY_HI == prilv )
	{
		SetThreadPriority(m_threadhandle, THREAD_PRIORITY_ABOVE_NORMAL);
	}
	else
	{
		SetThreadPriority(m_threadhandle, THREAD_PRIORITY_BELOW_NORMAL);
	}
#else
	struct sched_param sc;
	if ( Thread::PRIORITY_NORMAL == prilv )
	{
		// BUG: This can't be right
		sc.sched_priority = (sched_get_priority_max(SCHED_FIFO) -
			sched_get_priority_min(SCHED_FIFO))/2;
		pthread_setschedparam(m_threadid, SCHED_FIFO, &sc);
	}
	else if ( Thread::PRIORITY_HI == prilv )
	{
		sc.sched_priority = sched_get_priority_max(SCHED_FIFO);
		pthread_setschedparam(m_threadid, SCHED_FIFO, &sc);
	}
	else
	{
		sc.sched_priority = sched_get_priority_min(SCHED_FIFO)+1;
		pthread_setschedparam(m_threadid, SCHED_FIFO, &sc);
	}
#endif
}

#ifdef _WINDOWS
DWORD WINAPI Thread::TheThread(void *param)
{
	Thread *thread = (Thread *)param;

	thread->m_running = true;

	try
	{
		thread->Run();
	}
	catch (Exception *ex)
	{
		Log::SWrite(ex);
		delete ex;
	}
	catch (OutOfMemoryException mex)
	{
		Log::SWrite(mex);
	}

	thread->m_hasrun = true;
	thread->m_running = false;
	thread->m_joinme.Notify();

	return 0;
}
#else
void *spl::TheThread(void *param)
{
	Thread *thread = (Thread *)param;

	thread->m_running = true;

	try
	{
		thread->Run();
	}
	catch (Exception *ex)
	{
		Log::SWrite(ex);
		delete ex;
	}
	catch (OutOfMemoryException mex)
	{
		Log::SWrite(mex);
	}

	thread->m_hasrun = true;
	thread->m_running = false;
	thread->m_joinme.Notify();

	return NULL;
}
#endif

void Thread::YYield()
{
#ifdef _WIN32
	::Sleep( 1 );
#else
	sched_yield();
#endif
}

void Thread::Sleep( long ms )
{
#ifdef _WIN32
	::Sleep( ms );
#else
	sleep( (unsigned int)(ms/1000.0) );
#endif
}

/*
Thread Thread::CurrentThread()
{
#if defined(_WIN32) || defined(WIN32)
	return Thread(GetCurrentThread());
#else
	return Thread(pthread_self());
#endif
}
*/

#if defined(DEBUG)
void Thread::CheckMem() const
{
}

void Thread::ValidateMem() const
{
}
#endif
