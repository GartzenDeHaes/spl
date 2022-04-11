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
#include <stdio.h>
#include <memory.h>
#include <spl/Int32.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

void StringBuffer::Init(int size)
{
	m_buf = new char[size];
	m_used = 0;
	m_len = size;
	m_buf[0] = '\0';
}

StringBuffer::StringBuffer()
{
	Init(16);
}

StringBuffer::StringBuffer(const char *str)
{
	Init((int)strlen(str) + 1);
	Append(str);
}

StringBuffer::StringBuffer(int size)
{
	Init(size);
}

StringBuffer::StringBuffer(const StringBuffer& sb)
{
	Init(sb.m_len + 1);
	Append(sb.m_buf);
}

StringBuffer::~StringBuffer()
{
	delete[] m_buf;
}

#ifdef BSTR
StringBuffer::StringBuffer(BSTR str)
{
	m_len = SysStringLen(str) +1;

	m_buf = new char[len];
	for (m_used = 0; m_used < m_len; m_used++)
	{
		buf[m_used] = (char)str[m_used];
	}
	m_buf[m_used] = '\0';
}
#endif

void StringBuffer::Trim()
{
	ASSERT_MEM(m_buf, m_len);
	int x;
	for (x = m_used-1; x >= 0; x--)
	{
		if (m_buf[x] != ' ')
		{
			break;
		} 
		m_buf[x] = '\0';
	}
	m_used = x + 1;
	if ( ' ' == m_buf[0] )
	{
		x = 1;
		while ( ' ' == m_buf[x] )
		{
			x++;
		}
		ASSERT(' ' != m_buf[x]);
		memcpy(&m_buf[0], &m_buf[x], m_used - x);
		m_used -= x;
	}
	m_buf[m_used] = '\0';
}

/*
 *  Append characters to the end of the buffer.  Extend
 *  the buffer if needed.
 *  Throws OutOfMemoryException
 */
void StringBuffer::Append(const char *str)
{
	ASSERT_MEM(m_buf, m_len);

	int alen = (int)strlen(str);

	while (alen + m_used + 1 > m_len)
	{
		Extend();
	}
	int x;
	for (x = 0; x < alen; x++)
	{
		m_buf[m_used + x] = str[x];
	}
	m_buf[m_used + x] = '\0';
	m_used += alen;

	ASSERT(m_buf[m_used] == '\0');
}

void StringBuffer::Fill(char ch, int len)
{
	while ( len-- > 0 )
	{
		Append(ch);
	}
}

/**
 *  Insert a string at the index.  If the index is
 *  after EOS, the string is extended as necessary.
 */
void StringBuffer::Insert(int index, const char *str)
{
	ASSERT(m_buf[m_used] == '\0');
	ASSERT_MEM(m_buf, m_len);

	int x;
	int alen = (int)strlen(str);

	ExtendTo(m_used + alen + 1);
	for ( x = (m_used-index)-1; x >= 0; x-- )
	{
		m_buf[index+alen+x] = m_buf[index+x];
	}
	for (x = 0; x < alen; x++)
	{
		m_buf[index+x] = str[x];
	}
	m_used = m_used + alen;
	ASSERT( m_used < m_len );
	m_buf[m_used] = '\0';
	ValidateMem();
}

void StringBuffer::SetCharAt(const int index, char c)
{
	ASSERT_MEM(m_buf, m_len);
	ASSERT(m_buf[m_used] == '\0');

	ExtendTo(index);
	m_buf[index] = c;

	if (index == m_used)
	{
		if (m_used == m_len)
		{
			Extend();
		}
		m_buf[index+1] = '\0';
		m_used++;
	}
	ASSERT(m_buf[m_used] == '\0');
	ASSERT_MEM(m_buf, m_len);
}

void StringBuffer::RemoveCharAt( int idx )
{
	ASSERT_MEM(m_buf, m_len);
	ASSERT(m_buf[m_used] == '\0');

	if ( idx >= m_used || idx < 0 )
	{
		return;
	}
	memmove( &m_buf[idx],  &m_buf[idx+1], m_used-idx );
	m_used--;
}

