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
#include <spl/types.h>

#ifdef _WINDOWS
#include <stdio.h>
#endif

#include <spl/io/BlockingStream.h>
#include <spl/io/DualChannelStream.h>
#include <spl/Int64.h>
#include <spl/io/NullStream.h>
#include <spl/text/StringBuffer.h>
#include <spl/io/Stream.h>
#include <spl/io/StreamBuffer.h>
#include <spl/io/StreamDelegateDispatch.h>
#include <spl/io/TextReader.h>
#include <spl/io/TextWriter.h>
#include <spl/io/ThreadSafeStream.h>
#include <stdarg.h>
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

using namespace spl;

extern char *int32toa( int32 i, char *buf, const int buflen );

using namespace spl;

StreamDelegateDispatch::~StreamDelegateDispatch()
{
}

void StreamDelegateDispatch::Add(IStreamReadListener *l)
{
	m_onClose += Delegate<IStreamReadListener>::Create(l, &IStreamReadListener::IStreamRead_OnClose);
	m_onError += DelegateOneParameter<IStreamReadListener, const String&>::Create(l, &IStreamReadListener::IStreamRead_OnError);
	m_onRead += DelegateTwoParameter<IStreamReadListener, const Array<byte>&, int>::Create(l, &IStreamReadListener::IStreamRead_OnRead);
}

#ifdef DEBUG
void StreamDelegateDispatch::ValidateMem() const
{
	m_onClose.ValidateMem();
	m_onError.ValidateMem();
	m_onRead.ValidateMem();
}

void StreamDelegateDispatch::CheckMem() const
{
	m_onClose.CheckMem();
	m_onError.CheckMem();
	m_onRead.CheckMem();
}
#endif

IStreamReadListener::~IStreamReadListener()
{
}

spl::IStream::IStream()
{
}

spl::IStream::~IStream()
{
}

IStreamState::IStreamState( IStream* parent, IStreamState **parentStateVar )
: m_parent(parent), m_holder(parentStateVar)
{
}

IStreamState::~IStreamState()
{
}

void IStreamState::ChangeState(IStreamState *newState)
{
	this->OnLeave( newState );
	newState->OnEnter( this );
	*m_holder = newState;
	delete this;
}

NullStream::NullStream()
{
}

NullStream::~NullStream()
{
}

void NullStream::Close()
{
}

void NullStream::Flush()
{
}

int NullStream::Read(Array<byte>& buffer, const int offset, int count)
{
	buffer[offset] = 0;
	return 0;
}

int NullStream::ReadByte()
{
	return 0;
}

long NullStream::Seek(const long offset, const SeekOrigin origin)
{
	return 0;
}

void NullStream::Write(const Array<byte>& buffer, const int offset, const int count)
{
}

void NullStream::WriteByte(byte value)
{
}

bool NullStream::CanRead() const
{
	return true;
}

bool NullStream::CanSeek() const
{
	return true;
}

bool NullStream::CanWrite() const
{
	return true;
}

long NullStream::Length() const
{
	return 0;
}

long NullStream::Position() const
{
	return 0;
}

#ifdef DEBUG
void NullStream::ValidateMem() const
{
}

void NullStream::CheckMem() const
{
}
#endif

DualChannelStream::DualChannelStream(IStreamPtr input, IStreamPtr output, bool canClose)
: m_in(input), m_out(output)
{
}

DualChannelStream::~DualChannelStream()
{
}

void DualChannelStream::Close()
{
	m_in->Close();
	m_out->Close();
}

void DualChannelStream::Flush()
{
	m_in->Flush();
	m_out->Flush();
}

int DualChannelStream::Read(Array<byte>& buffer, const int offset, int count)
{
	return m_in->Read(buffer, offset, count);
}

int DualChannelStream::ReadByte()
{
	return m_in->ReadByte();
}

long DualChannelStream::Seek(const long offset, const SeekOrigin origin)
{
	return m_out->Seek(offset, origin);
}

void DualChannelStream::Write(const Array<byte>& buffer, const int offset, const int count)
{
	m_out->Write(buffer, offset, count);
}

void DualChannelStream::WriteByte(byte value)
{
	m_out->WriteByte(value);
}

bool DualChannelStream::CanRead() const
{
	return m_in->CanRead();
}

