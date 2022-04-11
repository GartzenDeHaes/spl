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
#include <spl/Exception.h>
#include <spl/io/log/Log.h>
#include <spl/UnitTest.h>

using namespace spl;

#if defined (DEBUG) || defined(_DEBUG)

extern void TVectorTest();
extern void THashtableTest();
extern void TestObjectPool();
extern void TestCommandLine();
extern void TestStringBuffer();
extern void TestPacket();
extern void TestTString();
extern void TestStreams();
extern void TestRecordSet();
extern void _RWLockTest();
extern void _TestList();
extern void _TestVariant();
extern void _TestCommand();
extern void _TestRandom();
extern void _TestDate();
extern void _TestDateTime();
extern void _TestDes();
extern void _TestRSA();
extern void _TestMutex();
extern void _TestDesStream();
extern void _TestStateMachine();
extern void _TestMath();
extern void _TestNetwork();
extern void _TestConfigSettings();
extern void _TestMemoryPool();
extern void _TestDelimFile();
extern void _TestUri();
extern void _TestDecimal();
extern void _TestNumeric();
extern void _TestFile();
extern void _TestSemaphore();
extern void _TestStringTokenizer();
extern void _TestRefCountPtr();
//extern void _TestTermCap();
extern void _TestStringTable();
extern void _TestVarInterp();
extern void _TestBigInteger();
extern void _TestRegex();
extern void _TestEnv();
extern void _ExceptionTestHarness();
extern void RedblacktreeTestHarness();
extern void _testdebug();
extern void _TestPipe();
extern void _TestBitMaskr();

void TestHarness::RunTests()
{
	try
	{
		DEBUG_FREE_HEAP();

		_testdebug();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		TestTString();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestRefCountPtr();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestNumeric();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestDate();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestDateTime();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_ExceptionTestHarness();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestDecimal();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		//_TestRandom();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestMath();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		TVectorTest();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestList();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();
		DEBUG_VALIDATE();

		_TestVariant();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();
		DEBUG_VALIDATE();

		_TestBigInteger();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();
		DEBUG_VALIDATE();

		THashtableTest();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		TestStringBuffer();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestStringTokenizer();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestStringTable();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		RedblacktreeTestHarness();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		TestObjectPool();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		TestCommandLine();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestDes();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		//_TestRSA();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		TestStreams();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestDelimFile();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestFile();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestPipe();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestCommand();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();
		DEBUG_VALIDATE();

		TestRecordSet();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		TestPacket();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestUri();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestDesStream();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestStateMachine();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestNetwork();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestConfigSettings();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestMemoryPool();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		//_TestTermCap();
		//DEBUG_CLEAR_MEM_CHECK_POINTS();
		//DEBUG_DUMP_MEM_LEAKS();
		//DEBUG_FREE_HEAP();

		_TestMutex();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_RWLockTest();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestSemaphore();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestRegex();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestEnv();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		_TestBitMaskr();
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		DEBUG_FREE_HEAP();

		DEBUG_VALIDATE();
	}
	catch ( Exception *ex )
	{
		Log::SWrite(ex);
		delete ex;
	}
	catch ( OutOfMemoryException mex )
	{
		Log::SWrite(mex);
	}
}
#else
void TestHarness::RunTests()
{
}
#endif

