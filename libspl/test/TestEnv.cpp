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
#include <spl/Environment.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifdef DEBUG

static void _TestEnvVariable()
{
#ifndef __TANDEM
	Environment::SetVariable("_SPLTEST", "YES");
	UNIT_ASSERT("Should be YES", Environment::GetVariable("_SPLTEST") == "YES");

	Log::SWriteOkFail( "Environment GetVariable test" );
#endif
}

static void _TestEnvGetDrives()
{
	Vector<StringPtr> driveList = Environment::GetLogicalDrives(true);
	UNIT_ASSERT("Should be one or more drives", driveList.Count() > 0);

	if (driveList.Count() == 1)
	{
		UNIT_ASSERT("Should be /", *driveList.ElementAt(0) == "/" || driveList.ElementAt(0)->EndsWith(":\\"));
	}
	else if (driveList.Count() > 1)
	{
		UNIT_ASSERT("Should be c:", driveList.ElementAt(0)->EndsWith(":\\"));
	}

	Log::SWriteOkFail( "Environment GetLogicalDrives test" );
}

static void _TestEnvHostName()
{
	UNIT_ASSERT("Should be not null", Environment::HostName().Length() > 0);

	Log::SWriteOkFail( "Environment HostName test" );
}

static void _TestEnvHostArchitecture()
{
	UNIT_ASSERT("Should be not null", Environment::HostArchitecture().Length() > 0);

	Log::SWriteOkFail( "Environment HostArchitecture test" );
}

static void _TestEnvOSName()
{
	UNIT_ASSERT("Should be not null", Environment::OSName().Length() > 0);

	Log::SWriteOkFail( "Environment OSName test" );
}

static void _TestEnvOSVersion()
{
	UNIT_ASSERT("Should be not null", Environment::OSVersion().Length() > 0);

	Log::SWriteOkFail( "Environment OSVersion test" );
}

static void _TestEnvUserName()
{
	UNIT_ASSERT("Should be not null", Environment::UserName().Length() > 0);

	Log::SWriteOkFail( "Environment UserName test" );
}

static void _TestEnvHomeDirectory()
{
	UNIT_ASSERT("Should be not null", Environment::HomeDirectory().Length() > 0);

	Log::SWriteOkFail( "Environment HomeDirectory test" );
}

static void _TestEnvTempDirectory()
{
	UNIT_ASSERT("Temp direcotry should be greater than zero", Environment::TempDirectory().Length() > 0);

	Log::SWriteOkFail( "Environment TempDirectory test" );
}

static void _TestEnvDiskFreeSpace()
{
#ifndef __TANDEM
	UNIT_ASSERT("Should be not 0", Environment::DiskFreeSpace() > 0);

	Log::SWriteOkFail( "Environment DiskFreeSpace test" );
#endif
}

static void _TestEnvProcessorCount()
{
	UNIT_ASSERT("Should be not 0", Environment::ProcessorCount() > 0);

	Log::SWriteOkFail( "Environment ProcessorCount test" );
}

static void _TestEnvProcName()
{
	String procName = Environment::ProcessName();
	UNIT_ASSERT("Should be not null", procName.Length() > 0);
#ifdef _WINDOWS
	UNIT_ASSERT("Should end with .exe", procName.EndsWith(".exe"));
#endif
	UNIT_ASSERT("Should find something", !procName.Equals("unknown"));
	UNIT_ASSERT("Shouldn't have path", procName.IndexOf('/') < 0);
	UNIT_ASSERT("Shouldn't have path", procName.IndexOf('\\') < 0);

	Log::SWriteOkFail( "Environment ProcessName test" );
}

static void _TestEnvFreeMem()
{
	UNIT_ASSERT("Should be not 0", Environment::FreeMemory() > 0);

	Log::SWriteOkFail( "Environment::FreeMemory test" );
}

static void _TestEnvTotalem()
{
	UNIT_ASSERT("Should be not 0", Environment::TotalMemory() > 0);

	Log::SWriteOkFail( "Environment::TotalMemory test" );
}

static void _TestEnvFreeVirt()
{
	// NOTE: might not have a swap file anymore.
	UNIT_ASSERT("Should be not 0", Environment::FreeVirtualMemory() >= 0);

	Log::SWriteOkFail( "Environment::FreeVirtualMemory test" );
}

static void _TestEnvOsString()
{
	UNIT_ASSERT("Should be not 0", Environment::OperatingSystem().Length() > 0);

	Log::SWriteOkFail( "Environment::OperatingSystem test" );
}

void _TestEnv()
{
	_TestEnvVariable();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvGetDrives();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvHostName();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvHostArchitecture();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvOSName();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvOSVersion();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvUserName();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvHomeDirectory();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvTempDirectory();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvDiskFreeSpace();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvProcessorCount();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvProcName();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvFreeMem();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvTotalem();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvFreeVirt();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestEnvOsString();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
