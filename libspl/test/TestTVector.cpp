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
#include <spl/collection/Vector.h>
#include <spl/String.h>

using namespace spl;

static void _TVecTest1()
{
	Vector<int> vec;
	vec.Add(1);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("TVector 1.0");

	Log::SWriteOkFail( "Vector test 1" );
}

static void _TVecTest2()
{
	int x;
	Vector<int> vec;

	for ( x = 0; x < 100; x++ )
	{
		vec.Add(x);
	}
	UNIT_ASSERT( "Vector count", vec.Count() == 100 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 2.0");

	for ( x = 0; x < 100; x++ )
	{
		UNIT_ASSERT("Vec elementAt", vec.ElementAt(x) == x);
	}
	for ( x = 0; x < 100; x++ )
	{
		vec.RemoveAt(0);
	}
	UNIT_ASSERT( "Vector count", vec.Count() == 0 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 2.1");

	Log::SWriteOkFail( "Vector test 2" );
}

static void _TVecTest3()
{
	int x;
	Vector<int> vec;

	for ( x = 0; x < 100; x++ )
	{
		vec.Insert(x, x);
	}
	for ( x = 0; x < 100; x++ )
	{
		vec.Insert(x, 50);
	}
	UNIT_ASSERT( "Vector count", vec.Count() == 200 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 3.0");

	for ( x = 0; x < 100; x++ )
	{
		vec.RemoveAt(50);
	}
	for ( x = 0; x < 100; x++ )
	{
		vec.Pop();
	}
	UNIT_ASSERT( "Vector count", vec.Count() == 0 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 3.1");

	Log::SWriteOkFail( "Vector test 3" );
}

static void _TVecTest4()
{
	int x;
	Vector<int> vec;

	for ( x = 0; x < 100; x++ )
	{
		vec.Add(x);
	}
	UNIT_ASSERT( "Vector count", vec.Count() == 100 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 4.0");

	for ( x = 0; x < 100; x++ )
	{
		vec.Remove(x);
	}
	UNIT_ASSERT( "Vector count", vec.Count() == 0 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 4.1");

	Log::SWriteOkFail( "Vector test 4" );
}

static void _TVecTest5()
{
	Vector<String> vec;
	vec.ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 5.0");

	vec.Add(String("hi"));
	vec.ValidateMem();
	UNIT_ASSERT( "Vector count", vec.Count() == 1 );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 5.1");

	Log::SWriteOkFail( "Vector test 5" );
}

static void _TVecTest6()
{
	int x;
	Vector<String> vec;
	vec.ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 6.0");

	for ( x = 0; x < 100; x++ )
	{
		vec.Add(String(Int32::ToString(x)));
	}
	vec.ValidateMem();
	UNIT_ASSERT( "Vector count", vec.Count() == 100 );

	int fecount = 0;
	foreach(s, vec)
	{
		fecount++;
		UNIT_ASSERT("String val", Int32::IsInt(s.CurrentRef()));
	}
	UNIT_ASSERT("foreach count", fecount == 100);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vec.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector 6.1");

	Log::SWriteOkFail( "Vector test 6" );
}

static void _VecRemoveAtTest()
{
	int x;
	Vector<char> *vec = new Vector<char>();

	for ( x = 0; x < 101; x++ )
	{
		vec->Add( (char) (x+1) );
	}
	for ( x = 0; x < 101; x++ )
	{
		vec->RemoveAt( 0 );
	}
	ASSERT_MEM( vec, sizeof(Vector<char>) );
	vec->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( vec );
	vec->CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector va1");

	for ( x = 0; x < 101; x++ )
	{
		vec->Add( (char) (x+1) );
	}
	for ( x = 0; x < 50; x++ )
	{
		vec->RemoveAt( 0 );
	}
	for ( x = 0; x < 1011; x++ )
	{
		vec->Add( (char) (x+1) );
	}
	while ( vec->Count() > 0 )
	{
		vec->RemoveAt( 0 );
	}
	ASSERT_MEM( vec, sizeof(Vector<char>) );
	vec->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( vec );
	vec->CheckMem();
	UNIT_ASSERT_MEM_NOTED("Vector va1");

	delete vec;

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector va1");

	Log::SWriteOkFail( "Vector RemoveAt stress test" );
}

void TVectorTest(  )
{
	_TVecTest1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector A");

	_TVecTest2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector B");

	_TVecTest3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector C");

	_TVecTest4();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector D");

	_TVecTest5();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector E");

	_TVecTest6();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector F");

	_VecRemoveAtTest();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("Vector F");
}

#endif
