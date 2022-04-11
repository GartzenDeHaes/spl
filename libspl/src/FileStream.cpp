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
#include <spl/configwin32.h>
#include <stdio.h>
#else
#include <spl/autoconf/config.h>
#endif
#include <stdarg.h>
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#include <spl/io/FileStream.h>

extern FILE *_msokfopen(const char* filename, const char* mode);

namespace spl
{
	class FileStreamState_Closed : public IStreamState
	{
	public:
		FileStreamState_Closed(IStream* parent, IStreamState **parentStateVar);
		virtual ~FileStreamState_Closed();

		virtual void OnLeave(IStreamState *newState);
		virtual void OnEnter(IStreamState *oldState);

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
	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

	FileStreamState_Closed::FileStreamState_Closed
	(
		IStream *parent, 
		IStreamState **parentStateVar
	)
	: IStreamState(parent, parentStateVar)
	{
	}

	FileStreamState_Closed::~FileStreamState_Closed()
	{
	}

	void FileStreamState_Closed::OnLeave(IStreamState *newState)
	{
	}

	void FileStreamState_Closed::OnEnter(IStreamState *oldState)
	{
	}

	void FileStreamState_Closed::Close()
	{
	}

	void FileStreamState_Closed::Flush()
	{
		throw new IOException("File is closed, so it can't be flushed");
	}

	int FileStreamState_Closed::Read(Array<byte>& buffer, const int offset, int count)
	{
		throw new IOException("File is closed, so it can't be read");
	}

	int FileStreamState_Closed::ReadByte()
	{
		throw new IOException("File is closed, so it can't be read");
	}

	long FileStreamState_Closed::Seek(const long offset, const SeekOrigin origin)
	{
		throw new IOException("File is closed, so it can't be seeked");
	}

	void FileStreamState_Closed::Write(const Array<byte>& buffer, const int offset, const int count)
	{
		throw new IOException("File is closed, so it can't be written");
	}

	void FileStreamState_Closed::WriteByte(byte value)
	{
		throw new IOException("File is closed, so it can't be written");
	}

	bool FileStreamState_Closed::CanRead() const
	{
		throw new NotImplementedException();
	}

	bool FileStreamState_Closed::CanSeek() const
	{
		throw new NotImplementedException();
	}

	bool FileStreamState_Closed::CanWrite() const
	{
		throw new NotImplementedException();
	}

	long FileStreamState_Closed::Length() const
	{
		throw new IOException("File is closed");
	}

	long FileStreamState_Closed::Position() const
	{
		throw new IOException("File is closed");
	}

	#ifdef DEBUG
	void FileStreamState_Closed::ValidateMem() const
	{
	}

	void FileStreamState_Closed::CheckMem() const
	{
	}
	#endif

	class FileStreamState_Open : public IStreamState
	{
	public:
		FileStreamState_Open(IStream *parent, IStreamState **parentStateVar);
		virtual ~FileStreamState_Open();

		virtual void OnLeave(IStreamState *newState);
		virtual void OnEnter(IStreamState *oldState);

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
	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

	FileStreamState_Open::FileStreamState_Open
	(
		IStream *parent, 
		IStreamState **parentStateVar
	)
	: IStreamState(parent, parentStateVar)
	{
	}

	FileStreamState_Open::~FileStreamState_Open()
	{
	}

	void FileStreamState_Open::OnLeave(IStreamState *newState)
	{
		if ( ((FileStream *)m_parent)->m_canClose )
		{
			fclose( ((FileStream *)m_parent)->m_fp );
		}
	}

	void FileStreamState_Open::OnEnter(IStreamState *oldState)
	{
	}

	void FileStreamState_Open::Close()
	{
		IStreamState *newstate = new FileStreamState_Closed(m_parent, m_holder);
		ChangeState(newstate);
	}

	void FileStreamState_Open::Flush()
	{
		fflush( ((FileStream *)m_parent)->m_fp);
	}

	int FileStreamState_Open::Read(Array<byte>& buffer, const int offset, int count)
	{
		if (feof(((FileStream *)m_parent)->m_fp))
		{
			return -1;
		}
		int ret = (int)fread(&buffer[offset], 1, count, ((FileStream *)m_parent)->m_fp);
		((FileStream *)m_parent)->m_lasterr = errno;
		return ret;
	}

	int FileStreamState_Open::ReadByte()
	{
		return fgetc(((FileStream *)m_parent)->m_fp);
	}

	long FileStreamState_Open::Seek(const long offset, const SeekOrigin origin)
	{
		if ( origin == FileStream::SEEK_Current )
		{
			return fseek(((FileStream *)m_parent)->m_fp, offset, SEEK_CUR);
		}
		else if ( origin == FileStream::SEEK_Begin )
		{
			return fseek(((FileStream *)m_parent)->m_fp, offset, SEEK_SET);
		}
		else
		{
			return fseek(((FileStream *)m_parent)->m_fp, offset, SEEK_END);
		}
	}

	void FileStreamState_Open::Write(const Array<byte>& buffer, const int offset, const int count)
	{
		fwrite(&buffer[offset], 1, count, ((FileStream *)m_parent)->m_fp);
		((FileStream *)m_parent)->m_lasterr = errno;
	}

