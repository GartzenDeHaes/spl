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
#ifndef _tstring_h
#define _tstring_h

#include <string.h>
#include <stdlib.h>

#ifndef __cplusplus
#error not cpp
#endif

#include <spl/types.h>
#include <spl/Debug.h>

#ifndef _WINDOWS
char *ltoa( long i, char *buf, const int buflen );
#endif

#include <stdarg.h>

#include <spl/collection/Array.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/collection/Vector.h>
#include <spl/WeakReference.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define STRING_MAJIC 0x0001		//< Majic number for ASSERT's in Compare and Convert

#ifdef StrCmp
#undef StrCmp
#endif

extern int StrCpyLen( char *strto, const char *strfrom, const int maxlen );
extern int StrCpy( char *strto, const char *strfrom );
extern int StrCmp( const char *str1, const char *str2, int maxlen );
extern int StrCmpNoCase( const char *str1, int str1len, const char *str2 );
extern int IndexofchfromWithLen( const char *str, const char ch, int start, const int len );
#define IndexOfCh(s,c) spl::IndexofchfromWithLen(s,c,0,(int)strlen(s))

class String;
typedef RefCountPtr<String> StringPtr;
typedef WeakReference<String, StringPtr> StringRef;

/** @brief Your friend, the string.
 *
 */
class String : public IComparable, public IMemoryValidate
{
private:
	static char m_empty[2];

protected:
	const char *m_cstr;
	bool m_isintern;
	int m_len;

	void InitWith(const char *cp, const int offset, const int len);

public:
	String();
	
	inline String( const char *cp, const int offset, const int len )
	{
		InitWith(cp, offset, len);
	}

	inline String( const Array<byte>& cp, const int offset, const int len )
	{
		InitWith((const char *)cp.Data(), offset, len);
	}

	inline String( const Array<char>& cp, const int offset, const int len )
	{
		InitWith((const char *)cp.Data(), offset, len);
	}

	inline String( const char *cp, const int len )
	{
		InitWith(cp, 0, len);
	}

	inline String( const char *cp )
	{
		InitWith(cp, 0, (int)strlen(cp));
	}

	String( const String& str );

	~String();

	void Set( const String& cp );

	inline String& operator =(const String& str)
	{
		if ( Equals(str) )
		{
			return *this;
		}
		Set(str);
		return *this;
	}

	inline String& operator =(const char *str)
	{
		if ( Equals(str) )
		{
			return *this;
		}
		Set(str);
		return *this;
	}

	inline bool operator ==(const String& str) const
	{
		return Equals(str);
	}

	inline bool operator ==(const char *cp) const
	{
		return Equals(cp);
	}

	inline bool operator !=(const String& str) const
	{
		return !Equals(str);
	}

	inline bool operator <(const String& str) const
	{
		return Compare(str) < 0;
	}

	inline bool operator >(const String& str) const
	{
		return Compare(str) > 0;
	}

	inline bool operator <=(const String& str) const
	{
		return Compare(str) <= 0;
	}

	inline bool operator >=(const String& str) const
	{
		return Compare(str) >= 0;
	}

	StringPtr Substring( int start, int len ) const;
	inline StringPtr Substring( int start ) const { return Substring(start, m_len - start); }

	StringPtr Cat( const String& cp, const int len ) const;
	inline StringPtr Cat( const String& arg ) const { return Cat(arg.GetChars(), arg.Length()); }

	inline String operator +( const String& arg ) const { return *Cat(arg); }

	StringPtr Right( int len ) const;
	StringPtr Left( int len ) const;
	StringPtr Mid( int start, int stop ) const;

	RefCountPtr<Vector<StringPtr> > Split( const String& cp ) const;
	
	StringPtr PadRight( char ch, int count );

	virtual int Compare( const IComparable *str ) const;
	virtual bool Equals( const IComparable *str ) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	inline int Compare( const String& str ) const
	{
		return Compare( str.m_cstr );
	}

	inline int Compare( const char *cp ) const
	{
		return spl::StrCmp( m_cstr, cp, (int)strlen(cp) );
	}

