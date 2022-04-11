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

#if defined(DEBUG) || defined(_DEBUG)

#include <spl/Int32.h>
#include <spl/io/log/Log.h>
#include <spl/collection/Hashtable.h>

using namespace spl;

static void _THashTest1()
{
	Hashtable<int, int> ht;
	ht.ValidateMem();
	ht.Set(1, 1);
	ht.ValidateMem();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 1.1");

	{
		RefCountPtr<Vector<int> > vec = ht.Values();	
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		ht.CheckMem();
		vec.CheckMem();
		UNIT_ASSERT_MEM_NOTED("THashtable 1.2");
	}

	ht.Clear();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 1.3");
	Log::SWriteOkFail( "Hashtable test 1" );
}

static void _THashTest1a()
{
	Hashtable<int, int> ht;

	ht.Set(0, 99);
	UNIT_ASSERT("THash set", ht.Get(0) == 99);
	UNIT_ASSERT("THash cleared", ht.Count() == 1);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 1a.1");
	
	ht.Remove(0);
	UNIT_ASSERT("ht count", ht.Count() == 0);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 1a.2");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 1a.3");
	Log::SWriteOkFail( "Hashtable test 1a" );
}

static void _THashTest2()
{
	int x;
	Hashtable<int, int> ht;

	for ( x = 0; x < 100; x++ )
	{
		ht.Set(x, x);
		UNIT_ASSERT("THash set", ht.Get(x) == x);
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 2.1");
	
	for ( x = 0; x < 100; x++ )
	{
		ht.Remove(x);
		UNIT_ASSERT("ht count", ht.Count() == 99 - x);
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		ht.CheckMem();
		UNIT_ASSERT_MEM_NOTED("THashtable 2.2");
	}
	UNIT_ASSERT("THash cleared", ht.Count() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 2.3");
	Log::SWriteOkFail( "Hashtable test 2" );
}

static void _THashTest3()
{
	int x;
	Hashtable<int, int> ht;

	for ( x = 100; x >= 0; x-- )
	{
		ht.Set(x, x);
		UNIT_ASSERT("THash set", ht.Get(x) == x);
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 3.1");
	
	for ( x = 100; x >= 0; x-- )
	{
		ht.Remove(x);
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		ht.CheckMem();
		UNIT_ASSERT_MEM_NOTED("THashtable 3.2");
	}
	UNIT_ASSERT("THash cleared", ht.Count() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 3.3");
	Log::SWriteOkFail( "Hashtable test 3" );
}

static void _THashTest4()
{
	int x;
	Math::InitRandom();
	Hashtable<int, int> ht;

	for ( x = 100; x >= 0; x-- )
	{
		int k = Math::RandomRange(5000);
		ht.Set(k, k);
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 4.1");
	
	for ( x = 100; x >= 0; x-- )
	{
		int k = Math::RandomRange(5000);
		ht.Remove(k);
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		ht.CheckMem();
		UNIT_ASSERT_MEM_NOTED("THashtable 4.2");
	}
	ht.Clear();
	UNIT_ASSERT("THash cleared", ht.Count() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 4.3");
	Log::SWriteOkFail( "Hashtable test 4" );
}

static void _THashTest5()
{
	int x;
	Hashtable<String, int> ht;

	for ( x = 500; x >= 0; x-- )
	{
		int k = Math::RandomRange(500);
		ht.Set(*Int32::ToString(k), k);
		UNIT_ASSERT("THash set", ht.Get(*Int32::ToString(k)) == k);
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 5.1");
	
	for ( x = 200; x >= 0; x-- )
	{
		int k = Math::RandomRange(500);
		ht.Remove(*Int32::ToString(k));
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		ht.CheckMem();
		UNIT_ASSERT_MEM_NOTED("THashtable 5.2");
	}
	ht.Clear();
	UNIT_ASSERT("THash cleared", ht.Count() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 5.3");
	Log::SWriteOkFail( "Hashtable test 5" );
}

static void _THashTest6()
{
	Hashtable<String, int> ht;

	ht.Set("0", 0);
	int count = 0;
	
	foreach(v,ht)
	{
		count++;
		UNIT_ASSERT("hashtable foreach", v.Current() == 0);
	}
	UNIT_ASSERT("foreach count", count == 1);
	
	ht.Set("1", 1);
	count = 0;
	
	UNIT_ASSERT("0", ht.ContainsKey("0"));
	UNIT_ASSERT("1", ht.ContainsKey("1"));

	UNIT_ASSERT("0", ht.Get("0") == 0);
	UNIT_ASSERT("1", ht.Get("1") == 1);

	{
		Hashtable<String, int>::Iterator itt = ht.Begin();
		while(itt.Next())
		{
			count++;
			UNIT_ASSERT("hashtable itter", itt.Current() == 0 || itt.Current() == 1);
		}
		UNIT_ASSERT("foreach count", count == 2);
	}

	{
		count = 0;
		foreach(v2,ht)
		{
			count++;
			UNIT_ASSERT("hashtable foreach 2", v2.Current() == 0 || v2.Current() == 1);
		}
		UNIT_ASSERT("foreach count", count == 2);
	}
	
	ht.Clear();
	UNIT_ASSERT("THash cleared", ht.Count() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ht.CheckMem();
	UNIT_ASSERT_MEM_NOTED("THashtable 5.3");
	Log::SWriteOkFail( "Hashtable test 6" );
}

void THashtableTest(  )
{
	_THashTest1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Hashtable A");

	_THashTest1a();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Hashtable A.1");

	_THashTest2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Hashtable B");

	_THashTest3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Hashtable C");

	_THashTest4();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Hashtable D");

	_THashTest5();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Hashtable D");
	
	_THashTest6();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Hashtable E");
}

#endif
