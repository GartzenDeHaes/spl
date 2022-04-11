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
#ifndef _cfile_h
#define _cfile_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/DateTime.h>
#include <spl/Memory.h>
#include <spl/io/IStream.h>
#include <spl/io/Permissions.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/io/TextStream.h>

namespace spl
{
/** 
 * @defgroup file File System
 * @ingroup io
 * @{
 */

/** @brief Static file operations.
 */
class File
{
private:
	inline File() {}
	inline File& operator =(const File&) { return *this; }

public:
	typedef enum _FileMode
	{
		FILEMODE_Append = 6,
		FILEMODE_Create = 2,
		FILEMODE_CreateNew = 1,
		FILEMODE_Open = 3,
		FILEMODE_OpenOrCreate = 4,
		FILEMODE_Truncate = 5
	} FileMode;

	typedef enum _FileAccess
	{
		FILEACC_Read = 0x1,
		FILEACC_Write = 0x2,
		FILEACC_ReadWrite = FILEACC_Read | FILEACC_Write
	} FileAccess;

protected:
public:
	static void Delete(const String& filename);
	static void Rename(const String& oldname, const String& newname);
	static void Move(const String &oldFile, const String &newFile);
	
	static bool Exists(const String& filename);
	
	static void Copy( const String& sourcefile, const String& destfile );
	static void Copy( const String& sourcefile, const String& destfile, bool overwrite );

	static RefCountPtr<Array<byte> > LoadBinary( const String& filename );
	static StringPtr LoadText( const String& filename );

	static StringPtr ToOsFilePath( const String& filepathname );
	static StringPtr GetFileName( const String& filepathname );

	static TextWriterPtr AppendText( const String& filename );
	static IStreamPtr Create( const String& filename );
	static TextWriterPtr CreateText( const String& filename );
	static TextReaderPtr ReadText( const String& filename );
	static IStreamPtr Open( const String& filename, FileMode mode );
	static IStreamPtr Open( const String& filename, FileMode mode, FileAccess access );
	static IStreamPtr OpenRead( const String& filename );
	static IStreamPtr OpenText( const String& filename );
	static IStreamPtr OpenWrite( const String& filename );

	static long Size( const String& filename );
	static DateTime GetCreationTime( const String& filename );
	static DateTime GetLastAccessTime( const String& filename );
	static DateTime GetLastWriteTime( const String& filename );

	static bool IsFile(const String& filename);

	static Permissions GetPermissions(const String& filename);
	static void SetPermissions(const String& filename, const Permissions& perms);
};

REGISTER_TYPEOF(90, File);

/** @} */
}
#endif
