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
#ifdef _WIN32
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#include <spl/io/FileStream.h>

#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

namespace spl
{
	class FdStreamState_Closed : public IStreamState
	{
	public:
		FdStreamState_Closed(spl::IStream *parent, IStreamState **parentStateVar);
		virtual ~FdStreamState_Closed();

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

	FdStreamState_Closed::FdStreamState_Closed
	(
		spl::IStream *parent, 
		IStreamState **parentStateVar
	)
	: IStreamState(parent, parentStateVar)
	{
	}

	FdStreamState_Closed::~FdStreamState_Closed()
	{
	}

	void FdStreamState_Closed::OnLeave(IStreamState *newState)
	{
	}

	void FdStreamState_Closed::OnEnter(IStreamState *oldState)
	{
	}

	void FdStreamState_Closed::Close()
	{
		throw new IOException("File is closed, so it can't be closed again.");
	}

	void FdStreamState_Closed::Flush()
	{
		throw new IOException("File is closed, so it can't be flushed");
	}

	int FdStreamState_Closed::Read(Array<byte>& buffer, const int offset, int count)
	{
		throw new IOException("File is closed, so it can't be read");
	}

	int FdStreamState_Closed::ReadByte()
	{
		throw new IOException("File is closed, so it can't be read");
	}

	long FdStreamState_Closed::Seek(const long offset, const SeekOrigin origin)
	{
		throw new IOException("File is closed, so it can't be seeked");
	}

	void FdStreamState_Closed::Write(const Array<byte>& buffer, const int offset, const int count)
	{
		throw new IOException("File is closed, so it can't be written");
	}

	void FdStreamState_Closed::WriteByte(byte value)
	{
		throw new IOException("File is closed, so it can't be written");
	}

	bool FdStreamState_Closed::CanRead() const
	{
		throw new NotImplementedException();
	}

	bool FdStreamState_Closed::CanSeek() const
	{
		throw new NotImplementedException();
	}

	bool FdStreamState_Closed::CanWrite() const
	{
		throw new NotImplementedException();
	}

	long FdStreamState_Closed::Length() const
	{
		throw new IOException("File is closed");
	}

	long FdStreamState_Closed::Position() const
	{
		throw new IOException("File is closed");
	}

	#ifdef DEBUG
	void FdStreamState_Closed::ValidateMem() const
	{
	}

	void FdStreamState_Closed::CheckMem() const
	{
	}
	#endif

	class FdStreamState_Open : public IStreamState
	{
	public:
		FdStreamState_Open(IStream *parent, IStreamState **parentStateVar);
		virtual ~FdStreamState_Open();

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

	FdStreamState_Open::FdStreamState_Open
	(
		IStream* parent, 
		IStreamState **parentStateVar
	)
	: IStreamState(parent, parentStateVar)
	{
	}

	FdStreamState_Open::~FdStreamState_Open()
	{
	}

	void FdStreamState_Open::OnLeave(IStreamState *newState)
	{
		if ( ((FdStream *)m_parent)->m_canClose )
		{
			close( ((FdStream *)m_parent)->m_fd );
		}
	}

	void FdStreamState_Open::OnEnter(IStreamState *oldState)
	{
	}

	void FdStreamState_Open::Close()
	{
		IStreamState *newstate = new FdStreamState_Closed(m_parent, m_holder);
		ChangeState(newstate);
	}

	void FdStreamState_Open::Flush()
	{
		//flush( ((FdStream *)m_parent)->m_fd);
	}

	int FdStreamState_Open::Read(Array<byte>& buffer, const int offset, int count)
	{
		ASSERT(offset + count <= buffer.Length());

		int ret = (int)read(((FdStream *)m_parent)->m_fd, &(buffer.Data()[offset]), count);
		((FdStream *)m_parent)->m_lasterr = errno;
		return ret;
	}

	int FdStreamState_Open::ReadByte()
	{
		byte b;
		read(((FdStream *)m_parent)->m_fd, &b, 1);
		return b;
	}

	long FdStreamState_Open::Seek(const long offset, const SeekOrigin origin)
	{
		throw new NotImplementedException();
	}

	void FdStreamState_Open::Write(const Array<byte>& buffer, const int offset, const int count)
	{
		ASSERT(offset + count <= buffer.Length());

		write(((FdStream *)m_parent)->m_fd, &(buffer.Data()[offset]), count);
		((FdStream *)m_parent)->m_lasterr = errno;
	}

