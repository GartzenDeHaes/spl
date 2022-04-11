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
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <spl/Environment.h>
#include <spl/io/Directory.h>
#include <spl/io/File.h>
#include <spl/io/WinPerm.h>

#include <stdio.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
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
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

using namespace spl;

#ifdef WIN32
char Directory::m_seperator = '\\';
//#include <spl/cleanwindows.h>
#else
char Directory::m_seperator = '/';
#endif

#ifdef WIN32
extern bool EncodePermissions(const Permissions& perm, _WinPerms &wperm);
#else
extern void EncodePermissions(const Permissions& perm, mode_t& mode);
#endif

Vector<StringPtr> Directory::ListFiles(const String& directory, const String& filter)
{
	Vector<StringPtr> list;

#ifdef WIN32
    struct _finddata_t c_file;

#if _MSC_VER >= 1400
    intptr_t hFile;
#else
    long hFile;
#endif

	char odir[512];

	_getcwd(odir, 512);

	if ( 0 != _chdir(directory.GetChars()) )
	{
		return list;
	}

    /* Find first .c file in current directory */
    if( (hFile = _findfirst( filter.GetChars(), &c_file )) == -1L )
	{
		return list;
	}

	list.Add( StringPtr(new String(c_file.name)) );
	while( _findnext( hFile, &c_file ) == 0 )
	{
		list.Add( StringPtr(new String(c_file.name)) );
	}
	_findclose( hFile );

	if (0 != _chdir(odir))
	{
		throw new IOException("Cannot change directory to " + String(odir));
	}

#else

	DIR *dp;
	struct dirent *ep;

	dp = opendir (directory.GetChars());
	if (dp != NULL)
	{
		while (ep = readdir (dp))
		{
			list.Add( StringPtr(new String(ep->d_name)) );
		}
		closedir (dp);
	}

#endif
	return list;
}

void Directory::ChangeWorkingDirectory(const String& directory)
{
#ifdef WIN32
	if ( 0 != _chdir(directory.GetChars()) )
	{
		throw new InvalidArgumentException(Environment::LastErrorMessage());
	}
#else
	if ( 0 != chdir(directory.GetChars()) )
	{
		throw new InvalidArgumentException(Environment::LastErrorMessage());
	}
#endif
}

StringPtr Directory::GetWorkingDirectory()
{
	char workingDir[512];

#ifdef _WINDOWS
	return StringPtr(new String(_getcwd(workingDir, 512)));
#else
	return StringPtr(new String(getcwd(workingDir, 512)));
#endif
}

Directory::Directory(const String& directory)
: m_raw(directory), m_parts()
{
	char sep[2];
	sep[0] = m_seperator;
	sep[1] = '\0';
	m_parts = m_raw.Split(sep);
	if (m_parts->Count() > 0)
	{
		StringPtr fileName = m_parts->ElementAt(m_parts->Count() - 1);
		if (fileName->Length() == 0 || File::Exists(fileName))
		{
			m_parts->RemoveAt(m_parts->Count() - 1);
		}
	}
}

Directory::~Directory()
{
}

Directory::Directory( const Directory& dir )
: m_raw(), m_parts()
{
	*this = dir;
}

Directory& Directory::operator =(const Directory& dir)
{
	m_parts->Clear();

	char sep[2];
	sep[0] = m_seperator;
	sep[1] = '\0';
	m_raw = dir.m_raw;
	m_parts = dir.m_parts;

	return *this;
}

Directory Directory::Parent() const
{
	StringBuffer buf(m_raw.Length());
	for (int x = 0; x < m_parts->Count() - 1; x++)
	{
		buf.Append(m_parts->ElementAt(x));
		buf.Append(Directory::SeperatorChar());
	}

	return Directory(buf.ToString());
}

uint32 Directory::HashCode() const
{
	return m_raw.HashCode();
}

bool Directory::Equals( const IComparable& a ) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Directory& dir = static_cast<const Directory &>(a);
	return m_raw.Equals( dir.m_raw );
}

int Directory::Compare( const IComparable& a ) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Directory& dir = static_cast<const Directory &>(a);
	return m_raw.Compare( dir.m_raw );
}

int32 Directory::MajicNumber() const
{
	return 0x00F1;
}

StringPtr Directory::RemoveTrailingSeperator(const String& path)
{
	if (path.CharAt(path.Length() - 1) == m_seperator)
	{
		return path.Substring(0, path.Length() - 1);
	}
	return StringPtr(new String(path));
}

StringPtr Directory::ToString() const
{
	StringBuffer buf(m_raw.Length());

	for (int x = 0; x < m_parts->Count(); x++)
	{
		buf.Append(m_parts->ElementAt(x));
		buf.Append(Directory::SeperatorChar());
	}

	return buf.ToString();
}

bool Directory::IsDirectory(const String &path)
{
#ifdef _WINDOWS

	struct _stat stbuf;

	if (_stat(RemoveTrailingSeperator(path)->GetChars(), &stbuf) != 0)
	{
		return false;
	}

	return (stbuf.st_mode & _S_IFDIR) != 0;

#else

	struct stat stbuf;

	if(stat(path.GetChars(), &stbuf) != 0)
	{
		return false;
	}
	return S_ISDIR(stbuf.st_mode);

#endif
}

void Directory::Delete(const String& path)
{
#ifdef WIN32

	if(::RemoveDirectory(RemoveTrailingSeperator(path)->GetChars()) != TRUE)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#else

	if(::rmdir(path.GetChars()) != 0)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#endif
}

void Directory::Rename(const String& oldname, const String& newname)
{
	if (rename(oldname.GetChars(), newname.GetChars()) != 0)
	{
		throw new IOException(Environment::LastErrorMessage());
	}
}

void Directory::Create(const String& path, const Permissions& perm)
{
	if(path.Length() < 1)
	{
		throw new InvalidArgumentException("Path is required");
	}

#ifdef _WINDOWS

	SECURITY_ATTRIBUTES sa;
	_WinPerms wperm;

	EncodePermissions(perm, wperm);

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = wperm.pdesc;
	sa.bInheritHandle = FALSE;

	if (::CreateDirectory(RemoveTrailingSeperator(path)->GetChars(), &sa) != TRUE)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#else

	mode_t mode;
	EncodePermissions(perm, mode);

	if(::mkdir(path.GetChars(), mode) != 0)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#endif
}

#ifdef DEBUG
void Directory::ValidateMem() const
{
	m_raw.ValidateMem();
	m_parts.ValidateMem();
}

void Directory::CheckMem() const
{
	m_raw.ValidateMem();
	m_parts.CheckMem();
}
#endif