StringBuffer& StringBuffer::operator =(const char *str)
{
	ASSERT_MEM(m_buf, m_len);
	m_used = 0;
	Append(str);

	return *this;
}

StringBuffer& StringBuffer::operator =(const StringBuffer& sb )
{
	ASSERT_MEM(m_buf, m_len);
	m_used = 0;
	Append(sb.m_buf);

	return *this;
}

bool StringBuffer::IsNumeric() const
{
	bool sawdot = false;
	ASSERT(m_buf[m_used] == '\0');
	ASSERT_MEM(m_buf, m_len);

	for (int x = 0; x < m_used; x++)
	{
		if (! isdigit(m_buf[x]))
		{
			if ( !sawdot && '.' == m_buf[x] )
			{
				sawdot = true;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

int StringBuffer::ToInt() const
{
	return Int32::Parse(m_buf);
}

void StringBuffer::ExtendTo(int index)
{
	ASSERT_MEM(m_buf, m_len);

	if (index > m_len)
	{
		char *newbuf;

		if ((newbuf = new char[index + 1]) == NULL)
		{
			throw OutOfMemoryException();
		}
		memcpy(newbuf, m_buf, m_len);
		m_len = index+1;
		delete[] m_buf;
		m_buf = newbuf;
		ASSERT_MEM(m_buf, m_len);
	}
	//if (index > used)
	//{
	//	int x;
	//	for (x = used+1; x < index; x++)
	//	{
	//		buf[x] = ' ';
	//	}
	//	//buf[used] = '\0';
	//}
	ASSERT(m_buf[m_used] == '\0');
	ASSERT_MEM(m_buf, m_len);
}

void StringBuffer::Extend()
{
	ASSERT_MEM(m_buf, m_len);

	int newlen = m_len*2;
	char *newbuf;

	newbuf = new char[newlen];

	memcpy(newbuf, m_buf, m_len);
	m_len = newlen;
	delete[] m_buf;
	m_buf = newbuf;
	ASSERT_MEM(m_buf, m_len);
}

int StringBuffer::IndexOf( const char *cp, const int start ) const
{
	for ( int pos = start; pos < m_len; pos++ )
	{
		int end;
		int cppos = 1;

		if ( m_buf[pos] == cp[0] )
		{
			for( end = pos+1; end < m_len && cp[cppos] != '\0'; end++ )
			{
				if ( m_buf[end] != cp[cppos] )
				{
					break;
				}
				cppos++;
			}
			if ( cp[cppos] == '\0' )
			{
				return pos;
			}
		}
	}
	return -1;
}

int StringBuffer::IndexOfIgnoreCase( const char *cp, const int start ) const
{
	for ( int pos = start; pos < m_len; pos++ )
	{
		if ( tolower(m_buf[pos]) == tolower(cp[0]) )
		{
			int end;
			int cppos = 1;
			for( end = pos+1; end < m_len && cp[cppos] != '\0'; end++ )
			{
				if ( m_buf[end] != cp[cppos] && tolower(m_buf[end]) != tolower(cp[cppos]) )
				{
					break;
				}
				cppos++;
			}
			if ( cp[cppos] == '\0' )
			{
				return pos;
			}
		}
	}
	return -1;
}

void StringBuffer::Replace( const char from, const char to )
{
	for ( int x = 0; x < m_used; x++ )
	{
		if ( m_buf[x] == from )
		{
			m_buf[x] = to;
		}
	}
}

void StringBuffer::ToLower()
{
	for (int x = 0; x < m_used; x++)
	{
		m_buf[x] = tolower(m_buf[x]);
	}
}

void StringBuffer::ToUpper()
{
	for (int x = 0; x < m_used; x++)
	{
		m_buf[x] = toupper(m_buf[x]);
	}
}

StringPtr StringBuffer::ToString() const
{
	return StringPtr( new String(m_buf) );
}

RefCountPtr<Array<byte> > StringBuffer::ToByteArray() const
{
	return String(m_buf).ToByteArray();
}

#if defined(DEBUG)
void StringBuffer::CheckMem() const
{
	DEBUG_NOTE_MEM_ALLOCATION(m_buf);
}

void StringBuffer::ValidateMem() const
{
	ASSERT_MEM(m_buf, m_len);
}
#endif

