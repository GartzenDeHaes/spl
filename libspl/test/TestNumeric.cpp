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
#include <spl/Double.h>
#include <spl/Numeric.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
#include <spl/UInt32.h>
#include <spl/UInt64.h>

#include <math.h>

using namespace spl;

#ifdef DEBUG

static void _TestIsNumeric()
{
	UNIT_ASSERT("", Numeric::IsNumeric("") == false);
	UNIT_ASSERT(".", Numeric::IsNumeric(".") == false);
	UNIT_ASSERT("a", Numeric::IsNumeric("a") == false);
	UNIT_ASSERT("--", Numeric::IsNumeric("--") == false);
	UNIT_ASSERT("+1+", Numeric::IsNumeric("+1+") == false);
	UNIT_ASSERT("1+", Numeric::IsNumeric("1+") == false);
	UNIT_ASSERT("999.9.", Numeric::IsNumeric("999.9.") == false);
	UNIT_ASSERT(".9.", Numeric::IsNumeric(".9.") == false);

	UNIT_ASSERT("1", Numeric::IsNumeric("1") == true);
	UNIT_ASSERT("999", Numeric::IsNumeric("999") == true);
	UNIT_ASSERT("999.9", Numeric::IsNumeric("999.9") == true);
	UNIT_ASSERT(".9", Numeric::IsNumeric(".9") == true);

	UNIT_ASSERT("-1", Numeric::IsNumeric("-1") == true);
	UNIT_ASSERT("+1", Numeric::IsNumeric("+1") == true);
	UNIT_ASSERT("-10", Numeric::IsNumeric("-10") == true);

	Log::SWriteOkFail( "IsNumeric test" );
}

static void _TestInt32Parse()
{
	UNIT_ASSERT("42", Int32::Parse("42") == 42);
	UNIT_ASSERT("-42", Int32::Parse("-42") == -42);
	UNIT_ASSERT("+42", Int32::Parse("+42") == 42);
	UNIT_ASSERT("0x42", Int32::Parse("0x42", 4, 16) == 0x42);
	UNIT_ASSERT("42 base 16", Int32::Parse("42", 2, 16) == 0x42);

	Log::SWriteOkFail( "Int32::Parse test" );
}

static void _TestInt32IsInt()
{
	UNIT_ASSERT("42", Int32::IsInt("42"));
	UNIT_ASSERT("-42", Int32::IsInt("-42"));
	UNIT_ASSERT("+42", Int32::IsInt("+42"));
	UNIT_ASSERT("0x42", Int32::IsInt("0x42", 4, 16));

	UNIT_ASSERT("42.0", !Int32::IsInt("42.0"));
	UNIT_ASSERT("0x42 base 10", !Int32::IsInt("0x42", 4, 10));
	UNIT_ASSERT("42 base 16", Int32::IsInt("42", 2, 16));
	UNIT_ASSERT("a42", !Int32::IsInt("a42"));
	UNIT_ASSERT("42a", !Int32::IsInt("42a"));
	UNIT_ASSERT("a", !Int32::IsInt("a"));

	Log::SWriteOkFail( "Int32::IsInt test" );
}

static void _TestInt32ToString()
{
	UNIT_ASSERT("42", *Int32::ToString(42) == "42");
	UNIT_ASSERT("-42", *Int32::ToString(-42) == "-42");
	UNIT_ASSERT("0x42", *Int32::ToString(0x42, 16) == "42");
	UNIT_ASSERT("20100429", *Int32::ToString(20100429) == "20100429");

	Log::SWriteOkFail( "Int32::ToString test" );
}

static void _TestUInt32Parse()
{
	UNIT_ASSERT("42", UInt32::Parse("42") == 42);
	UNIT_ASSERT("+42", UInt32::Parse("+42") == 42);
	UNIT_ASSERT("0x42", UInt32::Parse("0x42", 4, 16) == 0x42);

	Log::SWriteOkFail( "UInt32::Parse test" );
}

static void _TestUInt32IsInt()
{
	UNIT_ASSERT("42", UInt32::IsInt("42"));
	UNIT_ASSERT("-42", !UInt32::IsInt("-42"));
	UNIT_ASSERT("+42", !UInt32::IsInt("+42"));
	UNIT_ASSERT("0x42", UInt32::IsInt("0x42", 4, 16));

	UNIT_ASSERT("42.0", !UInt32::IsInt("42.0"));
	UNIT_ASSERT("0x42 base 10", !UInt32::IsInt("0x42", 4, 10));
	UNIT_ASSERT("42 base 16", UInt32::IsInt("42", 4, 16));
	UNIT_ASSERT("a42", !UInt32::IsInt("a42"));
	UNIT_ASSERT("42a", !UInt32::IsInt("42a"));
	UNIT_ASSERT("a", !UInt32::IsInt("a"));

	Log::SWriteOkFail( "UInt32::IsInt test" );
}

static void _TestUInt32ToString()
{
	UNIT_ASSERT("42", *UInt32::ToString(42) == "42");
	UNIT_ASSERT("0x42", *UInt32::ToString(0x42, 16) == "42");

	Log::SWriteOkFail( "UInt32::ToString test" );
}

static void _TestUInt64Parse()
{
	UNIT_ASSERT("42", UInt64::Parse("42") == 42);
	UNIT_ASSERT("+42", UInt64::Parse("+42") == 42);
	UNIT_ASSERT("0x42", UInt64::Parse("0x42", 4, 16) == 0x42);

	Log::SWriteOkFail( "UInt64::Parse test" );
}

