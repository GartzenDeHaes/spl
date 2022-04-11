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
#include <spl/io/Stream.h>
#ifdef _stream2_h
/* _stream2_h is in Stream.h, only compile this file if using the new Stream.h header */

#ifndef _filestream_h
#define _filestream_h

#include <spl/io/File.h>
#include <spl/RefCountPtr.h>

namespace spl
{

/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class FdStream;
typedef RefCountPtrCast<FdStream, IStream, IStreamPtr> FdStreamPtr;
typedef WeakReference<FdStream, IStreamPtr> FdStreamRef;

/** @brief Low level file access using open, read, write
  *
  */
class FdStream : public IStream
{
protected:
	IStreamState *m_state;
	String m_filepathname;
	int m_fd;
	bool m_canClose;
	File::FileMode m_mode;
	File::FileAccess m_access;
	int m_lasterr;

public:
	FdStream(int fd, File::FileAccess access, bool canClose = false);
	FdStream(const String& path, File::FileMode mode, File::FileAccess access);
	virtual ~FdStream();

	virtual void Close();
	virtual void Flush();
	virtual int Read(Array<byte>& buffer, const int offset, int count);
	virtual int ReadByte();
	virtual long Seek(const long offset, const SeekOrigin origin);
	virtual void Write(const Array<byte>& buffer, const int offset, const int count);
	virtual void WriteByte(byte value);

	virtual bool CanRead() const;
	virtual bool CanSeek() const;
	virtual bool CanWrite() const;

	virtual long Length() const;
	virtual long Position() const;

	friend class FdStreamState_Open;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

class FileStream;
typedef RefCountPtrCast<FileStream, IStream, IStreamPtr> FileStreamPtr;
typedef WeakReference<FileStream, IStreamPtr> FileStreamRef;

/** STDIO file stream.
 *
 */
class FileStream : public IStream
{
private:
	void Init();

protected:
	IStreamState *m_state;
	String m_filepathname;
	FILE *m_fp;
	bool m_canClose;
	File::FileMode m_mode;
	File::FileAccess m_access;
	int m_lasterr;

public:
	FileStream(FILE *fp, File::FileAccess access, bool canClose = false);
	FileStream(const char *path, File::FileMode mode, File::FileAccess access);
	FileStream(const String& path, File::FileMode mode, File::FileAccess access);
	virtual ~FileStream();

	virtual void Close();
	virtual void Flush();
	virtual int Read(Array<byte>& buffer, const int offset, int count);
	virtual int ReadByte();
	virtual long Seek(const long offset, const SeekOrigin origin);
	virtual void Write(const Array<byte>& buffer, const int offset, const int count);
	virtual void WriteByte(byte value);

	virtual bool CanRead() const;
	virtual bool CanSeek() const;
	virtual bool CanWrite() const;

	virtual long Length() const;
	virtual long Position() const;

	friend class FileStreamState_Open;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}

#endif
#endif