bool DualChannelStream::CanSeek() const
{
	return m_out->CanSeek();
}

bool DualChannelStream::CanWrite() const
{
	return m_out->CanWrite();
}

long DualChannelStream::Length() const
{
	return m_in->Length();
}

long DualChannelStream::Position() const
{
	return m_out->Position();
}

#ifdef DEBUG
void DualChannelStream::ValidateMem() const
{
	m_in.ValidateMem();
	m_out.ValidateMem();
}

void DualChannelStream::CheckMem() const
{
	m_in.CheckMem();
	m_out.CheckMem();
}
#endif

BlockingStream::BlockingStream(IStreamPtr strm)
: m_strm(strm), m_dataready(), m_datalock()
{
	ASSERT(m_strm->CanRead());
}

BlockingStream::~BlockingStream()
{
	m_strm->Close();
}

int BlockingStream::Read(Array<byte>& buffer, const int offset, int count)
{
	int readbytes = 0;

	while ( 0 == readbytes )
	{
		m_datalock.Lock();
		int pending = m_strm->Length();
		m_datalock.Unlock();
		if ( 0 == pending )
		{
			m_dataready.Wait();
		}
		m_datalock.Lock();
		readbytes = m_strm->Read(buffer, offset, count);
		m_datalock.Unlock();
	}
	return readbytes;
}

int BlockingStream::ReadByte()
{
	Array<byte> buf(1);
	int count = Read(buf, 0, 1);
	if ( 0 >= count )
	{
		return -1;
	}
	return (int)buf[0];
}

void BlockingStream::Write(const Array<byte>& buffer, const int offset, const int count)
{
	m_datalock.Lock();
	m_strm->Write(buffer, offset, count);
	m_datalock.Unlock();
	m_dataready.Notify();
}

void BlockingStream::WriteByte(byte value)
{
	Array<byte> buf(1);
	buf[0] = value;
	Write(buf, 0, 1);
}

void BlockingStream::Close()
{
	m_strm->Close();
}

void BlockingStream::Flush()
{
	m_strm->Flush();
}

long BlockingStream::Seek(const long offset, const SeekOrigin origin)
{
	return m_strm->Seek(offset, origin);
}

bool BlockingStream::CanRead() const
{
	return m_strm->CanRead();
}

bool BlockingStream::CanSeek() const
{
	return m_strm->CanSeek();
}

bool BlockingStream::CanWrite() const
{
	return m_strm->CanWrite();
}

long BlockingStream::Length() const
{
	return m_strm->Length();
}

long BlockingStream::Position() const
{
	return m_strm->Position();
}

#ifdef DEBUG
void BlockingStream::ValidateMem() const
{
	m_strm->ValidateMem();
}

void BlockingStream::CheckMem() const
{
	m_strm.CheckMem();
}
#endif

TextWriter::TextWriter(IStreamPtr strm)
: m_strm(strm)
{
	if ( ! m_strm->CanWrite() )
	{
		throw new InvalidArgumentException("Stream is not writable.");
	}
}

TextWriter::~TextWriter()
{
}

void TextWriter::Close()
{
	m_strm->Close();
}

void TextWriter::Flush()
{
	m_strm->Flush();
}

void TextWriter::Write(const String& buf)
{
	ASSERT ( sizeof(char) == sizeof(byte) );

	m_strm->Write( buf.ToByteArray() );
}

void TextWriter::Write(byte b)
{
	Write((unsigned int)b);
}

void TextWriter::Write(int8 i)
{
	char buf[128];
	int32toa(i, buf, 128);
	Write(buf);
}

void TextWriter::Write(int16 i)
{
	char buf[128];
	int32toa(i, buf, 128);
	Write(buf);
}

void TextWriter::Write(int32 i)
{
	char buf[128];
	int32toa(i, buf, 128);
	Write(buf);
}

void TextWriter::Write(int64 i)
{
	Write(*Int64::ToString(i));
}

void TextWriter::Write(uint16 i)
{
	char buf[128];
	int32toa(i, buf, 128);
	Write(buf);
}

void TextWriter::Write(uint32 i)
{
	char buf[128];
	int32toa(i, buf, 128);
	Write(buf);
}

