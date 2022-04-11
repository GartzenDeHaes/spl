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
#ifndef _environment_h
#define _environment_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
/**
 * @defgroup env Environment
 * @ingroup core
 * @{
 */

/** @brief Static class to get inforamtion about the local machine and environment variables.
 *
 */
class Environment
{
private:
	Environment();

public:
	static String NewLine();
	static String GetVariable(const String& name);
#ifndef __TANDEM
	static void SetVariable(const String& name, const String& value);
#endif
	static Vector<StringPtr> GetLogicalDrives(bool localOnly);
	static String ProcessName();
	static String HostName();
	static String HostArchitecture();
	static String OSName();
	static String OSVersion();
	static String UserName();
	static String HomeDirectory();
	static String TempDirectory();
	static int ProcessorCount();
	static int LastError();
	static String LastErrorMessage();
	static int64 FreeMemory();
	static int64 TotalMemory();
	static int64 FreeVirtualMemory();
	static String OperatingSystem();
#ifndef __TANDEM
	static uint64 DiskFreeSpace();
#endif
};

/** @} */
}
#endif
