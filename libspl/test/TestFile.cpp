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
#include <spl/io/log/Log.h>
#include <spl/io/File.h>

using namespace spl;

#ifdef DEBUG

static void _TestFile1()
{
	String filename("filetest.txt");
	const char *line1 = "The time has come";
	const char *line2 = ", said the Walrus, ";

	if (File::Exists(filename))
	{
		File::Delete(filename);
	}
	if (File::Exists("filetest2.txt"))
	{
		File::Delete("filetest2.txt");
	}
	if (File::Exists("filetest3.txt"))
	{
		File::Delete("filetest3.txt");
	}

	TextWriterPtr writer = File::CreateText(filename);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	filename.CheckMem();
	writer.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("File test 1.0");

	writer->WriteLine(line1);
	writer->Close();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	filename.CheckMem();
	writer.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("File test 1.1");

	UNIT_ASSERT("File::Exists", File::Exists(filename));
	UNIT_ASSERT("File::Size", File::Size(filename) > 17);
	UNIT_ASSERT("File::GetCreationTime", File::GetCreationTime(filename) <= DateTime::Now());
	UNIT_ASSERT("File::GetLastAccessTime", File::GetLastAccessTime(filename) <= DateTime::Now());
	UNIT_ASSERT("File::GetLastWriteTime", File::GetLastWriteTime(filename) <= DateTime::Now());
	UNIT_ASSERT("File::IsFile", File::IsFile(filename));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	filename.CheckMem();
	writer.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("File test 1.2");

	TextReader *reader = new TextReader(File::OpenText(filename));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	filename.CheckMem();
	DEBUG_NOTE_MEM(reader);
	reader->CheckMem();
	writer.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("File test 1.3");

	StringPtr line = reader->ReadLine();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	filename.CheckMem();
	DEBUG_NOTE_MEM(reader);
	reader->CheckMem();
	line.CheckMem();
	writer.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("File test 1.4");

	UNIT_ASSERT("Reread line 1", line->Equals(line1));

	reader->Close();
	delete reader;

	writer = File::AppendText(filename);
	writer->WriteLine(line2);
	writer->Close();

	reader = new TextReader(File::OpenText(filename));
	reader->ReadLine();
	line = reader->ReadLine();
	reader->Close();
	delete reader;

	UNIT_ASSERT("Reread line 2", line->Equals(line2));

	File::Rename(filename, "filetest2.txt");

	UNIT_ASSERT("File::Rename", !File::Exists(filename));
	filename = "filetest2.txt";
	UNIT_ASSERT("File::Rename", File::Exists(filename));

	File::Move(filename, "filetest3.txt");
	UNIT_ASSERT("File::Move", !File::Exists(filename));
	filename = "filetest3.txt";
	UNIT_ASSERT("File::Move", File::Exists(filename));

	Permissions perms = File::GetPermissions(filename);
	UNIT_ASSERT("File::GetPermissions", perms.CanUserRead());
	UNIT_ASSERT("File::GetPermissions", perms.CanUserWrite());

	perms.SetOthersRead();
	File::SetPermissions(filename, perms);

	File::Delete(filename);
	UNIT_ASSERT("File::Delete", !File::Exists(filename));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	filename.CheckMem();
	writer.CheckMem();
	line.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("File test 1.5");

	Log::SWriteOkFail( "File test 1" );
}

void _TestFile()
{
	_TestFile1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif

