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
#include <spl/io/DelimitedFile.h>
#include <spl/io/log/Log.h>
#include <spl/io/MemoryStream.h>

using namespace spl;

static void _TestDelimFile1()
{
	MemoryStreamPtr ms = MemoryStreamPtr(new MemoryStream());
	const char *csv = "abc,123,!@#\nrfd,\"567\",$%^\n";
	Array<byte> csva((const byte *)csv, (int)strlen(csv));
	ms->Write(csva);

	TextReader reader( ms );
	DelimitedFilePtr df = DelimitedFile::Parse(reader, ',');

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	reader.CheckMem();
	csva.CheckMem();
	df.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DelimitedFile 1.0");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	df.CheckMem();
	reader.CheckMem();
	csva.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DelimitedFile 1.0");

	UNIT_ASSERT("row count", df->RowCount() == 2);
	UNIT_ASSERT("col count 1", df->RowAt(0)->Count() == 3);
	UNIT_ASSERT("col count 2", df->RowAt(1)->Count() == 3);

	UNIT_ASSERT("abc", df->CellAt(0, 0)->ToString()->Equals("abc"));
	UNIT_ASSERT("123", df->CellAt(0, 1)->ToString()->Equals("123"));
	UNIT_ASSERT("!@#", df->CellAt(0, 2)->ToString()->Equals("!@#"));
	UNIT_ASSERT("rfd", df->CellAt(1, 0)->ToString()->Equals("rfd"));
	UNIT_ASSERT("567", df->CellAt(1, 1)->ToString()->Equals("567"));
	UNIT_ASSERT("$%^", df->CellAt(1, 2)->ToString()->Equals("$%^"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	csva.CheckMem();
	df.CheckMem();
	reader.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DelimitedFile 1.2");
	Log::SWriteOkFail( "DelimitedFile 1" );
}

static void _TestDelimFile2()
{
	MemoryStreamPtr ms = MemoryStreamPtr(new MemoryStream());
	const char *csv = "abc,123,!@#\nrfd,\"567\",\n,,\n";
	Array<byte> csva((const byte *)csv, (int)strlen(csv));
	ms->Write(csva);
	TextReader reader( ms );
	DelimitedFilePtr df = DelimitedFile::Parse(reader, ',');

	UNIT_ASSERT("row count", df->RowCount() == 3);
	UNIT_ASSERT("col count 1", df->RowAt(0)->Count() == 3);
	UNIT_ASSERT("col count 2", df->RowAt(1)->Count() == 3);
	UNIT_ASSERT("col count 3", df->RowAt(2)->Count() == 3);

	UNIT_ASSERT("abc", df->CellAt(0, 0)->ToString()->Equals("abc"));
	UNIT_ASSERT("123", df->CellAt(0, 1)->ToString()->Equals("123"));
	UNIT_ASSERT("!@#", df->CellAt(0, 2)->ToString()->Equals("!@#"));
	UNIT_ASSERT("rfd", df->CellAt(1, 0)->ToString()->Equals("rfd"));
	UNIT_ASSERT("567", df->CellAt(1, 1)->ToString()->Equals("567"));
	UNIT_ASSERT("$%^", df->CellAt(1, 2)->ToString()->Equals(""));
	UNIT_ASSERT("\\0", df->CellAt(2, 0)->ToString()->Equals(""));
	UNIT_ASSERT("\\0", df->CellAt(2, 1)->ToString()->Equals(""));
	UNIT_ASSERT("\\0", df->CellAt(2, 2)->ToString()->Equals(""));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	csva.CheckMem();
	reader.CheckMem();
	df.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DelimitedFile 2.2");
	Log::SWriteOkFail( "DelimitedFile 2" );
}

void _TestDelimFile()
{
	_TestDelimFile1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestDelimFile2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}


#endif
