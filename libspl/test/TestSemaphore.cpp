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
#include <spl/threading/Event.h>
#include <spl/Int32.h>
#include <spl/threading/InterlockCounter.h>
#include <spl/threading/Mutex.h>
#include <spl/collection/Queue.h>
#include <spl/threading/Semaphore.h>
#include <spl/threading/ThreadStartDelegate.h>

using namespace spl;

#if defined(DEBUG) && !defined(__TANDEM)

class SemaphoreTester
{
private:
	volatile bool m_running;
	Event m_waitForIt;
	InterlockCounter m_count;
	Semaphore m_semaphore;
	Queue<int> m_q;

	ThreadStartDelegate<SemaphoreTester> m_thread1;
	ThreadStartDelegate<SemaphoreTester> m_thread2;
	ThreadStartDelegate<SemaphoreTester> m_thread3;
	ThreadStartDelegate<SemaphoreTester> m_thread4;

public:
	inline SemaphoreTester()
	:	m_thread1(),
		m_thread2(),
		m_thread3(),
		m_thread4(),
		m_count(),
		m_waitForIt(),
		m_semaphore(3),
		m_running(true),
		m_q()
	{		
		m_thread1.Set(this, &SemaphoreTester::RunMe);
		m_thread2.Set(this, &SemaphoreTester::RunMe);
		m_thread3.Set(this, &SemaphoreTester::RunMe);
		m_thread4.Set(this, &SemaphoreTester::RunMe);
		
		m_thread1.Start();
		m_thread2.Start();
		m_thread3.Start();
		m_thread4.Start();		
	}

	inline ~SemaphoreTester()
	{
	}

	inline int32 Count()
	{
		return m_count.Get();
	}

	inline void Stop()
	{
		m_running = false;
		m_waitForIt.Notify();

		m_thread1.Join(3000);
		m_thread2.Join(3000);
		m_thread3.Join(3000);
		m_thread4.Join(5000);
	}
	
	inline Queue<int>& Q() { return m_q; }

private:
	void RunMe();
};

void SemaphoreTester::RunMe()
{
	m_semaphore.Lock();

	m_count += 1;

	if (m_running)
	{
		m_q.Put(1);
		m_waitForIt.Wait();
	}

	m_semaphore.Unlock();
}
	
static void _TestSemaphore1()
{
	DateTime now = DateTime::Now();

	SemaphoreTester tst;
	//tst.Q().Get();
	//tst.Q().Get();
	//tst.Q().Get();

	while (tst.Count() < 3 && now.AddSeconds(10) > DateTime::Now())
	{
		Thread::YYield();
		Thread::Sleep(500);
	}

	StringPtr msg = String("Should be three threads running ").Cat(Int32::ToString(tst.Count())); 
	UNIT_ASSERT(msg->GetChars(), tst.Count() == 3);

	tst.Stop();

	UNIT_ASSERT("Should be four threads ran", tst.Count() == 4);

	Log::SWriteOkFail( "Semaphore test" );
}

void _TestSemaphore()
{
	_TestSemaphore1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif

