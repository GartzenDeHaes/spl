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
#include <spl/io/File.h>
#include <spl/io/FileStream.h>
#include <spl/io/log/Log.h>
#include <spl/io/MemoryStream.h>

using namespace spl;

#ifdef DEBUG

static void _TestStream1()
{
	String str1("hi there this is some text that");
	MemoryStream ms;
	ms.Write(str1.ToByteArray());
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str1.CheckMem();
	ms.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Stream test 1.0");

	const char *filename = "t_testfile.txt";
	if ( File::Exists (filename) )
	{
		File::Delete (filename);
	}
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ms.CheckMem();
	str1.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Stream test 1.0a");

	TextWriterPtr tw = File::CreateText(filename);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ms.CheckMem();
	str1.CheckMem();
	tw.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Stream test 1.0ab");

	tw->WriteLine("The time has come");
	tw->WriteLine("said the walrus");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ms.CheckMem();
	str1.CheckMem();
	tw.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Stream test 1.0b");

	tw->Close();
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ms.CheckMem();
	str1.CheckMem();
	tw.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Stream test 1.0c");

	FileStreamPtr fs(new FileStream(filename, File::FILEMODE_Open, File::FILEACC_Read));
	UNIT_ASSERT("Can not read file", fs->CanRead());
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str1.CheckMem();
	ms.CheckMem();
	fs.CheckMem();
	tw.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Stream test 1.1");
	
	TextReader tr(fs);
	StringPtr line = tr.ReadLine();
	UNIT_ASSERT("Line 1", line->Equals("The time has come"));
	line = tr.ReadLine();
	UNIT_ASSERT("Line 2", line->Equals("said the walrus"));
	UNIT_ASSERT("EOF", tr.Read() == -1);
	
	tr.Close();
	File::Delete(filename);

	Log::SWriteOkFail( "Stream test 1" );
}

static void _TestMemoryStream()
{
	int x;
	MemoryStream ms;
	
	for ( x = 0; x < 1000; x++ )
	{
		ms.WriteByte((byte)'a');
		ms.WriteByte((byte)'b');
		ms.WriteByte((byte)'c');
	}
	UNIT_ASSERT("Should be 3000", ms.Length() == 3000);
	
	ms.Seek(0, IStream::SEEK_Begin);
	for ( x = 0; x < 1000; x++ )
	{
		UNIT_ASSERT("a", ms.ReadByte() == 'a');
		UNIT_ASSERT("b", ms.ReadByte() == 'b');
		UNIT_ASSERT("c", ms.ReadByte() == 'c');
	}
	
	UNIT_ASSERT("Should be empty", ms.Length() == 0);
	
	Log::SWriteOkFail( "MemoryStream test 1" );
}

static void _TestMemoryStream2()
{
	MemoryStream ms;
	int x;
	
	for ( x = 0; x < 1000; x++ )
	{
		ms.WriteByte((byte)'a');
		ms.WriteByte((byte)'b');
		ms.WriteByte((byte)'c');
	}
	UNIT_ASSERT("Should be 3000", ms.Length() == 3000);
	UNIT_ASSERT("Position", ms.Position() == 0);
	
	ms.Seek(0, IStream::SEEK_Begin);
	Array<byte> buf(1001);

	ms.Read(buf, 0, 900);
	UNIT_ASSERT("Should be 2100", ms.Length() == 2100);
	UNIT_ASSERT("Position", ms.Position() == 0);
	for ( x = 0; x < 900; x+=3 )
	{
		UNIT_ASSERT("a", buf[x] == 'a');
		UNIT_ASSERT("b", buf[x+1] == 'b');
		UNIT_ASSERT("c", buf[x+2] == 'c');
	}
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ms.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("MemoryStream 2.1");
	
	for ( x = 0; x < 700; x++ )
	{
		UNIT_ASSERT("a", ms.ReadByte() == 'a');
		UNIT_ASSERT("b", ms.ReadByte() == 'b');
		UNIT_ASSERT("c", ms.ReadByte() == 'c');
	}
	UNIT_ASSERT("Should be 0", ms.Length() == 0);
	UNIT_ASSERT("EOF", ms.ReadByte() == -1);
	
	ms.WriteByte('x');
	UNIT_ASSERT("Should be 1", ms.Length() == 1);
	int readlen = ms.Read(buf, 0, 2);
	UNIT_ASSERT("Should be one", readlen == 1);
	UNIT_ASSERT("x", buf[0] == 'x');
	UNIT_ASSERT("Should be 0", ms.Length() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ms.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("MemoryStream 2.2");
	
	Log::SWriteOkFail( "MemoryStream test 2" );
}

void TestStreams()
{
	_TestStream1();
	_TestMemoryStream();
	_TestMemoryStream2();
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
