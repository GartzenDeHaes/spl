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
#include <spl/threading/Mutex.h>
#include <spl/io/log/Log.h>
#include <spl/math/Math.h>
#include <spl/threading/Thread.h>

using namespace spl;

#ifdef DEBUG

class _TestMtxThread : public Thread
{
private:
	static volatile int g_val;

	Mutex& m_mtx;
	bool m_running;
	int m_trycount;
	int m_failcount;

public:
	_TestMtxThread(Mutex& mtx, int trycount);
	virtual ~_TestMtxThread();

	void Run();
	inline void Stop() { m_running = false; }
	inline int Failures() { return m_failcount; }
};

volatile int _TestMtxThread::g_val = 0;

_TestMtxThread::_TestMtxThread(Mutex& mtx, int trycount)
: m_mtx(mtx), m_trycount(trycount), m_failcount(0)
{
	Start();
}

_TestMtxThread::~_TestMtxThread()
{
}

void _TestMtxThread::Run()
{
	m_running = true;
	while ( m_running && m_trycount-- > 0 )
	{
		int val = Math::RandomRange(1000000);
		m_mtx.Lock();
		g_val = val;
		Thread::Sleep (100);
		if ( g_val != val )
		{
			m_failcount++;
		}
		m_mtx.Unlock();
	}
}

void _TestMutex()
{
	Math::InitRandom();
	Mutex mtx;
	_TestMtxThread t1(mtx, 20);
	_TestMtxThread t2(mtx, 20);
	_TestMtxThread t3(mtx, 20);

	t1.Join();
	t2.Join();
	t3.Join();

	UNIT_ASSERT("Mutex lock failures",
		t1.Failures() == 0 &&
		t2.Failures() == 0 &&
		t3.Failures() == 0);

	Log::SWriteOkFail( "Mutex test 1" );
}

#endif