void TextWriter::Write(float32 f)
{
	char buf[128];
	sprintf(buf, "%f", f);
	Write(buf);
}

void TextWriter::Write(float64 f)
{
	char buf[128];
	sprintf(buf, "%lf", f);
	Write(buf);
}

void TextWriter::WriteLine(const String& str)
{
	Write(str);
#ifdef _WINDOWS
	Write("\r\n");
#elif _MACOSX
	Write("\r");
#else
	Write("\n");
#endif
}

// FYI, va args doesn't work with reference.
void TextWriter::WriteFormat(const String fmt, ...)
{
	va_list marker;
	va_start(marker, fmt);

	StringPtr str = String::FormatVA(fmt, marker);

	Write(str->GetChars());
}

#ifdef DEBUG
void TextWriter::ValidateMem() const
{
	m_strm.ValidateMem();
}

void TextWriter::CheckMem() const
{
	m_strm.CheckMem();
}
#endif

TextReader::TextReader(IStreamPtr strm)
: m_strm(strm), m_peekch(-1)
{
}

TextReader::~TextReader()
{
}

void TextReader::Close()
{
	m_strm->Close();
}

int TextReader::Read()
{
	return _Read();
}

void TextReader::UnRead(int ch)
{
	if (m_peekch > -1)
	{
		throw Exception("UnRead already has pending input");
	}

	m_peekch = ch;
}

int TextReader::ReadBlock(Array<byte>& buf, int offset, int count)
{
	ASSERT( count > 0 && offset >= 0 );
	if ( m_peekch > -1 )
	{
		buf[offset++] = (char)m_peekch;
		count--;
	}

	ASSERT ( sizeof(char) == sizeof(byte) );
	return m_strm->Read(buf, offset, count);
}

bool TextReader::ReadLine(StringBuffer& sb)
{
	int ch;

	while ( '\n' != (ch = _Read()) )
	{
		if ( -1 == ch )
		{
			if ( sb.Length() == 0 )
			{
				return false;
			}
			return true;
		}
		if ( '\r' != ch )
		{
			sb.Append( (char)ch );
		}
	}
	return true;
}

StringPtr TextReader::ReadLine()
{
	StringBuffer buf;
	if ( ReadLine(buf) )
	{
		return buf.ToString();
	}
	return StringPtr();
}

bool TextReader::ReadLine(Array<byte>& buf, int& lineLen)
{
	int buflen = buf.Length();
	lineLen = 0;

	ASSERT( buflen > 0 );
	int ch = _Read();

	if ( -1 == ch )
	{
		return false;
	}

	while ( lineLen < (buflen-1) && ch != '\n' && ch != -1 )
	{
		if ( '\r' != ch )
		{
			buf[lineLen++] = ch;
		}
		ch = _Read();
	}
	buf[lineLen++] = '\0';

	return true;
}

StringPtr TextReader::ReadToEnd()
{
	StringBuffer buf;
	int ch;

	while ( (ch = _Read()) > -1 )
	{
		m_strm->ValidateMem();
		buf.Append( (char)ch );
	}
	return buf.ToString();
}

#ifdef DEBUG
void TextReader::ValidateMem() const
{
	m_strm.ValidateMem();
}

void TextReader::CheckMem() const
{
	m_strm.CheckMem();
}
#endif

ThreadSafeStream::ThreadSafeStream( IStreamPtr strm )
: m_strm(strm), m_lock()
{
}

ThreadSafeStream::~ThreadSafeStream()
{
	m_strm->Close();
}

void ThreadSafeStream::Close()
{
	m_strm->Close();
}

void ThreadSafeStream::Flush()
{
	m_lock.Lock();
	m_strm->Flush();
	m_lock.Unlock();
}

int ThreadSafeStream::Read(Array<byte>& buffer, const int offset, int count)
{
	m_lock.Lock();
	int ret = m_strm->Read(buffer, offset, count);
	m_lock.Unlock();
	return ret;
}

int ThreadSafeStream::ReadByte()
{
	m_lock.Lock();
	int ch = m_strm->ReadByte();
	m_lock.Unlock();
	return ch;
}

long ThreadSafeStream::Seek(const long offset, const SeekOrigin origin)
{
	m_lock.Lock();
	long ret = m_strm->Seek(offset, origin);
	m_lock.Unlock();
	return ret;
}

