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
#include <spl/io/MemoryStream.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

MemoryStream::MemoryStream()
: m_buf(), m_ptr(0)
{
}

MemoryStream::~MemoryStream()
{
}

void MemoryStream::Close()
{
	m_buf.Clear();
	m_ptr = 0;
}

void MemoryStream::Flush()
{
	m_buf.Clear();
	m_ptr = 0;
}

int MemoryStream::Read(Array<byte>& buffer, const int offset, int count)
{
	if ( m_buf.Count() == 0 )
	{
		return 0;
	}
	if ( m_ptr+count > m_buf.Count() )
	{
		count = m_buf.Count() - m_ptr;
	}
	int bufPos = offset;
	int pos = count;
	while ( pos-- > 0 )
	{
		buffer[bufPos++] = m_buf.ElementAt(m_ptr);
		m_buf.RemoveAt(m_ptr);
	}
	return count;
}

int MemoryStream::ReadByte()
{
	if ( m_ptr < m_buf.Count() )
	{
		m_buf.ValidateMem();
		return m_buf.RemoveAt(m_ptr);
	}
	return -1;
}

long MemoryStream::Seek(const long offset, const SeekOrigin origin)
{
	if ( IStream::SEEK_Begin == origin )
	{
		m_ptr = (offset > m_buf.Count()) ? m_buf.Count() : (int)offset;
	}
	else if ( IStream::SEEK_End == origin )
	{
		m_ptr = (offset > m_buf.Count()) ? 0 : m_buf.Count() - (int)offset;
	}
	else
	{
		m_ptr += offset;
		if ( m_ptr < 0 )
		{
			m_ptr = 0;
		}
		else if ( m_ptr > m_buf.Count() )
		{
			m_ptr = m_buf.Count();
		}
	}
	return m_ptr;
}

void MemoryStream::Write(const Array<byte>& buffer, const int offset, const int count)
{
	for ( int x = 0; x < count; x++)
	{
		m_buf.Add( buffer[x + offset] );
	}
}

void MemoryStream::WriteByte(byte value)
{
	m_buf.Add( value );
}

bool MemoryStream::CanRead() const
{
	return true;
}

bool MemoryStream::CanSeek() const
{
	return true;
}

bool MemoryStream::CanWrite() const
{
	return true;
}

long MemoryStream::Length() const
{
	return m_buf.Count();
}

long MemoryStream::Position() const
{
	return m_ptr;
}

StringPtr MemoryStream::ToString()
{
	StringBuffer buf;
	while ( Length() > 0 )
	{
		buf.Append((char)ReadByte());
	}
	return buf.ToString();
}

#ifdef DEBUG
void MemoryStream::ValidateMem() const
{
	m_buf.ValidateMem();
}

void MemoryStream::CheckMem() const
{
	m_buf.CheckMem();
}
#endif
