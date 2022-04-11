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
#include <spl/Date.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifdef DEBUG

static void _TestDate1()
{
	Date dt;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Date test 1.0");

	UNIT_ASSERT("Date now", dt.Year() > 2007);
	
	Log::SWriteOkFail( "Date test 1" );
}

static void _TestDate2()
{
	Date dt(1999, 1, 31);
	
	UNIT_ASSERT("Date y", dt.Year() == 1999);
	UNIT_ASSERT("Date m", dt.Month() == 1);
	UNIT_ASSERT("Date d", dt.Day() == 31);
	UNIT_ASSERT("Date dow", dt.DayOfWeek() == 0);
	UNIT_ASSERT("Date leap year", !dt.IsLeapYear());
	UNIT_ASSERT("Date do year", dt.DayOfYear() == 31);
	
	time_t t = dt.ToSysTime();
	Date dt2(t);
	
	UNIT_ASSERT("Date time_t", dt.Equals(dt2));
		
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Date test 2.0");

	Log::SWriteOkFail( "Date test 2" );
}

static void _TestDate3()
{
	Date dt(1999, 12, 31);
	Date dt2(2000, 1, 31);
		
	UNIT_ASSERT("Date leap year", dt2.IsLeapYear());
	UNIT_ASSERT("Date equal", !dt.Equals(dt2));
	UNIT_ASSERT("Date diff", 31 == dt2.DiffInDays(&dt));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Date test 3.0");

	Log::SWriteOkFail( "Date test 3" );
}

void _TestDate()
{
	_TestDate1();
	_TestDate2();
	_TestDate3();
}

#endif
