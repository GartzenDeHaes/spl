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
#include <spl/Debug.h>
#include <spl/io/log/Log.h>
#include <spl/math/Math.h>
#include <spl/threading/Mutex.h>
#include <spl/threading/RWLock.h>
#include <spl/threading/Thread.h>

using namespace spl;

#if defined(DEBUG) || defined(_DEBUG)

class _SharedLockTestThread : public Thread
{
private:
	static volatile int m_sdata;

	volatile bool m_running;
	int m_loopcount;
	bool m_havelock;
	int m_mydata;
	RWLock *m_lock;
	bool m_reader;

	void Run();

public:
	_SharedLockTestThread(RWLock *lock, bool reader, int loopcount);
	virtual ~_SharedLockTestThread();

	void Stop();
};

volatile int _SharedLockTestThread::m_sdata = 0;

_SharedLockTestThread::_SharedLockTestThread(RWLock *lock, bool reader, int loopcount)
: m_lock(lock), m_reader(reader), m_mydata(0), m_havelock(false), m_running(false), m_loopcount(loopcount)
{
	Start();
}

_SharedLockTestThread::~_SharedLockTestThread()
{
	m_running = false;
	Join(3000);
}

void _SharedLockTestThread::Stop()
{
	m_running = false;
}

void _SharedLockTestThread::Run()
{
	m_running = true;
	while ( (m_running && m_loopcount-- > 0) || m_havelock )
	{
		if ( m_reader )
		{
			if ( m_havelock )
			{
				UNIT_ASSERT("data changed", m_mydata == m_sdata);
				m_lock->UnlockRead();
				m_havelock = false;
			}
			else
			{
				m_lock->LockRead();
				m_mydata = m_sdata;
				m_havelock = true;
			}
		}
		else
		{
			if ( m_havelock )
			{
				UNIT_ASSERT("data changed", m_mydata == m_sdata);
				m_lock->UnlockWrite();
				m_havelock = false;
			}
			else
			{
				m_lock->LockWrite();
				m_sdata++;
				m_mydata = m_sdata;
				m_havelock = true;
			}
		}
		Thread::Sleep((int)(Math::Random() * 50));
	}
}

static void _SharedLockTest1()
{
	RWLock thelock;
	_SharedLockTestThread t1(&thelock, true, 10);
	t1.Join();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest1.1");

	Log::SWriteOkFail( "RWLockTest1" );
}

static void _SharedLockTest2()
{
	RWLock thelock;
	_SharedLockTestThread t1(&thelock, true, 20);
	_SharedLockTestThread t2(&thelock, true, 20);
	t1.Join();
	t2.Join();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest2.1");

	Log::SWriteOkFail( "RWLockTest2" );
}

static void _SharedLockTest3()
{
	RWLock thelock;
	_SharedLockTestThread t1(&thelock, false, 15);
	t1.Join();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest3.1");

	Log::SWriteOkFail( "RWLockTest3" );
}

static void _SharedLockTest4()
{
	RWLock thelock;
	_SharedLockTestThread t1(&thelock, true, 40);
	_SharedLockTestThread t2(&thelock, false, 15);
	t1.Join();
	t2.Join();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest4.1");

	Log::SWriteOkFail( "RWLockTest4" );
}

static void _SharedLockTest5()
{
	RWLock thelock;
	_SharedLockTestThread t1(&thelock, true, 40);
	_SharedLockTestThread t2(&thelock, false, 15);
	_SharedLockTestThread t3(&thelock, true, 40);
	t1.Join();
	t2.Join();
	t3.Join();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWTest5.1");

	Log::SWriteOkFail( "RWLockTest5" );
}

void _RWLockTest(  )
{
	_SharedLockTest1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest1");

	_SharedLockTest2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest2");

	_SharedLockTest3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest3");

	_SharedLockTest4();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest3");

	_SharedLockTest5();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("RWLockTest3");
}

#endif
