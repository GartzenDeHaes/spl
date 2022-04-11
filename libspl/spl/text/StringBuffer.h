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
#ifndef _string_buffer_h
#define _string_buffer_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Char.h>
#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/RefCountPtr.h>
#include <spl/WeakReference.h>

namespace spl
{
/** 
 * @defgroup text Text
 * @{
 */

class StringBuffer;

/** @brief StringBuffer pointer type.  @ref StringBuffer */
typedef RefCountPtr<StringBuffer> StringBufferPtr;	//< StringBuffer smart pointer.
typedef WeakReference<StringBuffer, StringBufferPtr> StringBufferRef;

/** 
 *	@brief Dynamically resizing string. 
 */
class StringBuffer : public IMemoryValidate
{
private:

	char *m_buf;
	int m_used;
	int m_len;

	void Init(int size);

public:

#ifdef BSTR
	StringBuffer(BSTR str);
#endif

	StringBuffer();
	StringBuffer(const char *str);
	StringBuffer(int size);
	StringBuffer(const StringBuffer& sb);
	virtual ~StringBuffer();

	void Trim();

	inline void SetLength(int l)
	{
		ASSERT (l >= 0);
		ASSERT_MEM(m_buf, m_len);
		if (l > m_len)
		{
			ExtendTo(l);
		}
		m_used = l;
		m_buf[m_used] = '\0';
	}

	inline void Clear() { SetLength(0); }

	/*
	 *  Append characters to the end of the buffer.  Extend
	 *  the buffer if needed.
	 *  Throws OutOfMemoryException
	 */
	void Append(const char *str);

	inline void Append(const String *str) 
	{ 
		Append(str->GetChars()); 
	}

	inline void Append(StringPtr strp)
	{
		Append(*strp);
	}

	inline void Append(const String& str)
	{
		Append(str.GetChars());
	}

	inline void Append(const char c)
	{
		ASSERT_MEM(m_buf, m_len);

		if (m_used + 1 >= m_len)
		{
			Extend();
		}
		m_buf[m_used] = c;
		m_buf[m_used + 1] = '\0';
		m_used++;

		ASSERT(m_buf[m_used] == '\0');
	}

	inline void Append(const char *str, int start, int len)
	{
		ASSERT_MEM(m_buf, m_len);
		for (int x = 0; x < len; x++)
		{
			Append(str[x+start]);
		}
	}

	inline void Append(const char *str, int len) 
	{ 
		Append(str, 0, len); 
	}
	
	inline void Append(byte b)
	{
		Append((char *)&b, 1);
	}

	inline void Append(Array<byte>& data, int len)
	{
		Append((const char *)data.Data(), len);
	}

	inline void Append(Array<char>& data, int len)
	{
		Append((const char *)data.Data(), len);
	}

	/** @brief Appends len chars ch
	 */
	void Fill(char ch, int len);

	inline int Length() const
	{
		ASSERT_MEM(m_buf, m_len);
		return m_used;
	}

	//inline operator char *()
	//{
	//	ASSERT_MEM(buf, len);
	//	return buf;
	//}

	inline char CharAt(int index) const
	{
		if (index > m_used)
		{
			return '\0';
		}
		return m_buf[index];
	}

	inline char& operator[](int index) const
	{
		ASSERT_MEM(m_buf, m_len);

		if (index > m_used)
		{
			return m_buf[0];
		}
		return m_buf[index];
	}

	inline StringBuffer& operator = (StringPtr str) { *this = str->GetChars(); return *this; }
	StringBuffer& operator = (const char *str);
	StringBuffer& operator =(const StringBuffer& sb );

	StringBuffer& operator = (const String& str)
	{
		SetLength(0);
		Append(str);
		return *this;
	}

	inline void Set(const char *str)
	{
		SetLength(0);
		Append(str);
	}

	inline void Set(const String& str)
	{
		SetLength(0);
		Append(str);
	}

	void SetCharAt( const int index, char c );
	void RemoveCharAt( const int idx );
	
	inline void InsertCharAt( const char ch, const int idx )
	{
		char buf[2];
		buf[0] = ch;
		buf[1] = '\0';
		Insert(idx, buf);
	}

	/**
	 *  Insert a string at the index.  If the index is
	 *  after EOS, the string is extended as necessary.
	 */
	void Insert(int index, const char *str);

	void Replace( const char from, const char to );

	inline char *GetChars() const { return m_buf; }

	bool IsNumeric() const;
	int ToInt() const;

	void ToLower();
	void ToUpper();

	int IndexOf( const char *cp, const int start ) const;
	inline int IndexOf( const char ch ) const { return IndexOf(ch, 0); }

	inline int IndexOf( const char ch, const int start ) const
	{
		return spl::IndexofchfromWithLen(m_buf, ch, start, m_used);
	}

	int IndexOfIgnoreCase( const char *cp, const int start ) const;

	inline bool Equals(const char *cp) const { return strcmp(cp, m_buf) == 0; }

	StringPtr ToString() const;

	RefCountPtr<Array<byte> > ToByteArray() const;

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif

private:

	void ExtendTo(int index);

	void Extend();
};

/** @} */
}
#endif


