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
#include <spl/io/DesStream.h>

using namespace spl;

DesStream::DesStream(const String& password, IStreamPtr stream)
: m_password(password), m_stream(stream), m_des(password)
{
}

DesStream::~DesStream()
{
}

void DesStream::Close()
{
	m_stream->Close();
}

void DesStream::Flush()
{
	m_stream->Flush();
}

int DesStream::Read(Array<byte>& output, const int offset, int count)
{
	Array<byte> b(8);
	int readCount = 0;
	int len = (count - offset);
	int outpos = offset;

	if ( (len % 8) != 0 )
	{
		throw new InvalidArgumentException("DES: read length must be a multiple of 8");
	}

	for ( int x = 0; x < len/8; x++ )
	{
		int srlen = m_stream->Read(b, 0, 8);
		if ( srlen != 8 )
		{
			throw new IOException("DES: short read, could be caused by non-blocking IO");
		}
		m_des.DecryptBinary(b);
		Array<byte>::Copy(b, 0, output, outpos, 8);
		readCount += srlen;
		outpos += srlen;
	}
	return readCount;
}

int DesStream::ReadByte()
{
	throw new Exception("DES I/O must be multiples of 8 bytes");
}

long DesStream::Seek(const long offset, const SeekOrigin origin)
{
	return m_stream->Seek(offset, origin);
}

void DesStream::Write(const Array<byte>& buffer, const int offset, const int count)
{
	int len = (count - offset);

	if ( (len % 8) != 0 )
	{
		throw new InvalidArgumentException("DES: Write length must be a multiple of 8");
	}
	
	Array<byte> output;
	m_des.EncryptBinary(buffer, output);
	ASSERT(output.Length() == buffer.Length());

	m_stream->Write(output);
}

void DesStream::WriteByte(byte value)
{
	throw new Exception("DES I/O must be multiples of 8 bytes");
}

bool DesStream::CanRead() const
{
	return m_stream->CanRead();
}

bool DesStream::CanSeek() const
{
	return m_stream->CanSeek();
}

bool DesStream::CanWrite() const
{
	return m_stream->CanWrite();
}

long DesStream::Length() const
{
	return m_stream->Length();
}

long DesStream::Position() const
{
	return m_stream->Position();
}

#ifdef DEBUG
void DesStream::ValidateMem() const
{
	m_password.ValidateMem();
	m_stream.ValidateMem();
	m_stream->ValidateMem();
}

void DesStream::CheckMem() const
{
	m_password.CheckMem();
	m_stream.CheckMem();
	m_stream->CheckMem();
}
#endif
