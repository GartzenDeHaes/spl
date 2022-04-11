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
//#include <spl/interp/JsObject.h>
#include <spl/io/log/Log.h>
#include <spl/Variant.h>

#ifdef DEBUG

using namespace spl;

static void _TestVariant1()
{
	Variant *vp = new Variant(1);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( vp );
	vp->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant 1.1");

	delete vp;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Variant 1.1");
	Log::SWriteOkFail( "Variant 1" );
}

static void _TestVariantBinOps()
{
	Variant v1(4), v2(2);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	v1.CheckMem();
	v2.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant 2.1");

	Variant res = v1 + v2;
	UNIT_ASSERT("4 + 2", res.ToInt32() == 6);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	v1.CheckMem();
	v2.CheckMem();
	res.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant 2.2");

	res = v1 - v2;
	UNIT_ASSERT("4 - 2", res.ToInt32() == 2);

	res = v1 / v2;
	UNIT_ASSERT("4 / 2", res.ToInt32() == 2);

	res = v1 * v2;
	UNIT_ASSERT("4 * 2", res.ToInt32() == 8);

	res = v2 % v1;
	UNIT_ASSERT(*res.ToString() + " == 2 %% 4", res.ToInt32() == 2);

	v1 = 4.4;
	v2 = 2.2;

	res = v1 + v2;
	UNIT_ASSERT(*res.ToString() + " == 4.4 + 2.2", res.ToString()->Equals("6.6"));

	res = v1 - v2;
	UNIT_ASSERT(*res.ToString() + " == 4.4 - 2.2", res.ToString()->Equals("2.2"));

	res = v1 / v2;
	UNIT_ASSERT(*res.ToString() + " == 4.4 / 2.2", Math::Round(res.ToFloat64(), 2) == 2);

	v1 = "a";
	v2 = "b";

	res = v1 + v2;
	UNIT_ASSERT(*res.ToString() + " == ab", res.ToString()->Equals("ab"));

	res = "11";
	UNIT_ASSERT(*res.ParseInt()->ToString() + " == parseInt", res.ParseInt()->ToInt64() == 11);
	UNIT_ASSERT(*res.ParseFloat()->ToString() + " == parseFloat", res.ParseFloat()->ToFloat64() == 11.0);

	res = v1 / v2;
	UNIT_ASSERT(*res.ToString() + " == undefined", res.ToString()->Equals("undefined"));

	v1 = DateTime::Now();
	res = v1 * v2;
	UNIT_ASSERT(*res.ToString() + " == undefined", res.ToString()->Equals("undefined"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	v1.CheckMem();
	v2.CheckMem();
	res.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant 2.3");

	Log::SWriteOkFail( "Variant math" );
}

static void _TestVariantLogOps()
{
	Variant v1(4), v2(2);

	v1 = true;
	v2 = false;

	Variant res = v1 | v2;
	UNIT_ASSERT(*res.ToString() + " == true | false", res.ToBool() == true);
	
	res = v1 || v2;
	UNIT_ASSERT(*res.ToString() + " == true || false", res.ToBool() == true);

	res = v1 & v2;
	UNIT_ASSERT(*res.ToString() + " == true & false", res.ToBool() == false);

	res = v1 && v2;
	UNIT_ASSERT(*res.ToString() + " == true && false", res.ToBool() == false);

	res = v1 ^ v2;
	UNIT_ASSERT(*res.ToString() + " == true ^ false", res.ToBool() == true);

	v1 = 1;
	v2 = 0;
	
	res = v1 > v2;
	UNIT_ASSERT(*res.ToString() + " == 1 > 0", res.ToBool() == true);
	
	res = v1 < v2;
	UNIT_ASSERT(*res.ToString() + " == 1 < 0", res.ToBool() == false);

	res = v1 >= v2;
	UNIT_ASSERT(*res.ToString() + " == 1 >= 0", res.ToBool() == true);

	res = v1 <= v2;
	UNIT_ASSERT(*res.ToString() + " == 1 <= 0", res.ToBool() == false);

	res = v1 == v2;
	UNIT_ASSERT(*res.ToString() + " == 1 == 0", res.ToBool() == false);

	res = v1 != v2;
	UNIT_ASSERT(*res.ToString() + " == 1 != 0", res.ToBool() == true);

	v1 = "hi";
	v2 = "hi";
	UNIT_ASSERT(*res.ToString() + " == 1 == 0", res.ToBool() == true);

	v1 = DateTime::Now();
	v2 = v1;
	UNIT_ASSERT(*res.ToString() + " == Now == Now", res.ToBool() == true);

	v1 = 1;
	v2 = 2;
	
	res = v1 | v2;
	UNIT_ASSERT(*res.ToString() + " == 1 | 2 ", res.ToInt32() == 3);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	v1.CheckMem();
	v2.CheckMem();
	res.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant 2.1");

	Log::SWriteOkFail( "Variant logic" );
}

static void _TestVariantUnOps()
{
	Variant v1(false);

	Variant res = !v1;
	UNIT_ASSERT(*res.ToString() + " == !false", res.ToBool() == true);

	v1 = 1;
	res = ~v1;
	UNIT_ASSERT(*res.ToString() + " == ~1", res.ToInt32() != 1 && res.ToInt32() < 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	v1.CheckMem();
	res.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant 3.1");

	Log::SWriteOkFail( "Variant unary" );
}

static void _TestVariantObject()
{
/*	Variant v1;

	if (true)
	{
		RefCountPtr<IJsObject> tmp(new JsObject());
		v1 = tmp;

		Variant v2 = v1;
		v1.ValidateMem();
		v2.ValidateMem();
		UNIT_ASSERT("IsObject",  v2.IsObject());
	}

	v1.ValidateMem();	
	UNIT_ASSERT("IsObject",  v1.IsObject());

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	v1.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant object 1");
*/
	Log::SWriteOkFail( "Variant object" );
}

static void _TestVariantToObject()
{
/*	Variant v1;

	if (true)
	{
		RefCountPtr<IJsObject> tmp(new JsObject());
		v1 = tmp;
	}

	Variant v2;
	if (true)
	{
		RefCountPtr<IJsObject> tmp2 = v1.ToObject();
		UNIT_ASSERT_MEM("jsobject ref mem", tmp2.Get(), sizeof(JsObject));

		v2 = tmp2;
		v2.ValidateMem();
	}

	v1.ValidateMem();
	v2.ValidateMem();
	UNIT_ASSERT("IsObject",  v1.IsObject());

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	v1.CheckMem();
	v2.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Variant object 1");
*/
	Log::SWriteOkFail( "Variant ToObject" );
}

void _TestVariant()
{
	_TestVariant1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestVariantBinOps();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestVariantLogOps();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestVariantUnOps();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestVariantObject();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestVariantToObject();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
