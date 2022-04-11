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
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
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

#include <spl/io/Directory.h>
#include <spl/Environment.h>
#include <spl/io/File.h>
#include <spl/io/FileStream.h>
#include <spl/Int32.h>
#include <spl/io/WinPerm.h>

#ifdef _WINDOWS
extern bool EncodePermissions(const spl::Permissions& perm, _WinPerms &wperm);
extern bool DecodePermissions(_WinPerms &wperm, spl::Permissions &perm);
#include <Aclapi.h>
#else
extern void EncodePermissions(const spl::Permissions& perm, mode_t& mode);
extern void DecodePermissions(const mode_t mode, spl::Permissions& perm);
#endif

using namespace spl;

FILE *_msokfopen(const char* filename, const char* mode)
{
	#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		FILE* fp = 0;
		errno_t err = fopen_s(&fp, File::ToOsFilePath(filename)->GetChars(), mode);
		if (!err && fp)
			return fp;
		return NULL;
	#else
		return fopen( filename, mode );
	#endif
}

void File::Delete( const String& filename )
{
	if (remove( ToOsFilePath(filename)->GetChars() ) != 0)
	{
		throw new IOException(Environment::LastErrorMessage());
	}
}

void File::Rename(const String& oldname, const String& newname)
{
	if (rename(oldname.GetChars(), newname.GetChars()) != 0)
	{
		throw new IOException(Environment::LastErrorMessage());
	}
}

bool File::Exists( const String& filename )
{
	bool ret = false;
	FILE *fp = _msokfopen(filename.GetChars(), "r");
	if ( NULL != fp )
	{
		ret = true;
		fclose(fp);
	}
	return ret;
}

bool File::IsFile(const String& filename)
{
#ifdef WIN32

	struct _stat stbuf;
	if(::_stat(Directory::RemoveTrailingSeperator(filename)->GetChars(), &stbuf) != 0)
	{
		return false;
	}

	return (stbuf.st_mode & _S_IFREG) != 0;

#else

	struct stat stbuf;
	if(::stat(filename.GetChars(), &stbuf) != 0)
	{
		return false;
	}
	return S_ISREG(stbuf.st_mode);

#endif
}

spl::IStreamPtr File::Create( const String& filename )
{
	return FileStreamPtr(new FileStream(ToOsFilePath(filename)->GetChars(), File::FILEMODE_OpenOrCreate, File::FILEACC_ReadWrite));
}

void File::Copy( const String& sourcefile, const String& destfile )
{
	Copy(sourcefile, destfile, false);
}

void File::Copy( const String& sourcefile, const String& destfile, bool overwrite )
{
	if ( ! File::Exists(sourcefile) )
	{
		throw new InvalidArgumentException("Source file not found.");
	}
	if ( !overwrite && File::Exists(destfile) )
	{
		throw new IOException("Destination file exists");
	}

#ifdef _WINDOWS

	if(::CopyFile(sourcefile.GetChars(), destfile.GetChars(), overwrite ? FALSE : TRUE) != TRUE)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#else

	#define BUFSIZE 256

	FILE *fpfrom, *fpto;
	char mode[5];
	char buf[BUFSIZE];

	strcpy(mode, "r");
	if ( /*binary*/true )
	{
		strcat(mode, "b");
	}
	if ( NULL == (fpfrom = _msokfopen(sourcefile.GetChars(), mode)) )
	{
		throw new IOException(Environment::LastErrorMessage());
	}
	strcpy(mode, "w");
	if ( /*binary*/true )
	{
		strcat(mode, "b");
	}
	if ( NULL == (fpto = _msokfopen(destfile.GetChars(), mode)) )
	{
		fclose( fpfrom );
		throw new IOException(Environment::LastErrorMessage());
	}
	while ( ! feof( fpfrom ) )
	{
		size_t len = fread(buf, 1, BUFSIZE, fpfrom);
		if ( fwrite(buf, 1, len, fpto) != len )
		{
			fclose( fpto );
			fclose( fpfrom );
			throw new IOException(Environment::LastErrorMessage());
		}
	}
	fclose( fpto );
	fclose( fpfrom );

#endif
}

RefCountPtr<Array<spl::byte> > File::LoadBinary( const String& filename )
{
	int size;

#ifdef _WINDOWS
    struct _finddata_t c_file;
#if _MSC_VER >= 1400
    intptr_t hFile;
#else
    long hFile;
#endif
    if( (hFile = _findfirst( ToOsFilePath(filename)->GetChars(), &c_file )) == -1 )
	{
		return RefCountPtr<Array<byte> >();
	}
	size = c_file.size;
	_findclose( hFile );
#else
	struct stat st;
	stat(filename.GetChars(), &st);
	size = st.st_size;
#endif

	FILE *fp = _msokfopen(filename.GetChars(), "rb");
	if ( NULL == fp )
	{
		return RefCountPtr<Array<byte> >();
	}

	RefCountPtr<Array<byte> > buf( new Array<byte>(size) );
	if ( size != (int)fread(buf.Get(), 1, size, fp) )
	{
		return RefCountPtr<Array<byte> >();
	}
	return buf;
}

StringPtr File::LoadText(const String& filename)
{
	int size = Size(filename);
	if (size < 1)
	{
		size = 1024;
	}

	FILE *fp = _msokfopen(filename.GetChars(), "r");
	if ( NULL == fp )
	{
		return StringPtr();
	}

	const int cbufSize = 512;
	char cbuf[cbufSize];
	StringBuffer buf( size );
	int count;

	while ((count = (int)fread(cbuf, 1, cbufSize, fp) ) > 0)
	{
		buf.Append(cbuf, 0, count);
	}
	
	fclose(fp);
	
	return buf.ToString();
}

