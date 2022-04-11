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
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#endif

#include <spl/types.h>
#include <spl/Debug.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif
#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef HAVE_DIRECT_H
#include <direct.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif

#include <spl/Char.h>
#include <spl/io/Directory.h>
#include <spl/Environment.h>
#include <spl/io/File.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
#include <spl/text/Regex.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

String Environment::NewLine()
{
#ifdef _WINDOWS
	return "\r\n";
#elif defined(_MACOS)
	return "\r";
#else
	return "\n";
#endif
}

String Environment::ProcessName()
{
#ifdef _WINDOWS
	char  path[512] = { 0 };

	if (!GetModuleFileName(NULL, path, sizeof(path)) || !path[0])
	{
		return "unknown";
	}
	String fullName(path);
	return *fullName.Substring(fullName.LastIndexOf("\\")+1);
#else
	bool parsePath = false;
	pid_t pid = getpid();
	StringPtr spid(Int32::ToString((int)pid));
	String path = "/proc/" + *spid + "/execname";
	if (!File::Exists(path))
	{
		path = "/proc/" + *spid + "/cmdline";
		if (!File::Exists(path))
		{
			return "unknown";
		}
		parsePath = true;
	}
	TextReaderPtr reader = File::ReadText(path);
	StringPtr execname = reader->ReadLine();
	reader->Close();
	if (parsePath)
	{
		int pos = execname->LastIndexOf("/");
		if (pos > -1)
		{
			execname = execname->Substring(pos+1);
		}
		pos = execname->IndexOf(" ");
		if (0 > pos)
		{
			return *execname;
		}
		return *execname->Substring(pos+1);
	}
	else
	{
		return *execname->Substring(execname->LastIndexOf("/")+1);
	}
#endif
}

String Environment::GetVariable(const String& name)
{
#ifdef _WINDOWS
	DWORD len = ::GetEnvironmentVariable(name.GetChars(), NULL, 0);
	if(len == 0)
	{
		return String();
	}

	char *buf = new char[len];

	len = ::GetEnvironmentVariable(name.GetChars(), buf, len);

	String val(buf);
	delete [] buf;
	return val;

#else

	String val = ::getenv(name.GetChars());
	return(val);

#endif
}

#ifndef __TANDEM
void Environment::SetVariable(const String& name, const String& value)
{
	if(name.IndexOf('=') > -1)
	{
		return;
	}

#ifdef _WINDOWS

	::SetEnvironmentVariable(name.GetChars(), value.GetChars());

#else

	::setenv(name.GetChars(), value.GetChars(), 1);

#endif
}
#endif

String Environment::UserName()
{
#ifdef _WINDOWS
    char ubuf[256];

    DWORD size = (DWORD)sizeof(ubuf);
    if(::GetUserName(ubuf, &size) != TRUE)
	{
		throw new Exception("GetUserName error");
	}
	return String(ubuf);
#else
	passwd *pw = getpwuid(geteuid());
	if(NULL == pw)
	{
		throw new Exception("getpwuid error");
	}
	if (NULL == pw->pw_name)
	{
	    return String("unknown");
	}
	return String(pw->pw_name);
#endif
}

Vector<StringPtr> Environment::GetLogicalDrives(bool localOnly)
{
	Vector<StringPtr> drives;

#ifdef _WINDOWS

	DWORD mask = ::GetLogicalDrives();

	for(int i = 0; i < 26; i++)
	{
		if(mask & 0x01)
		{
			char path[4];

			sprintf(path, "%c:\\", 'A' + i);

			if(! localOnly || (::GetDriveType(path) == DRIVE_FIXED))
			{
				drives.Add( StringPtr(new String((const char *)path)) );
			}
		}

		mask >>= 1;
	}

#else

	drives.Add(StringPtr(new String("/")));

#endif

	return drives;
}

String Environment::HostName()
{
#ifdef _WINDOWS
	char hbuf[MAX_COMPUTERNAME_LENGTH + 1];

	DWORD size = (DWORD)sizeof(hbuf);
	if(::GetComputerName(hbuf, &size) != TRUE)
	{
		throw new Exception("GetComputerName error");
	}
	return String(hbuf);
#else
    struct utsname uts;
    uname(&uts);
    return String(uts.nodename);
#endif
}

