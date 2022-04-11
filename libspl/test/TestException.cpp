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

using namespace spl;

#ifdef DEBUG

void static _ExceptionTest1()
{
	try
	{
		throw new Exception("hi");
		UNIT_ASSERT("Exception not thrown", false);
	}
	catch ( Exception )
	{
		UNIT_ASSERT("Execption", false);
	}
	catch ( Exception *ex )
	{
		delete ex;
		return;
	}
	UNIT_TEST("Exception catch", false);
}

void static _ExceptionTest2()
{
	try
	{
		throw new InvalidArgumentException("hi");
		UNIT_ASSERT("Exception not thrown", false);
	}
	catch ( Exception )
	{
		UNIT_ASSERT("Execption", false);
	}
	catch ( Exception *ex )
	{
		delete ex;
		return;
	}
	UNIT_ASSERT("Exception not catched", false);
}

void static _ExceptionTest3()
{
	try
	{
		throw new SocketException("hi");
		UNIT_ASSERT("Exception not thrown", false);
	}
	catch ( Exception )
	{
		UNIT_ASSERT("Execption", false);
	}
	catch ( Exception *ex )
	{
		delete ex;
		return;
	}
	UNIT_ASSERT("Exception not catched", false);
}

void _ExceptionTestHarness()
{
	_ExceptionTest1();
	Log::SWriteOkFail( "Exception 1" );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Exception Test 1");

	_ExceptionTest2();
	Log::SWriteOkFail( "Exception 2" );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Exception Test 2");

	_ExceptionTest3();
	Log::SWriteOkFail( "Exception 3" );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Exception Test 2");
}

#endif