StringPtr File::ToOsFilePath( const String& filepathname )
{
#if defined(_WIN32) || defined(_WIN64)
	return filepathname.Replace('/', '\\');
#else
	StringPtr fpn = filepathname.Replace('\\', '/');
	if ( fpn->IndexOf(':') > -1 )
	{
		throw new InvalidArgumentException("Colons not allowed in unix file names");
	}
	return fpn;
#endif
}

StringPtr File::GetFileName( const String& filepathname )
{
	char sep[2];
	sep[0] = Directory::SeperatorChar();
	sep[1] = '\0';
	RefCountPtr<Vector<StringPtr> > parts = filepathname.Split(sep);
	if (parts->Count() == 0)
	{
		return StringPtr(new String());
	}
	return parts->ElementAt(parts->Count() -1);
}

TextWriterPtr File::AppendText( const String& filename )
{
	return TextWriterPtr(new TextWriter(FileStreamPtr(new FileStream(*ToOsFilePath(filename), File::FILEMODE_Append, File::FILEACC_Write))));
}

TextWriterPtr File::CreateText( const String& filename )
{
	return TextWriterPtr(new TextWriter( FileStreamPtr(new FileStream(*ToOsFilePath(filename), File::FILEMODE_Create, File::FILEACC_ReadWrite )) ));
}

TextReaderPtr File::ReadText( const String& filename )
{
	return TextReaderPtr(new TextReader( FileStreamPtr(new FileStream(*ToOsFilePath(filename), File::FILEMODE_Open, File::FILEACC_Read )) ));
}

spl::IStreamPtr File::Open( const String& filename, FileMode mode )
{
	return FileStreamPtr(new FileStream(*ToOsFilePath(filename), mode, File::FILEACC_ReadWrite ));
}

spl::IStreamPtr File::Open( const String& filename, FileMode mode, FileAccess access )
{
	return FileStreamPtr(new FileStream(*ToOsFilePath(filename), mode, access));
}

spl::IStreamPtr File::OpenRead( const String& filename )
{
	return FileStreamPtr(new FileStream(*ToOsFilePath(filename), File::FILEMODE_Open, File::FILEACC_Read));
}

spl::IStreamPtr File::OpenText( const String& filename )
{
	return FileStreamPtr(new FileStream(*ToOsFilePath(filename), File::FILEMODE_Open, File::FILEACC_ReadWrite));
}

spl::IStreamPtr File::OpenWrite( const String& filename )
{
	return FileStreamPtr(new FileStream(*ToOsFilePath(filename), File::FILEMODE_Open, File::FILEACC_Write ));
}

long File::Size(const String& filename)
{
	struct stat file_stats;
	if(0 != stat(ToOsFilePath(filename)->GetChars(), &file_stats))
    {
		throw new IOException(Environment::LastErrorMessage());
	}                
	return (long)file_stats.st_size;
}

DateTime File::GetCreationTime( const String& filename )
{
	struct stat file_stats;
	if(0 != stat(ToOsFilePath(filename)->GetChars(), &file_stats))
    {
		throw new IOException(Environment::LastErrorMessage());
	}                
	return DateTime(file_stats.st_ctime);
}

DateTime File::GetLastAccessTime( const String& filename )
{
	struct stat file_stats;
	if(0 != stat(ToOsFilePath(filename)->GetChars(), &file_stats))
    {
		throw new IOException(Environment::LastErrorMessage());
	}                
	return DateTime(file_stats.st_atime);
}

DateTime File::GetLastWriteTime( const String& filename )
{
	struct stat file_stats;
	if(0 != stat(ToOsFilePath(filename)->GetChars(), &file_stats))
    {
		throw new IOException(Environment::LastErrorMessage());
	}                
	return DateTime(file_stats.st_mtime);
}

Permissions File::GetPermissions(const String& filename)
{
	Permissions perms;

#ifdef _WINDOWS

	_WinPerms wperm; // needs members to be the above crap

	if(::GetNamedSecurityInfo((LPTSTR)(Directory::RemoveTrailingSeperator(filename)->GetChars()),
							SE_FILE_OBJECT,
							(OWNER_SECURITY_INFORMATION
							 | GROUP_SECURITY_INFORMATION
							 | DACL_SECURITY_INFORMATION),
							&wperm.user, &wperm.group, &wperm.dacl, NULL,
							&wperm.pdesc) != ERROR_SUCCESS)
	{
		throw new IOException(Environment::LastErrorMessage());
	}
	
	DecodePermissions(wperm, perms);

#else

	struct stat stbuf;

	if(::stat(filename.GetChars(), &stbuf) != 0)
	{
		throw new IOException("stat error");
	}

	DecodePermissions(stbuf.st_mode, perms);

#endif

	return perms;
}

void File::SetPermissions(const String& filename, const Permissions& perms)
{
#ifdef WIN32

	_WinPerms wperm;

	if(! EncodePermissions(perms, wperm))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	if(::SetFileSecurity(Directory::RemoveTrailingSeperator(filename)->GetChars(), DACL_SECURITY_INFORMATION, wperm.pdesc) != TRUE)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#else

	mode_t mode;
	EncodePermissions(perms, mode);

	if(::chmod(filename.GetChars(), mode) != 0)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#endif
}

void File::Move(const String &oldFile, const String &newFile)
{
#ifdef WIN32

	if(::MoveFile(Directory::RemoveTrailingSeperator(oldFile)->GetChars(), Directory::RemoveTrailingSeperator(newFile)->GetChars()) != TRUE)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

#else

	int r = ::link(oldFile.GetChars(), newFile.GetChars());
	if(r != 0)
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	if(::unlink(oldFile.GetChars()) != 0)
	{
		::unlink(newFile.GetChars());

		throw new IOException(Environment::LastErrorMessage());
	}

#endif
}