String Environment::HostArchitecture()
{
#ifdef _WINDOWS
    SYSTEM_INFO sys_info;

    ::GetSystemInfo(&sys_info);

    switch(sys_info.dwProcessorType)
    {
      case PROCESSOR_INTEL_386:
      case PROCESSOR_INTEL_486:
      case PROCESSOR_INTEL_PENTIUM:
        return String("x86");

      case 2200:
        return String("ia64");

#ifdef PROCESSOR_AMD_X8664
      case PROCESSOR_AMD_X8664:
        return String("amd8664");
#endif

      default:
        return String("unknown");
    }

#else
	struct utsname uts;
	uname(&uts);
	return String(uts.machine);
#endif
}

String Environment::OSName()
{
#ifdef _WINDOWS
	return String("Windows");
#else
    struct utsname uts;
    uname(&uts);

	return String(uts.sysname);
#endif
}

String Environment::OSVersion()
{
#ifdef _WINDOWS
    OSVERSIONINFO os_info;

	os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx(&os_info);

    switch(os_info.dwMajorVersion)
    {
      case 4:
        switch(os_info.dwMinorVersion)
        {
          case 0:
            if(os_info.dwPlatformId == VER_PLATFORM_WIN32_NT)
              return String("NT 4.0");
            else
              return String("95");
            break;

          case 10:
            return String("98");

          case 90:
            return String("ME");

          default:
            return String("unknown");
        }
        break;

      case 5:
        switch(os_info.dwMinorVersion)
        {
          case 0:
            return String("2000");

          case 1:
            return String("XP");

          case 2:
            return String("Server 2003");

          default:
            return String("unknown");
        }
        break;

      case 6:
        return String("Vista");

	  case 7:
		  return String("7");

      default:
			return String("unknown");
    }
#else
	struct utsname uts;
	uname(&uts);
	return String(uts.release);
#endif
}

String Environment::HomeDirectory()
{
#ifdef _WINDOWS
	return String(Environment::GetVariable("USERPROFILE"));
#else
	passwd *pw = getpwuid(geteuid());
	if(! pw)
	{
		throw new Exception("getpwuid error");
	}
	return String(pw->pw_dir);
#endif
}

String Environment::TempDirectory()
{
#ifdef _WINDOWS
    DWORD tmpLen = ::GetTempPath(0, NULL);
    char *buf = new char[tmpLen];
    ::GetTempPath(tmpLen, buf);
	String path(buf);
	delete [] buf;
	return path;
#else
    const char *tmpEnv = getenv("TMPDIR");
	if (NULL != tmpEnv)
	{
		return String(tmpEnv);
	}
	tmpEnv = getenv("TMP");
	if (NULL == tmpEnv)
	{
		char *tpath = tempnam(NULL, NULL);
		if (NULL != tpath)
		{
			String ret(tpath);
			realfree(tpath);
			return ret;
		}
	}
	String dir = HomeDirectory() + *Char::ToString(Directory::SeperatorChar()) + "tmp";
	if (!Directory::IsDirectory(dir))
	{
		Directory::Create(dir);
	}
	return dir;
#endif
}

#ifndef __TANDEM
uint64 Environment::DiskFreeSpace()
{
#ifdef _WINDOWS

	ULARGE_INTEGER x, avail;

	if(::GetDiskFreeSpaceEx("\\", &x, &x,	&avail) == FALSE)
		return 0;

	return (uint64)avail.QuadPart;

#else

	struct statfs buf;
	if(::statfs("/", &buf) != 0)
		return 0;

	return (uint64)(buf.f_bavail) * 1024;

#endif
}
#endif

int Environment::LastError()
{
#ifdef _WINDOWS
	return GetLastError();
#else
	return errno;
#endif
}

String Environment::LastErrorMessage()
{
#ifdef _WINDOWS
	void *buf = NULL;
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,GetLastError(),0,
				  (LPTSTR) &buf,0,NULL);
	String ret((char *)buf);

	// Free the buffer
	::LocalFree(buf);

	return ret;
