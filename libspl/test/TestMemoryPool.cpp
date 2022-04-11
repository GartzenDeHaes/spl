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
#include <spl/io/log/Log.h>
#include <spl/collection/MemoryPool.h>

using namespace spl;

static void _TestMemoryPool1()
{
	MemoryPool *mp = new MemoryPool(9);
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(mp);
	mp->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("MemoryPool test 1.0");

	byte *onebyte = (byte *)mp->Malloc(1);
	ASSERT_PTR(onebyte);
	*onebyte = 'A';
	UNIT_ASSERT("onebyte ptr", mp->IsValidPointer(onebyte));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(mp);
	mp->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("MemoryPool test 1.1");

	byte *onebyte2 = (byte *)mp->Malloc(1);
	ASSERT_PTR(onebyte2);
	*onebyte2 = 'B';

	UNIT_ASSERT("A", *onebyte == 'A');
	UNIT_ASSERT("B", *onebyte2 == 'B');
	ASSERT_PTR(onebyte);
	ASSERT_PTR(onebyte2);
	UNIT_ASSERT("onebyte ptr", mp->IsValidPointer(onebyte));
	UNIT_ASSERT("onebyte ptr2", mp->IsValidPointer(onebyte2));
	
	mp->ValidateMem();

	*onebyte = 'C';
	UNIT_ASSERT("A", *onebyte == 'C');
	UNIT_ASSERT("B", *onebyte2 == 'B');
	ASSERT_PTR(onebyte);
	ASSERT_PTR(onebyte2);
	UNIT_ASSERT("onebyte ptr", mp->IsValidPointer(onebyte));
	UNIT_ASSERT("onebyte ptr2", mp->IsValidPointer(onebyte2));

	mp->Free(onebyte);
	UNIT_ASSERT("B", *onebyte2 == 'B');
	ASSERT_PTR(onebyte2);
	UNIT_ASSERT("onebyte ptr2", mp->IsValidPointer(onebyte2));

	mp->Free(onebyte2);
	mp->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(mp);
	mp->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("MemoryPool test 1.2");

	delete mp;

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("MemoryPool test 1.3");

	Log::SWriteOkFail( "MemoryPool test 1" );
}

void _TestMemoryPool()
{
	_TestMemoryPool1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
