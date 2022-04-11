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
#include <spl/RefCountPtr.h>
#include <spl/String.h>

using namespace spl;

#ifdef DEBUG

static void _TestRefCountPtr1()
{
	RefCountPtr<int> pi(new int[1]);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	pi.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RefCountPtr test 1.0");

	UNIT_ASSERT("1", pi.ReferenceCount() == 1);

	{
		RefCountPtr<int> pi2 = pi;
		pi2.ValidateMem();
		pi.ValidateMem();
		UNIT_ASSERT("assign", pi2.ReferenceCount() == 2);
		UNIT_ASSERT("assign", pi.ReferenceCount() == 2);

		*pi2 = 6;
	}

	UNIT_ASSERT("1", pi.ReferenceCount() == 1);
	UNIT_ASSERT("6", *pi == 6);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	pi.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RefCountPtr test 1.1");

	Log::SWriteOkFail( "RefCountPtr test" );
}

static StringPtr _TestReturnStringPtrNull()
{
	return StringPtr();
}

void _TestRefCountPtrRegression1()
{
	{
		StringPtr ptr = _TestReturnStringPtrNull();
		UNIT_ASSERT("Should be null", ptr.IsNull());
		UNIT_ASSERT("Should be not notnull", ! ptr.IsNotNull());
	}
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RefCountPtr regression 1.1");

	Log::SWriteOkFail( "RefCountPtr StringPtr() NULL regression" );
}

void _TestRefCountPtr()
{
	_TestRefCountPtr1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestRefCountPtr");
	
	_TestRefCountPtrRegression1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestRefCountPtrRegression1");
}

#endif