	void FileStreamState_Open::WriteByte(byte value)
	{
		fputc(value, ((FileStream *)m_parent)->m_fp);
		((FileStream *)m_parent)->m_lasterr = errno;
	}

	bool FileStreamState_Open::CanRead() const
	{
		throw new NotImplementedException();
	}

	bool FileStreamState_Open::CanSeek() const
	{
		throw new NotImplementedException();
	}

	bool FileStreamState_Open::CanWrite() const
	{
		throw new NotImplementedException();
	}

	long FileStreamState_Open::Length() const
	{
		long oldpos = ftell(((FileStream *)m_parent)->m_fp);
		fseek(((FileStream *)m_parent)->m_fp, 0, SEEK_END);
		long size = ftell(((FileStream *)m_parent)->m_fp);
		fseek(((FileStream *)m_parent)->m_fp, oldpos, SEEK_SET);
		return size;
	}

	long FileStreamState_Open::Position() const
	{
		return ftell(((FileStream *)m_parent)->m_fp);
	}

	#ifdef DEBUG
	void FileStreamState_Open::ValidateMem() const
	{
	}

	void FileStreamState_Open::CheckMem() const
	{
	}
	#endif

	FileStream::FileStream(FILE *fp, File::FileAccess access, bool canClose)
	: m_filepathname(""), m_mode(File::FILEMODE_Open), m_access(access), m_canClose(canClose), m_lasterr(0), m_state(NULL)
	{
		m_fp = fp;
		m_state = new FileStreamState_Open(this, &m_state);
	}

	void FileStream::Init()
	{
		if ( File::Exists(m_filepathname) )
		{
			if ( File::FILEMODE_CreateNew == m_mode )
			{
				throw new IOException("File exists");
			}
			if ( File::FILEMODE_Truncate == m_mode )
			{
				if ( NULL != (m_fp = _msokfopen(m_filepathname.GetChars(), "w")) )
				{
					fclose(m_fp);
				}
			}
		}
		else if ( File::FILEMODE_Open == m_mode )
		{
			throw new IOException("File not found");
		}
		else
		{
			// create the file
			if ( NULL != (m_fp = _msokfopen(m_filepathname.GetChars(), "a")) )
			{
				fclose(m_fp);
			}
		}
		if ( File::FILEMODE_Append == m_mode )
		{
			if ( File::FILEACC_Read == m_access )
			{
				m_fp = _msokfopen(m_filepathname.GetChars(), "a+b");
			}
			else
			{
				m_fp = _msokfopen(m_filepathname.GetChars(), "ab");
			}
		}
		else
		{
			if ( File::FILEACC_Read == m_access )
			{
				m_fp = _msokfopen(m_filepathname.GetChars(), "rb");
			}
			else if ( File::FILEACC_Write == m_access )
			{
				m_fp = _msokfopen(m_filepathname.GetChars(), "r+b");
			}
			else
			{
				m_fp = _msokfopen(m_filepathname.GetChars(), "r+b");
			}
		}
		if ( NULL == m_fp )
		{
			throw new IOException("Can't open file");
		}
		else
		{
			m_state = new FileStreamState_Open(this, &m_state);
		}
	}

	FileStream::FileStream(const char *path, File::FileMode mode, File::FileAccess access)
	: m_filepathname(path), m_mode(mode), m_access(access), 
	m_canClose(true), m_lasterr(0), m_state(NULL)
	{
		Init();
	}

	FileStream::FileStream(const String& path, File::FileMode mode, File::FileAccess access)
	: m_filepathname(path), m_mode(mode), m_access(access), 
	m_canClose(true), m_lasterr(0), m_state(NULL)
	{
		Init();
	}

	FileStream::~FileStream()
	{
		Close();
		delete m_state;
	}

	void FileStream::Close()
	{
		m_state->Close();
	}

	void FileStream::Flush()
	{
		m_state->Flush();
	}

	int FileStream::Read(Array<byte>& buffer, const int offset, int count)
	{
		return m_state->Read(buffer, offset, count);
	}

	int FileStream::ReadByte()
	{
		return m_state->ReadByte();
	}

	long FileStream::Seek(const long offset, const SeekOrigin origin)
	{
		return m_state->Seek(offset, origin);
	}

	void FileStream::Write(const Array<byte>& buffer, const int offset, const int count)
	{
		m_state->Write(buffer, offset, count);
	}

	void FileStream::WriteByte(byte value)
	{
		m_state->WriteByte(value);
	}

	bool FileStream::CanRead() const
	{
		return m_access != File::FILEACC_Write;
	}

	bool FileStream::CanSeek() const
	{
		return true;
	}

	bool FileStream::CanWrite() const
	{
		return m_access != File::FILEACC_Read;
	}

	long FileStream::Length() const
	{
		return m_state->Length();
	}

	long FileStream::Position() const
	{
		return m_state->Position();
	}

	#ifdef DEBUG
	void FileStream::ValidateMem() const
	{
		m_filepathname.ValidateMem();
		ASSERT_PTR( m_state );
		m_state->ValidateMem();
	}

	void FileStream::CheckMem() const
	{
		m_filepathname.CheckMem();
		DEBUG_NOTE_MEM( m_state );
		m_state->CheckMem();
	}
	#endif
}