#else
#ifdef HAVE_STRERROR_R
	char buf[256];
	buf[0] = '\0';
	strerror_r(errno, buf, 256);
	if ('\0' == buf[0])
	{
		return *(String::Format("Error number %d", errno));
	}
	return String(buf);
#else
	return String(strerror(errno));
#endif
#endif
}

int Environment::ProcessorCount()
{
#ifdef _WINDOWS
	SYSTEM_INFO si;
	::GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
#else
	const char *procCpuInfo = "/proc/cpuinfo";
	if (File::Exists(procCpuInfo))
	{
		Regex regex("^processor", RegexOptions::MULTILINE);
		StringPtr cpuinfo = File::LoadText(procCpuInfo);
		if (cpuinfo.IsNull())
		{
			return 1;
		}
		RefCountPtr<Array<StringPtr> > matches = regex.Matches(cpuinfo);
		return matches->Count();
	}
	return 1;
#endif
}

static StringPtr _EnvGetProcFile(const char* name)
{
	if (File::Exists(name))
	{
		return File::LoadText(name);
	}

	return StringPtr();
}

static int64 _EnvGetValFromMemInfo(const char* key)
{
	StringPtr meminfo = _EnvGetProcFile("/proc/meminfo");
	if (meminfo.IsNull())
	{
		return 0L;
	}
	int pos = meminfo->IndexOf(key);
	if (pos < 0)
	{
		return 0L;
	}
	StringPtr part(meminfo->Substring(pos)->Trim());
	pos = part->IndexOf(' ');
	StringPtr val(part->Substring(pos + 1)->Trim());
	pos = val->IndexOf(' ');
	val = val->Substring(0, pos);
	return Int64::Parse(val);

	return 0L;
}

int64 Environment::FreeMemory()
{
#ifdef _WINDOWS
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&statex);

	return statex.ullAvailPhys;
#else
	return _EnvGetValFromMemInfo("MemFree:") +
	_EnvGetValFromMemInfo("Buffers:") +
	_EnvGetValFromMemInfo("Cached:");
#endif
}

int64 Environment::TotalMemory()
{
#ifdef _WINDOWS
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&statex);

	return statex.ullTotalPhys;
#else
	return _EnvGetValFromMemInfo("MemTotal:");
#endif
}

int64 Environment::FreeVirtualMemory()
{
#ifdef _WINDOWS
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&statex);

	return statex.ullAvailVirtual;
#else
	return _EnvGetValFromMemInfo("SwapFree:");
#endif
}

String Environment::OperatingSystem()
{
#ifdef _WINDOWS
	OSVERSIONINFOEX info;
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((LPOSVERSIONINFO)&info);

	StringBuffer buf;
	buf.Append("Windows ");

	if (info.dwMajorVersion == 6)
	{
		if (info.dwMinorVersion == 0)
		{
			if (info.wProductType == VER_NT_WORKSTATION)
			{
				buf.Append(" VISTA");
			}
			else
			{
				buf.Append(" Server 2008");
			}
		}
		else
		{
			if (info.wProductType == VER_NT_WORKSTATION)
			{
				buf.Append(" 7");
			}
			else
			{
				buf.Append(" Server 2008 R2");
			}
		}
	}
	else if (info.dwMajorVersion == 5)
	{
		if (info.dwMinorVersion == 0)
		{
			buf.Append(" 2000");
		}
		else if (info.dwMinorVersion == 1)
		{
			buf.Append(" XP");
		}
		else
		{
			buf.Append(" Server 2003");
		}
	}

	buf.Append(" SP ");
	buf.Append(Int32::ToString(info.wServicePackMajor).Get());

	buf.Append(' ');
	buf.Append(Int32::ToString(info.dwMajorVersion).Get());
	buf.Append('.');
	buf.Append(Int32::ToString(info.dwMinorVersion).Get());

	return String(buf.GetChars());
#else
	return String(_EnvGetProcFile("/proc/version"));
#endif
}

