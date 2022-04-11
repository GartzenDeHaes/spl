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
#include <spl/threading/Thread.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifdef DEBUG

static volatile bool flag;
static volatile int count;

class _TestThread : public Thread
{
protected:
	int m_op;
	Mutex m_startedMutex;

public:
	_TestThread( int op )
	{
		m_op = op;
		m_startedMutex.Lock();
	}

	void Run()
	{
		m_startedMutex.Unlock();

		switch ( m_op )
		{
		case 1:
			Op1();
			break;
		case 2:
			Op2();
			break;
		case 3:
			Op3();
			break;
		}
	}

	void WaitStarted()
	{
		m_startedMutex.Lock();
		m_startedMutex.Unlock();
	}

private:
	void Op1()
	{
		flag = true;
	}

	void Op2()
	{
		for ( int x = 0; x < 1000; x++ )
		{
			count++;
		}
	}

	void Op3()
	{
		for ( int x = 0; x < 100000; x++ )
		{
			Thread::YYield();
			count++;
		}
	}
};

static bool singleTest1()
{
	int mcnt = Log::SMessageCount();
	flag = false;
	count = 0;
	_TestThread th(1);
	th.Start();
	th.Join();
	UNIT_ASSERT( "singleTest1", flag == true );
	return Log::SMessageCount() == mcnt;
}

static bool dualTest1()
{
	int mcnt = Log::SMessageCount();
	count = 0;
	flag = false;
	_TestThread th1(1);
	_TestThread th2(2);
	th2.Start();
	th1.Start();
	th2.Join();
	th1.Join();

	UNIT_ASSERT( "dualTest1", flag == true );
	UNIT_ASSERT( "dualTest1", count == 1000 );

	return Log::SMessageCount() == mcnt;
}

static bool killTest()
{
	int mcnt = Log::SMessageCount();
	flag = false;
	count = 0;
	_TestThread th(3);
	th.Start();
	th.WaitStarted();
	th.Kill();
	th.Join();
	UNIT_ASSERT( "killTest", count < 10000 );
	return Log::SMessageCount() == mcnt;
}

bool threadTestHarness()
{
	bool ret = singleTest1();
	ret |= dualTest1();
	ret |= killTest();
	return ret;
}

#endif

