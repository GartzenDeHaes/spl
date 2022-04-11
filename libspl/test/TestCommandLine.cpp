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
#include <spl/configuration/CommandLine.h>

#ifdef DEBUG

#include <spl/io/log/Log.h>

using namespace spl;

static void _TestCommandLine1()
{
	CommandLinePtr cl(CommandLine::Parse(0, NULL));
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	cl.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("CommandLine test 1.0");

	Log::SWriteOkFail( "CommandLine test 1" );
}

static void _TestCommandLine2()
{
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Test command line 2");

	const char *argv[] = {"prog", "--dir2=/bob/is/here"};

	CommandLinePtr cl(CommandLine::Parse(2, argv));
	cl.ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	cl.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("CommandLine test 2.1");

	UNIT_ASSERT( "arg count", 1 == cl->GetArgCount() );
	UNIT_ASSERT( "prog", cl->GetArg(0).Equals("prog") );
	UNIT_ASSERT( "dir2 doesn't exist", cl->HasSwitch("dir2") );
	UNIT_ASSERT( "dir2!=/bob/is/here", cl->GetSwitch("dir2").Equals("/bob/is/here") );

	Log::SWriteOkFail( "CommandLine test 2.0" );
}

static void _TestCommandLine3()
{
	const char *argv[] = {"prog", "-t=bob", "--dir=/mydir", "arg1", "--dir2=/bob/is/here"};

	CommandLinePtr cl(CommandLine::Parse(5, argv));

	cl.ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	cl.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("CommandLine test 3.0");

	UNIT_ASSERT( "arg count", 2 == cl->GetArgCount() );
	UNIT_ASSERT( "switch count", 3 == cl->GetSwitchCount() );
	UNIT_ASSERT( "prog", cl->GetArg(0).Equals("prog") );
	UNIT_ASSERT( "-t", cl->HasSwitch("t") );
	UNIT_ASSERT( "dir=/mydir", cl->HasSwitch("dir") );
	UNIT_ASSERT( "dir=/mydir", cl->GetSwitch("dir").Equals("/mydir") );
	UNIT_ASSERT( "dir2=/bob/is/here", cl->GetSwitch("dir2").Equals("/bob/is/here") );

	Log::SWriteOkFail( "CommandLine test 3" );
}

static void _TestCommandLine4()
{
	const char *argv[] = {"prog", "/t", "/dir", "\\mydir", "arg1", "/dir2:\\bob\\is\\here"};

	CommandLinePtr cl(CommandLine::Parse(6, argv));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	cl.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("CommandLine test 4.0");

	UNIT_ASSERT( "arg count", 3 == cl->GetArgCount() );
	UNIT_ASSERT( "prog", cl->GetArg(0).Equals("prog") );
	UNIT_ASSERT( "prog", cl->GetArg(1).Equals("\\mydir") );
	UNIT_ASSERT( "prog", cl->GetArg(2).Equals("arg1") );
	UNIT_ASSERT( "-t", cl->HasSwitch("t") );
	UNIT_ASSERT( "dir=/mydir", cl->HasSwitch("dir") );
	UNIT_ASSERT( "dir=/mydir", cl->GetSwitch("dir").Equals("") );
	UNIT_ASSERT( "dir2=/bob/is/here", cl->GetSwitch("dir2").Equals("\\bob\\is\\here") );

	Log::SWriteOkFail( "CommandLine test 4" );
}

static void _TestCommandLine5()
{
	const char *argv[] = {"/home/john/sources/spl/AnsiTerminal/.libs/lt-ansiterm", "localhost", "8011"};

	CommandLinePtr cl(CommandLine::Parse(3, argv));
	cl.ValidateMem();
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	cl.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("CommandLine test 5.0");

	UNIT_ASSERT( "arg count", 3 == cl->GetArgCount() );
	UNIT_ASSERT( "prog", cl->GetArg(0).Equals("/home/john/sources/spl/AnsiTerminal/.libs/lt-ansiterm") );
	UNIT_ASSERT( "localhost", cl->GetArg(1).Equals("localhost") );
	UNIT_ASSERT( "port", cl->GetArg(2).Equals("8011") );

	Log::SWriteOkFail( "CommandLine test 5" );
}

void _CommandLineRegression1()
{
	const char *argv[] = {"/home/john/sources/CoreServer/.libs/lt-RunTest"};
	CommandLinePtr cl(CommandLine::Parse(1, argv));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	cl.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("CommandLine regresstion 1.1");
		
	Log::SWriteOkFail( "CommandLine regression 1" );
}

void TestCommandLine()
{
	_TestCommandLine1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestCommandLine2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestCommandLine3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestCommandLine4();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestCommandLine5();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_CommandLineRegression1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