	inline int CompareNoCase( const char *cp ) const
	{
		return spl::StrCmpNoCase( m_cstr, m_len, cp );
	}

	inline int CompareNoCase( const String& sp ) const
	{
		return spl::StrCmpNoCase( m_cstr, m_len, sp.m_cstr );
	}

	inline int IndexOf( const String& str ) const
	{
		return IndexOf( str, 0 );
	}

	int IndexOf( const String& cp, const int start ) const;

	inline int IndexOf( const char ch ) const
	{
		return IndexOf( ch, 0 );
	}
	
	inline int IndexOf( const char ch, const int start ) const
	{
		return spl::IndexofchfromWithLen(m_cstr, ch, start, m_len);
	}

	int IndexOfAny( const String& str ) const;

	int LastIndexOf( const String& str, const int pos ) const;
	
	inline int LastIndexOf( const String &str ) const
	{
		return LastIndexOf(str, Length()-1);
	}

	inline bool StartsWith( char ch ) { char buf[2]; buf[0] = ch; buf[1] = '\0'; return StartsWith(buf); }
	bool StartsWith( const String& str ) const;

	bool EndsWith( const String& str ) const;

	inline bool EndsWith( char cp ) const
	{
		char buf[2];
		buf[0] = cp;
		buf[1] = '\0';
		return EndsWith( buf );
	}

	bool HasNonPrintableCharacters() const;
	StringPtr RemoveNonPrintableCharacters() const;

	inline bool EqualsIgnoreCase( const String& cp ) const
	{
		return 0 == CompareNoCase(cp);
	}

	inline bool Equals( const String& cp ) const
	{
		return 0 == spl::StrCmp(m_cstr, cp.m_cstr, m_len);
	}

	inline bool Equals( const char ch ) const
	{
		return 1 == m_len && ch == m_cstr[0];
	}

	inline int CountChar( const char ch ) const
	{
		return CountChar( m_cstr, m_len, ch );
	}

	virtual uint32 HashCode() const;
	virtual uint64 HashCodeLong() const;

	inline const char *GetChars() const
	{
		return m_cstr;
	}

	inline int Length() const
	{
		ASSERT( (int)strlen(m_cstr) == m_len );
		return m_len;
	}

	char CharAt( const int idx ) const;

	char operator[] (const int idx) const;
	
	StringPtr Replace(const char from, const char to) const;
	StringPtr Replace(const String& from, const String& to) const;

	StringPtr ToUpper() const;

	StringPtr ToLower() const;

	StringPtr StripQuotes();

	/// @brief Trim spaces from the right side of the string.
	StringPtr RTrim(char ch) const;
	inline StringPtr RTrim() { return RTrim(' '); }

	StringPtr LTrim(char ch) const;
	inline StringPtr LTrim() { return LTrim(' '); }

	/// @brief Trim spaces from both sides of the string
	StringPtr Trim() const;

	inline operator const char *()
	{
		return GetChars();
	}

	inline StringPtr Clone() const
	{
		return StringPtr(new String(*this));
	}

	RefCountPtr<Array<byte> > ToByteArray() const;
	
	inline StringPtr ToString() const
	{
		return StringPtr(new String(*this));
	}

	static StringPtr Fill( char ch, int length );

	static int CountChar( const char *str, const int len, const char ch );

	static StringPtr Base64Encode( RefCountPtr<Array<byte> > cp );
	static StringPtr Base64Encode( const char* cp, int len );
	static RefCountPtr<Array<byte> > Base64Decode( const String& cp );

	///@brief wrap a c-string, do not delete when the String is destructed.
	static StringPtr Intern( const char *str );

	///@brief copies the contents of the string and marks it as deletable.
	void UnIntern();

	static StringPtr Format(const String fmt, ...);
	static StringPtr FormatVA(const String fmt, va_list args);

	friend String operator +(const char *cp, const String& str);

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

String operator +(const char *cp, const String& str);

inline bool operator ==(const char *cp, const String& str)
{
	return str == cp;
}

/** @} */
}
#endif
