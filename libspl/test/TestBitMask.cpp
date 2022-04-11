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

#include <spl/math/BitMask.h>
#include <spl/io/log/Log.h>

using namespace spl;

void _TestBitMaskr()
{
    int x;
    int len = 130;
    BitMaskPtr bm(new BitMask(1, len));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	bm.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BitMask mem 1");

    for (x = 0; x < len; x++)
    {
        UNIT_ASSERT("bit check 0", ! bm->IsSet(0, x));
    }

    for (x = 1; x < len; x += 2)
    {
        bm->Set(0, x);
    }

    for (x = 1; x < len; x += 2)
    {
        UNIT_ASSERT("bit check 1", bm->IsSet(0, x));
    }

    for (x = 0; x < len; x += 2)
    {
        UNIT_ASSERT("bit check 1", ! bm->IsSet(0, x));
    }

    bm.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BitMask mem");

	Log::SWriteOkFail( "BitMask Test" );
}

#endif
