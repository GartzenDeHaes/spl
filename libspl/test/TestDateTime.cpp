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
#include <spl/DateTime.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifdef DEBUG

static void _TestDateTime1()
{
	DateTime dt;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DateTime test 1.0");

	UNIT_ASSERT("DateTime now", dt.Year() > 2007);
	
	Log::SWriteOkFail( "DateTime test 1" );
}

static void _TestDateTime2()
{
	DateTime dt(1999, 1, 31);
	
	UNIT_ASSERT("DateTime y", dt.Year() == 1999);
	UNIT_ASSERT("DateTime m", dt.Month() == 1);
	UNIT_ASSERT("DateTime d", dt.Day() == 31);
	UNIT_ASSERT("DateTime leap year", !dt.IsLeapYear());
	
	time_t t = dt.ToSysTime();
	DateTime dt2(t);
	
	UNIT_ASSERT("Date time_t", dt.Equals(dt2));
		
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DateTime test 2.0");

	Log::SWriteOkFail( "DateTime test 2" );
}

static void _TestDateTime3()
{
	DateTime dt(1999, 12, 31);
	DateTime dt2(2000, 1, 31);
	
	UNIT_ASSERT("Date eqaual", !dt.Equals(dt2));

	UNIT_ASSERT("Date diff d", 31 == dt2.DiffInDays(dt));
	UNIT_ASSERT("Date diff h", 31*24 == dt2.DiffInHours(dt));
	UNIT_ASSERT("Date diff m", 31*24*60 == dt2.DiffInMinutes(dt));
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DateTime test 3.0");

	Log::SWriteOkFail( "DateTime test 3" );
}

void _TestDateTime()
{
	_TestDateTime1();
	_TestDateTime2();
	_TestDateTime3();
}

#endif
