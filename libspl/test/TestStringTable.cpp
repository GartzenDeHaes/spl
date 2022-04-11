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

#include <spl/io/log/Log.h>
#include <spl/collection/StringTable.h>

using namespace spl;

static void _TestStringTableNull()
{
	StringTable st;
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	st.CheckMem();
	UNIT_ASSERT_MEM_NOTED("StringTable 1.0");

	Log::SWriteOkFail( "StringTable test null" );
}

static void _TestStringTable1()
{
	StringTable st;
	
	String *sp = st.GetPtr("bob");
	UNIT_ASSERT("bob", sp->Equals("bob"));
	UNIT_ASSERT_MEM("string", sp, sizeof(String));

	String& sr = st.Get("bob");
	UNIT_ASSERT("bob", sr.Equals("bob"));

	sr = "hi";
	UNIT_ASSERT("hi", sp->Equals("hi"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	st.CheckMem();
	UNIT_ASSERT_MEM_NOTED("StringTable 1.0");

	Log::SWriteOkFail( "StringTable test 1" );
}

void _TestStringTable(  )
{
	_TestStringTableNull();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("StringTable A");

	_TestStringTable1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("StringTable B");
}

#endif
