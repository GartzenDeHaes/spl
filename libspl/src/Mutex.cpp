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
#include <spl/types.h>

#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#include <spl/Environment.h>
#include <spl/threading/Mutex.h>
#include <spl/threading/Event.h>
#include <spl/threading/RWLock.h>
#include <spl/Exception.h>
#include <spl/String.h>

using namespace spl;

Mutex::Mutex()
{
	m_timeoutMs = INFINITE;
	initialized = false;
	Init();
}

Mutex::Mutex(int timeoutMs)
{
	m_timeoutMs = timeoutMs;
	initialized = false;
	Init();
}

Mutex::~Mutex()
{
	if (initialized)
	{
#ifdef WIN32
		CloseHandle(mutex);
#else
		pthread_mutex_destroy( &mutex );
#endif
	}
}

void Mutex::Init()
{
	if (initialized)
	{
		throw new Exception("Mutex already initialized");
	}
#ifdef WIN32
	mutex = CreateMutex(NULL,FALSE,NULL);
	if (mutex == NULL)
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#else
	int ret;
	if ( (ret = pthread_mutex_init( &mutex, NULL )) != 0 )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
	initialized = true;
	return;
}

bool Mutex::Lock()
{
	if (!initialized)
	{
		throw new Exception("Mutex not initialized");
	}
#ifdef WIN32
	int timeout = m_timeoutMs == 0 ? INFINITE : m_timeoutMs;
	int ret;
	if ( ret = (WAIT_TIMEOUT == WaitForSingleObject(mutex, timeout)) )
	{
		return false;
	}
	if ( ret == WAIT_FAILED )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
	return true;
#else
	if ( 0 != m_timeoutMs )
	{
		// emulate timeout
		int ret = pthread_mutex_trylock( &mutex );
		if ( 0 != ret )
		{
			if ( EBUSY == ret )
			{
				sleep(m_timeoutMs/1000);
				ret = pthread_mutex_trylock( &mutex );
			}
			if ( EBUSY == ret )
			{
				return false;
			}
			if ( 0 != ret )
			{
				throw new IOException(Environment::LastErrorMessage());
			}
			return true;
		}
	}
	else
	{
		int ret = pthread_mutex_lock( &mutex );
		if ( 0 != ret )
		{
			if ( EINVAL == ret )
			{
				throw new IOException(Environment::LastErrorMessage());
			}
			else if ( 0 != ret )
			{
				throw new IOException(Environment::LastErrorMessage());
			}
			return true;
		}
	}
	return false;
#endif
}

