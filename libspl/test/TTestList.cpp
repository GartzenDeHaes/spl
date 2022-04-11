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
#include <spl/collection/List.h>
#include <spl/io/log/Log.h>
#include <spl/String.h>

#ifdef DEBUG

using namespace spl;

static void _TestList1()
{
	List<int> *list = new List<int>();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( list );
	list->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 1.1");

	list->Add( 1 );
	UNIT_ASSERT("list count", list->Count() == 1);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( list );
	list->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 1.2");

	list->Remove( 1 );
	UNIT_ASSERT("list count", list->Count() == 0);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( list );
	list->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 1.3");

	list->Add( 2 );
	List<int>::Iterator iter = list->Begin();
	UNIT_ASSERT("list next", iter.Next());
	list->RemoveCurrent(iter);
	UNIT_ASSERT("list count", list->Count() == 0);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( list );
	list->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 1.4");

	delete list;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("List test 1.5");

	Log::SWriteOkFail( "TestList test 1" );
}

static void _TestList2()
{
	List<int> list;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	list.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 2.1");

	list.Add( 1 );
	UNIT_ASSERT("list count", list.Count() == 1);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	list.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 2.2");

	list.Remove( 1 );
	UNIT_ASSERT("list count", list.Count() == 0);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	list.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 2.3");

	list.Add( 2 );
	List<int>::Iterator iter = list.Begin();
	UNIT_ASSERT("list next", iter.Next());
	list.RemoveCurrent(iter);
	UNIT_ASSERT("list count", list.Count() == 0);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	list.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 2.4");

	Log::SWriteOkFail( "TestList test 2" );
}

static void _TestList3()
{
	List<String> list;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	list.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 3.1");

	String data1("1");
	list.Add( data1 );
	
	UNIT_ASSERT("list count", list.Count() == 1);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	list.CheckMem();
	data1.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 3.2");

	list.Remove( data1 );
	UNIT_ASSERT("list count", list.Count() == 0);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	data1.CheckMem();
	list.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 3.3");

	list.Add( data1 );
	List<String>::Iterator iter = list.Begin();
	UNIT_ASSERT("list next", iter.Next());
	list.RemoveCurrent(iter);
	UNIT_ASSERT("list count", list.Count() == 0);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	list.CheckMem();
	data1.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("List test 3.4");

	Log::SWriteOkFail( "TestList test 3" );
}

void _TestList()
{
	_TestList1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestList2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestList3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