static void _TestUInt64IsInt()
{
	UNIT_ASSERT("42", UInt64::IsInt("42"));
	UNIT_ASSERT("-42", !UInt64::IsInt("-42"));
	UNIT_ASSERT("+42", !UInt64::IsInt("+42"));
	//UNIT_ASSERT("0x42", UInt64::IsInt("0x42", 4, 16));

	UNIT_ASSERT("42.0", !UInt64::IsInt("42.0"));
	//UNIT_ASSERT("0x42 base 10", !UInt64::IsInt("0x42", 4, 10));
	//UNIT_ASSERT("42 base 16", UInt64::IsInt("42", 16));
	UNIT_ASSERT("a42", !UInt64::IsInt("a42"));
	UNIT_ASSERT("42a", !UInt64::IsInt("42a"));
	UNIT_ASSERT("a", !UInt64::IsInt("a"));

	Log::SWriteOkFail( "UInt64::IsInt test" );
}

static void _TestUInt64ToString()
{
	UNIT_ASSERT("42", *UInt64::ToString(42) == "42");
	UNIT_ASSERT("0x42", *UInt64::ToString(0x42, 16) == "42");

	Log::SWriteOkFail( "UInt64::ToString test" );
}

static void _TestInt64Parse()
{
	UNIT_ASSERT("42", Int64::Parse("42") == 42);
	UNIT_ASSERT("+42", Int64::Parse("+42") == 42);
	UNIT_ASSERT("-42", Int64::Parse("-42") == -42);
	//UNIT_ASSERT("0x42", Int64::Parse("0x42", 16) == 0x42);

	Log::SWriteOkFail( "Int64::Parse test" );
}

static void _TestInt64IsInt()
{
	UNIT_ASSERT("42", Int64::IsInt("42"));
	UNIT_ASSERT("-42", Int64::IsInt("-42"));
	UNIT_ASSERT("+42", Int64::IsInt("+42"));
	//UNIT_ASSERT("0x42", Int64::IsInt("0x42", 16));

	UNIT_ASSERT("42.0", !Int64::IsInt("42.0"));
	//UNIT_ASSERT("0x42 base 10", !Int64::IsInt("0x42", 10));
	//UNIT_ASSERT("42 base 16", Int64::IsInt("42", 16));
	UNIT_ASSERT("a42", !Int64::IsInt("a42"));
	UNIT_ASSERT("42a", !Int64::IsInt("42a"));
	UNIT_ASSERT("a", !Int64::IsInt("a"));

	Log::SWriteOkFail( "Int64::IsInt test" );
}

static void _TestInt64ToString()
{
	UNIT_ASSERT("42", *Int64::ToString(42) == "42");
	//UNIT_ASSERT("0x42", Int64::ToString(0x42, 16) == "0x42");

	Log::SWriteOkFail( "Int64::ToString test" );
}

#include <stdio.h>

static void _TestDoubleParse()
{
	UNIT_ASSERT("42", Double::Parse("42") == 42);
	UNIT_ASSERT("42.42", floor(Double::Parse("42.42")*100) == 4242);
	UNIT_ASSERT("-42", Double::Parse("-42") == -42);
	UNIT_ASSERT("-42.42", floor(Double::Parse("-42.42")*100) == -4242);
	UNIT_ASSERT("42e2", Double::Parse("42e2") == 42e2);
	UNIT_ASSERT("42e-2", floor(Double::Parse("42e-2")*100) == floor(42e-2 * 100));

	Log::SWriteOkFail( "Double::Parse test" );
}

static void _TestDoubleIsDouble()
{
	UNIT_ASSERT("42", Double::IsDouble("42"));
	UNIT_ASSERT(".42", Double::IsDouble(".42"));
	UNIT_ASSERT("-42", Double::IsDouble("-42"));
	UNIT_ASSERT("+42", Double::IsDouble("+42"));

	UNIT_ASSERT("a42", !Double::IsDouble("a42.0"));
	UNIT_ASSERT("42a", !Double::IsDouble("42a"));
	UNIT_ASSERT("a", !Double::IsDouble("a"));

	Log::SWriteOkFail( "Double::IsDouble test" );
}

static void _TestDoubleToString()
{
	UNIT_ASSERT("-42.1", *Double::ToString(-42.1) == "-42.1");
	UNIT_ASSERT("42.0", *Double::ToString(42.0) == "42");
	UNIT_ASSERT("42e2", *Double::ToString(42e2) == "4200");
	UNIT_ASSERT("42e-2", *Double::ToString(42e-2) == "0.42");

	Log::SWriteOkFail( "Double::ToString test" );
}

void _TestNumeric()
{
	_TestIsNumeric();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestInt32Parse();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestInt32IsInt();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestInt32ToString();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	UNIT_ASSERT("Int32::Min/Max", Int32::MinValue() < Int32::MaxValue());

	_TestUInt32Parse();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestUInt32IsInt();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestUInt32ToString();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	UNIT_ASSERT("UInt32::Min/Max", UInt32::MinValue() < UInt32::MaxValue());

	_TestUInt64Parse();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestUInt64IsInt();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestUInt64ToString();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	UNIT_ASSERT("UInt32::Min/Max", UInt64::MinValue() < UInt64::MaxValue());

	_TestInt64Parse();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestInt64IsInt();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestInt64ToString();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	UNIT_ASSERT("Int64::Min/Max", Int64::MinValue() < Int64::MaxValue());

	_TestDoubleParse();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestDoubleIsDouble();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestDoubleToString();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