void Mutex::Unlock()
{
	if (!initialized)
	{
		throw new Exception("Mutex not initialized");
	}
#ifdef _WIN32
	if (!ReleaseMutex(mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#else
	int ret;
	if ( 0 != (ret = pthread_mutex_unlock(&mutex)) )
	{
		if ( EPERM != ret )
		{
			throw new IOException(Environment::LastErrorMessage());
		}
	}
#endif
}

Event::Event()
{
#ifdef WIN32
	if ( NULL == (hevent = CreateEvent(NULL, 0, 0, NULL)) )
	{
		throw OutOfMemoryException();
	}
#else
	int ret;
	if ( 0 != (ret = pthread_mutex_init( &mtx, NULL )) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
	if ( 0 != (ret = pthread_cond_init( &cond, NULL )) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
	m_timeoutMs = INFINITE;
}

Event::Event( int timeoutMs )
{
#ifdef WIN32
	if ( NULL == (hevent = CreateEvent(NULL, 0, 0, NULL)) )
	{
		throw OutOfMemoryException();
	}
#else
	int ret = pthread_mutex_init( &mtx, NULL );
	if ( 0 != ret )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
	if ( 0 != (ret = pthread_cond_init( &cond, NULL )) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
	m_timeoutMs = timeoutMs;
}

void Event::SetTimeOut( int ms )
{
	m_timeoutMs = ms;
}

Event::~Event()
{
#ifdef WIN32
	CloseHandle(hevent);
#else
	pthread_mutex_destroy( &mtx );
	pthread_cond_destroy( &cond );
#endif
}

void Event::Wait()
{
#ifdef WIN32
	DWORD ret = WaitForSingleObject(hevent, m_timeoutMs);
	if (ret == WAIT_FAILED)
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#else
	int ret;

	if ( 0 != (ret = pthread_mutex_lock(&mtx)) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	if ( 0 != m_timeoutMs )
	{
		struct timespec ts;
		struct timeval tp;
		
		gettimeofday(&tp, NULL);
		ts.tv_sec  = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec += m_timeoutMs / 1000;
		
		ret = pthread_cond_timedwait(&cond, &mtx, &ts);
	}
	else
	{
		ret = pthread_cond_wait(&cond, &mtx);
	}
	if ( 0 != ret && ETIMEDOUT != ret )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
	if ( 0 != (ret = pthread_mutex_unlock( &mtx )) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#endif
}

void Event::Notify()
{
#ifdef WIN32
	if (!SetEvent(hevent))
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#else
	if (0 != pthread_cond_signal(&cond))
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
}

RWLock::RWLock()
#if defined(_WIN32) || defined(__TANDEM)
: m_stateMtx(), m_readLockCount(0), m_writeLockCount(0), m_waitingWriters(5000), m_waitingWritersCount(0), m_waitingReaders(5000), m_waitingReadersCount(0)
#endif
{
#if defined(_WIN32) || defined(__TANDEM)
#else
	if ( 0 != pthread_rwlock_init(&m_rwlock, NULL) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
}

RWLock::~RWLock()
{
#if defined(_WIN32) || defined(__TANDEM)
#else
	pthread_rwlock_destroy(&m_rwlock);
#endif
}

void RWLock::LockRead()
{
#if defined(_WIN32) || defined(__TANDEM)
	while ( true )
	{
		m_stateMtx.Lock();
		if ( m_waitingWritersCount > 0 || 0 != m_writeLockCount )
		{
			m_waitingReadersCount++;
			m_stateMtx.Unlock();
			m_waitingReaders.Wait();
			m_stateMtx.Lock();
			m_waitingReadersCount--;
			m_stateMtx.Unlock();
		}
		else
		{
			ASSERT(0 == m_writeLockCount);
			m_readLockCount++;
			m_stateMtx.Unlock();
			return;
		}
	}
#else
	if ( 0 != pthread_rwlock_rdlock(&m_rwlock) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
}

void RWLock::LockWrite()
{
#if defined(_WIN32) || defined(__TANDEM)
	while ( true )
	{
		m_stateMtx.Lock();
		if ( 0 != m_writeLockCount || m_waitingWritersCount > 0 || m_readLockCount != 0 )
		{
			m_waitingWritersCount++;
			m_stateMtx.Unlock();
			m_waitingWriters.Wait();
			m_stateMtx.Lock();
			m_waitingWritersCount--;
			m_stateMtx.Unlock();
		}
		else
		{
			ASSERT(0 == m_writeLockCount);
			m_writeLockCount++;
			m_stateMtx.Unlock();
			return;
		}
	}
#else
	if ( 0 != pthread_rwlock_wrlock(&m_rwlock) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
}

void RWLock::UnlockRead()
{
#if defined(_WIN32) || defined(__TANDEM)
	m_stateMtx.Lock();
	m_readLockCount--;
	ASSERT(m_readLockCount >= 0);
	ASSERT( 0 == m_writeLockCount );
	m_stateMtx.Unlock();

	if (0 == m_readLockCount && 0 != m_waitingWritersCount )
	{
		m_waitingWriters.Notify();
	}
#else
	if (0 != pthread_rwlock_unlock(&m_rwlock))
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
}

void RWLock::UnlockWrite()
{
#if defined(_WIN32) || defined(__TANDEM)
	ASSERT( 0 == m_readLockCount );

	m_stateMtx.Lock();
	ASSERT(m_writeLockCount == 1);
	m_writeLockCount--;
	if ( 0 != m_waitingWritersCount )
	{
		m_waitingWriters.Notify();
	}
	else if( 0 != m_waitingReadersCount )
	{
		m_waitingReaders.Notify();
	}
	m_stateMtx.Unlock();
#else
	if (0 != pthread_rwlock_unlock(&m_rwlock))
	{
		throw new IOException(Environment::LastErrorMessage());
	}
#endif
}
