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
#include <spl/data/Connection.h>

#ifdef DEBUG

#include <spl/io/log/Log.h>

using namespace spl;

static void _TestCommand1()
{
	Command *cmd = new Command("text");
	UNIT_ASSERT("command text", cmd->CommandTextGet().Equals("text"));
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(cmd);
	cmd->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Command 1.2");

	delete cmd;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Command 1.2");
	Log::SWriteOkFail( "Command 1" );
}

static void _TestCommand2()
{
	Command *cmd = new Command("text");
	cmd->CreateParameter("c1", 1);
	UNIT_ASSERT("prm count", cmd->ParameterCount() == 1);

	UNIT_ASSERT("command text", cmd->CommandTextGet().Equals("text"));
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(cmd);
	cmd->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Command 2.1");

	UNIT_ASSERT("c1==1", cmd->GetParameter("c1")->GetInt32() == 1 );
	UNIT_ASSERT("prm count", cmd->ParameterCount() == 1);

	delete cmd;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Command 2.2");
	Log::SWriteOkFail( "Command 2" );
}

void _TestCommand()
{
	_TestCommand1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestCommand2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
