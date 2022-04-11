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

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/io/DesStream.h>
#include <spl/io/log/Log.h>
#include <spl/io/Stream.h>
#include <spl/io/MemoryStream.h>

using namespace spl;

static void _TestDesStream1()
{
	MemoryStreamPtr ms = MemoryStreamPtr(new MemoryStream());
	DesStream ds("the password", ms);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ds.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DesStream test 1.1");

	const byte *txt = (const byte *)"the time has come, said the walrus.....";
	Array<byte> txta(txt, (int)strlen((const char *)txt)+1);
	UNIT_ASSERT("txt should be 40 chars", strlen((const char *)txt) == 39);

	ds.Write(txta, 0, 40);
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ds.CheckMem();
	txta.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DesStream test 1.2");

	Array<byte> buf(41);
	ds.Read(buf, 0, 40);
	UNIT_ASSERT("decryption failed", strcmp((char *)buf.Data(), (const char *)txt) == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ds.CheckMem();
	txta.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DesStream test 1.3");

	Log::SWriteOkFail( "DesStream test 1" );
}

void _TestDesStream()
{
	_TestDesStream1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