void ThreadSafeStream::Write(const Array<byte>& buffer, const int offset, const int count)
{
	m_lock.Lock();
	m_strm->Write(buffer, offset, count);
	m_lock.Unlock();
}

void ThreadSafeStream::WriteByte(byte value)
{
	m_lock.Lock();
	m_strm->WriteByte(value);
	m_lock.Unlock();
}

bool ThreadSafeStream::CanRead() const
{
	return m_strm->CanRead();
}

bool ThreadSafeStream::CanSeek() const
{
	return m_strm->CanSeek();
}

bool ThreadSafeStream::CanWrite() const
{
	return m_strm->CanWrite();
}

long ThreadSafeStream::Length() const
{
	long len = m_strm->Length();
	return len;
}

long ThreadSafeStream::Position() const
{
	long ret = m_strm->Position();
	return ret;
}

#ifdef DEBUG
void ThreadSafeStream::ValidateMem() const
{
	m_strm.ValidateMem();
}

void ThreadSafeStream::CheckMem() const
{
	m_strm.CheckMem();
}
#endif

StreamBuffer::StreamBuffer( IStreamPtr strm, const int buflen )
:	m_strm(strm), 
	m_bufpos(0), 
	m_buflen(buflen), 
	m_buf(buflen)
{
}

StreamBuffer::~StreamBuffer()
{
	Flush();
	m_strm->Close();
}

void StreamBuffer::Close()
{
	Flush();
	m_strm->Close();
}

void StreamBuffer::Flush()
{
	if ( m_bufpos > 0 && m_strm->CanWrite() )
	{
		m_strm->Write(m_buf, 0, m_bufpos);
	}
	m_bufpos = 0;
}

int StreamBuffer::Read(Array<byte>& buffer, const int offset, int count)
{
	if ( m_bufpos == 0 )
	{
		// load the buffer
		m_bufpos = m_strm->Read(m_buf, 0, m_buflen);
		if ( m_bufpos < 0 )
		{
			m_bufpos = 0;
			return -1;
		}
	}
	if ( count > m_bufpos )
	{
		count = m_bufpos;
	}
	ASSERT( m_bufpos >= count );
	memcpy(&buffer[offset], m_buf, count);
	memcpy(m_buf, &m_buf[count], m_bufpos - count);
	m_bufpos -= count;
	return count;
}

int StreamBuffer::ReadByte()
{
	Array<byte> buf(1);
	if ( 0 >= StreamBuffer::Read(buf, 0, 1) )
	{
		return -1;
	}
	return (int)buf[0];
}

long StreamBuffer::Seek(const long offset, const SeekOrigin origin)
{
	Flush();
	return m_strm->Seek(offset, origin);
}

void StreamBuffer::Write(const Array<byte>& buffer, const int offset, const int count)
{
	if ( m_bufpos + count >= m_buflen )
	{
		Flush();
	}
	if ( count >= m_buflen )
	{
		m_strm->Write(buffer, offset, count);
	}
	else
	{
		memcpy(&m_buf[m_bufpos], &buffer[offset], count);
		m_bufpos += count;
	}
	ASSERT( m_bufpos < m_buflen );
}

void StreamBuffer::WriteByte(byte value)
{
	if ( m_bufpos + 1 >= m_buflen )
	{
		Flush();
	}
	m_buf[m_bufpos++] = value;
}

bool StreamBuffer::CanRead() const
{
	return m_strm->CanRead();
}

bool StreamBuffer::CanSeek() const
{
	return m_strm->CanSeek();
}

bool StreamBuffer::CanWrite() const
{
	return m_strm->CanWrite();
}

long StreamBuffer::Length() const
{
	return m_strm->Length();
}

long StreamBuffer::Position() const
{
	if ( 0 != m_bufpos )
	{
		throw new IOException("StreamBuffer: Flush before calling Position");
	}
	return m_strm->Position();
}

#ifdef DEBUG
void StreamBuffer::ValidateMem() const
{
	m_buf.ValidateMem();
	m_strm.ValidateMem();
}

void StreamBuffer::CheckMem() const
{
	m_buf.CheckMem();
	m_strm.CheckMem();
}
#endif