	void FdStreamState_Open::WriteByte(byte value)
	{
		write(((FdStream *)m_parent)->m_fd, &value, 1);
		((FdStream *)m_parent)->m_lasterr = errno;
	}

	bool FdStreamState_Open::CanRead() const
	{
		throw new NotImplementedException();
	}

	bool FdStreamState_Open::CanSeek() const
	{
		throw new NotImplementedException();
	}

	bool FdStreamState_Open::CanWrite() const
	{
		throw new NotImplementedException();
	}

	long FdStreamState_Open::Length() const
	{
		throw new NotImplementedException();
	}

	long FdStreamState_Open::Position() const
	{
		throw new NotImplementedException();
		//return tell(((FdStream *)m_parent)->m_fd);
	}

	#ifdef DEBUG
	void FdStreamState_Open::ValidateMem() const
	{
	}

	void FdStreamState_Open::CheckMem() const
	{
	}
	#endif

	FdStream::FdStream(int fp, File::FileAccess access, bool canClose)
	: m_filepathname(""), m_mode(File::FILEMODE_Open), m_access(access), m_canClose(canClose), m_lasterr(0), m_state(NULL)
	{
		m_fd = fp;
		m_state = new FdStreamState_Open(this, &m_state);
	}

	FdStream::FdStream(const String& path, File::FileMode mode, File::FileAccess access)
	: m_filepathname(path), m_mode(mode), m_access(access), 
	m_canClose(true), m_lasterr(0), m_state(NULL)
	{
		if ( File::Exists(path) )
		{
			if ( File::FILEMODE_CreateNew == mode )
			{
				throw new IOException("File exists");
			}
			if ( File::FILEMODE_Truncate == mode )
			{
				if ( 0 > (m_fd = open(path.GetChars(), O_WRONLY)) )
				{
					close(m_fd);
				}
			}
		}
		else if ( File::FILEMODE_Open == mode )
		{
			throw new IOException("File not found");
		}
		else
		{
			// create the file
			if ( 0 > (m_fd = open(path.GetChars(), O_APPEND)) )
			{
				close(m_fd);
			}
		}
		if ( File::FILEMODE_Append == mode )
		{
			if ( File::FILEACC_Read == access )
			{
				m_fd = open(path.GetChars(), O_APPEND);
			}
			else
			{
				m_fd = open(path.GetChars(), O_APPEND);
			}
		}
		else
		{
			if ( File::FILEACC_Read == access )
			{
				m_fd = open(path.GetChars(), O_RDONLY);
			}
			else if ( File::FILEACC_Write == access )
			{
				m_fd = open(path.GetChars(), O_RDONLY);
			}
			else
			{
				m_fd = open(path.GetChars(), O_RDONLY);
			}
		}
		if ( 0 > m_fd )
		{
			throw new IOException("Can't open file");
		}
		else
		{
			m_state = new FdStreamState_Open(this, &m_state);
		}
	}

	FdStream::~FdStream()
	{
		Close();
		delete m_state;
	}

	void FdStream::Close()
	{
		m_state->Close();
	}

	void FdStream::Flush()
	{
		m_state->Flush();
	}

	int FdStream::Read(Array<byte>& buffer, const int offset, int count)
	{
		return m_state->Read(buffer, offset, count);
	}

	int FdStream::ReadByte()
	{
		return m_state->ReadByte();
	}

	long FdStream::Seek(const long offset, const SeekOrigin origin)
	{
		return m_state->Seek(offset, origin);
	}

	void FdStream::Write(const Array<byte>& buffer, const int offset, const int count)
	{
		m_state->Write(buffer, offset, count);
	}

	void FdStream::WriteByte(byte value)
	{
		m_state->WriteByte(value);
	}

	bool FdStream::CanRead() const
	{
		return m_access != File::FILEACC_Write;
	}

	bool FdStream::CanSeek() const
	{
		return true;
	}

	bool FdStream::CanWrite() const
	{
		return m_access != File::FILEACC_Read;
	}

	long FdStream::Length() const
	{
		return m_state->Length();
	}

	long FdStream::Position() const
	{
		return m_state->Position();
	}

	#ifdef DEBUG
	void FdStream::ValidateMem() const
	{
		m_filepathname.ValidateMem();
		ASSERT_PTR( m_state );
		m_state->ValidateMem();
	}

	void FdStream::CheckMem() const
	{
		m_filepathname.CheckMem();
		DEBUG_NOTE_MEM( m_state );
		m_state->CheckMem();
	}
	#endif
}
