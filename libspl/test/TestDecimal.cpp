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

#ifdef DEBUG
#include <spl/Decimal.h>
#include <spl/io/log/Log.h>

using namespace spl;

static void _TestDecimal1()
{
	Decimal d(0.65);
	UNIT_ASSERT("double", d.ToDouble() == 0.65);
	UNIT_ASSERT("string", d.ToString()->Equals("0.6500"));
	UNIT_ASSERT("int", d.ToInt() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Date test 1.0");

	Log::SWriteOkFail( "Decimal test 1" );
}

static void _TestDecimal2()
{
	Decimal d1(0.65);
	Decimal d2(0.25);

	Decimal sum = d1 + d2;
	UNIT_ASSERT("sum", sum.ToDouble() == 0.9);

	Decimal dif = d1 - d2;
	UNIT_ASSERT("diff", dif.ToDouble() == 0.4);

	Decimal product = d1 * 2;
	UNIT_ASSERT("*", product.ToDouble() == 1.3);

	product = d1 / 5;
	UNIT_ASSERT("/", product.ToDouble() == 0.13);

	UNIT_ASSERT("==", product == 0.13);
	UNIT_ASSERT("==", !(d1 == d2));
	UNIT_ASSERT("!=", (d1 != d2));
	UNIT_ASSERT(">", (d1 > d2));
	UNIT_ASSERT("<", !(d1 < d2));
	UNIT_ASSERT(">=", (d1 >= d2));
	UNIT_ASSERT("<=", !(d1 <= d2));

	d1 = 0.25;
	UNIT_ASSERT("==", d1 == 0.25);
	UNIT_ASSERT("==", (d1 == d2));
	UNIT_ASSERT("!=", !(d1 != d2));
	UNIT_ASSERT(">", !(d1 > d2));
	UNIT_ASSERT("<", !(d1 < d2));
	UNIT_ASSERT(">=", (d1 >= d2));
	UNIT_ASSERT("<=", (d1 <= d2));

	Log::SWriteOkFail( "Decimal test 2" );
}

void _TestDecimal()
{
	_TestDecimal1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestDecimal2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}


#endif
