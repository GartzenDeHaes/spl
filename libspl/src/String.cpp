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
#include <ctype.h>
#include <stdio.h>
#include <spl/Char.h>
#include <spl/Double.h>
#include <spl/Exception.h>
#include <spl/Int32.h>
#include <spl/math/Math.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

const char eos = '\0';

char String::m_empty[2] = {'\0', 0xC};

namespace spl
{
	int IndexofchfromWithLen( const char *str, const char ch, int start, const int len )
	{
		int pos;

		for ( pos = start; pos < len; pos++ )
		{
			if ( str[pos] == ch )
			{
				return pos;
			}
		}
		return -1;
	}

	char *StrCat(const char *cstr, const int cstrlen, const char *arg, const int arglen)
	{
		int len = cstrlen + arglen + 1;
		char *buf = (char *)malloc(len);
		if ( NULL == buf )
		{
			throw OutOfMemoryException();
		}
		StrCpyLen(buf, cstr, len);
		strcat(buf, arg);
		return buf;
	}

	int StrCmpNoCase( const char *str1, int str1len, const char *str2 )
	{
		while ( *str1 != '\0' && *str2  != '\0' && --str1len >= 0 )
		{
			if ( toupper(*str1) != toupper(*str2) )
			{
				if ( *str1 > *str2 )
				{
					return 1;
				}
				return -1;
			}
			str1++;
			str2++;
		}
		if ( *str1 != '\0' )
		{
			return 1;
		}
		if ( *str2 != '\0' )
		{
			return -1;
		}
		return 0;
	}

	int StrCmp( const char *str1, const char *str2, int maxlen )
	{
		while ( *str1 != '\0' && *str2  != '\0' && --maxlen >= 0 )
		{
			if ( *str1 != *str2 )
			{
				if ( *str1 > *str2 )
				{
					return 1;
				}
				return -1;
			}
			str1++;
			str2++;
		}
		if ( *str1 != '\0' )
		{
			return 1;
		}
		if ( *str2 != '\0' )
		{
			return -1;
		}
		return 0;
	}

	int StrCpyLen( char *str1, const char *str2, const int maxlen )
	{
		int len = maxlen;
		while ( *str2 != '\0' && len-- > 0 )
		{
			*(str1++) = *(str2++);
		}
		*str1 = '\0';
		return -(len - maxlen);
	}

	int StrCpy( char *str1, const char *str2 )
	{
		return StrCpyLen(str1, str2, Int32::MaxValue());
	}
};

String::String()
:	m_cstr(m_empty),
	m_isintern(true),
	m_len(0)

{
}

void String::InitWith( const char *str, const int offset, const int len )
{
	m_len = len;
	char *cp;
	if ( NULL == (cp = (char *)malloc(m_len+1)) )
	{
		throw OutOfMemoryException();
	}
	m_isintern = false;
	StrCpyLen(cp, &str[offset], len);
	m_cstr = cp;
}

String::String( const String& str )
{
	m_len = str.Length();
	if ( str.m_isintern )
	{
		m_cstr = str.m_cstr;
	}
	else
	{
		char *cp;
		if ( NULL == (cp = (char *)malloc(m_len+1)) )
		{
			throw OutOfMemoryException();
		}
		StrCpyLen(cp, str.m_cstr, m_len);
		m_cstr = cp;
	}
	m_isintern = str.m_isintern;
}

String::~String()
{
	if ( !m_isintern )
	{
		free((char *)m_cstr);
	}
}

void String::Set( const String &str )
{
	ValidateMem();
	if ( m_isintern )
	{
		m_isintern = false;
	}
	else
	{
		free( (char *)m_cstr );
	}
	
	m_len = str.Length();
	char *cp;
	cp = (char *)malloc( m_len + 1 );
	if ( NULL == cp )
	{
		throw OutOfMemoryException();
	}
	StrCpyLen(cp, str.m_cstr, m_len);
	m_cstr = cp;
}

StringPtr String::PadRight( char ch, int count )
{
	char *buf = (char *)malloc(count + 1);
	if ( NULL == buf )
	{
		throw OutOfMemoryException();
	}
	for ( int x = 0; x < count; x++ )
	{
		buf[x] = ch;
	}
	buf[count] = '\0';
	StringPtr ret = Cat(buf);
	free( buf );
	return ret;
}

StringPtr String::Fill( char ch, int length )
{
	StringBuffer buf;
	buf.Fill(ch, length);
	return buf.ToString();
}

char String::CharAt( const int idx ) const
{
	if ( idx > m_len )
	{
		throw new IndexOutOfBoundsException();
	}
	return m_cstr[idx];
}

char String::operator[] (const int idx) const
{
	if ( idx > m_len )
	{
		throw new IndexOutOfBoundsException();
	}
	return m_cstr[idx];
}

StringPtr String::Substring( int start, int len ) const
{
	char *ret;
	const char *cpstart;

	if ( len == 0 )
	{
		return StringPtr(new String());
	}

	ASSERT ( len > 0 );

	if( start > m_len )
	{
		start = m_len;
	}
	if ( start + len > m_len )
	{
		len = m_len - start;
	}
	if ( len < 0 )
	{
		len = 0;
	}
	cpstart = &m_cstr[start];
	ASSERT_PTR( cpstart );

	if ( NULL == (ret = (char *)malloc( len+1 )) )
	{
		throw OutOfMemoryException();
	}
	StrCpyLen( ret, cpstart, len );
	ret[len] = eos;
	ASSERT_MEM( ret, len+1 );

	StringPtr strret = StringPtr(new String(ret));
	free(ret);
	return strret;
}

RefCountPtr<Vector<RefCountPtr<String> > > String::Split( const String& cp ) const
{
	RefCountPtr<Vector<RefCountPtr<String> > > tvec(new Vector<StringPtr>);
	int delimlen = cp.Length();
	int delimpos = IndexOf( cp );
	int pos = 0;

	StringPtr span;

	if ( delimpos < 0 )
	{
		if (m_len > 0)
		{
			tvec->Add( StringPtr(new String(m_cstr)));
		}
		return tvec;
	}
	while ( pos < m_len )
	{
		span = Mid( pos, delimpos );
		pos = delimpos + delimlen;
		tvec->Add( span );

		delimpos = IndexOf( cp, delimpos+1 );
		if ( delimpos < 0 )
		{
			break;
		}
	}
	pos = m_len - pos;
	ASSERT( pos >= 0 );
	if ( pos > 0 )
	{
		span = Right( pos );
		tvec->Add( span );
	}

	return tvec;
}

int String::IndexOfAny( const String& cp ) const
{
	int ret;
	int cplen = cp.Length();
	for ( int x = 0; x < m_len; x++ )
	{
		for ( int y = 0; y < cplen; y++ )
		{
			if ( 0 <= (ret = IndexofchfromWithLen(m_cstr, cp[y], 0, m_len)) )
			{
				return ret;
			}
		}
	}
	return -1;
}

StringPtr String::Replace(const char from, const char to) const
{
	StringBuffer buf(m_cstr);
	buf.Replace(from, to);
	return buf.ToString();
}

StringPtr String::Replace(const String& from, const String& to) const
{
	int start = 0;
	int pos = IndexOf( from );
	if ( 0 > pos )
	{
		return Clone();
	}

	int fromlen = from.Length();
	StringBuffer buf(m_len + 10);

	while (start < m_len)
	{
		buf.Append(m_cstr, start, pos-start);
		start = pos;
		buf.Append(to);
		start += fromlen;

		if ( 0 > (pos = IndexOf( from, start )) )
		{
			buf.Append(m_cstr, start, m_len - start);
			start = m_len;
		}
	}
	return buf.ToString();
}

StringPtr String::Cat( const String& cp, const int len ) const
{
	char *buf = StrCat(m_cstr, m_len, cp.m_cstr, len);
	StringPtr ret = StringPtr(new String(buf));
	free(buf);
	return ret;
}

StringPtr String::Right( int len ) const
{
	if ( len > m_len )
	{
		len = m_len;
	}
	return Substring( m_len - len );
}

StringPtr String::Left( int len ) const
{
	if ( len > m_len )
	{
		len = m_len;
	}
	return Substring(0, len);
}

StringPtr String::Mid( int start, int stop ) const
{
	return Substring( start, stop - start );
}

StringPtr String::RTrim(char ch) const
{
	StringBuffer buf(m_len+1);

	int endpos;
	for ( endpos = m_len-1; m_cstr[endpos] == ch && endpos >= 0; endpos-- )
	{
	}
	for ( int x = 0; x <= endpos; x++ )
	{
		ASSERT(m_cstr[x] != '\0');
		buf.Append(m_cstr[x]);
	}
	return buf.ToString();
}

StringPtr String::LTrim(char ch) const
{
	if (m_cstr[0] != ch)
	{
		return StringPtr(new String(*this));
	}
	
	int pos;
	StringBuffer buf(m_len);
	for ( pos = 0; pos < m_len; pos++ )
	{
		if (m_cstr[pos] != ch)
		{
			break;
		}
	}
	for ( int x = pos; x < m_len; x++ )
	{
		buf.Append(m_cstr[x]);
	}

	return buf.ToString();
}

StringPtr String::Trim() const
{
	StringBuffer buf(m_len);

	int pos;
	for ( pos = 0; m_cstr[pos] == ' ' && pos < m_len; pos++ )
	{
	}
	int endpos;
	for ( endpos = m_len-1; m_cstr[endpos] == ' ' && endpos >= 0; endpos-- )
	{
	}
	for ( int x = pos; x <= endpos; x++ )
	{
		buf.Append(m_cstr[x]);
	}
	return buf.ToString();
}

void String::UnIntern()
{
	char *buf = (char *)malloc(m_len+1);
	StrCpyLen(buf, m_cstr, m_len);	
	m_isintern = false;
	m_cstr = buf;
}

//void String::InsertCharAt(char ch, int idx)
//{
//	ASSERT_MEM(m_cstr, m_len);
//	
//	if ( m_isintern )
//	{
//		UnIntern();
//	}
//
//	if ( idx < 0 )
//	{
//		throw new InvalidArgumentException("Index cannot be less than zero.");
//	}
//	int tmplen;
//	if ( idx > m_len )
//	{
//		tmplen = idx + 1;
//	}
//	else
//	{
//		tmplen = m_len + 1;
//	}
//	char *tmp = (char *)malloc(tmplen+1);
//	if ( NULL == tmp )
//	{
//		throw OutOfMemoryException();
//	}
//	if( idx < m_len )
//	{
//		memcpy(tmp, m_cstr, idx);
//		ASSERT_MEM(m_cstr, m_len);
//		ASSERT_MEM(tmp, tmplen);
//		memcpy(&tmp[idx+1], &m_cstr[idx], m_len - (idx) );
//	}
//	else
//	{
//		memcpy(tmp, m_cstr, m_len);
//	}
//	tmp[idx] = ch;
//	tmp[tmplen] = '\0';
//	
//	free(m_cstr);
//	m_cstr = tmp;
//	m_len = tmplen;
//}

int String::IndexOf( const String& cp, const int start ) const
{
	for ( int pos = start; pos < m_len; pos++ )
	{
		int end;
		int cppos = 1;

		if ( m_cstr[pos] == cp[0] )
		{
			for( end = pos+1; end < m_len && cp[cppos] != '\0'; end++ )
			{
				if ( m_cstr[end] != cp[cppos] )
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

StringPtr String::Intern( const char *str )
{
	String *s = new String();
	ASSERT(s->m_isintern);

	s->m_cstr = str;
	s->m_isintern = true;
	s->m_len = (int)strlen(str);
	return StringPtr(s);
}

uint32 String::HashCode() const
{
	return Math::Hash(m_cstr);
}

uint64 String::HashCodeLong() const
{
	return Math::HashLong(m_cstr);
}

int String::Compare( const IComparable *istr ) const
{
	if (istr->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const String *str = static_cast<const String *>(istr);
	str->ValidateMem();
	return Compare( str );
}

bool String::Equals( const IComparable *istr ) const
{
	if (istr->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const String *str = static_cast<const String *>(istr);
	str->ValidateMem();
	return Equals( str );
}

int String::Compare( const IComparable& istr ) const
{
	if (istr.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const String& str = static_cast<const String&>(istr);
	str.ValidateMem();
	return Compare( str );
}

bool String::Equals( const IComparable& istr ) const
{
	if (istr.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const String& str = static_cast<const String&>(istr);
	str.ValidateMem();
	return Equals( str );
}

int32 String::MajicNumber() const
{
	return STRING_MAJIC;
}

static const char *base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

#define is_base64(c) ((isalnum(c) || (c == '+') || (c == '/')))

RefCountPtr<String> String::Base64Encode( const char *cp, int len )
{
	return Base64Encode(RefCountPtr<Array<byte> >(new Array<byte>((const byte *)cp, len)));
}

RefCountPtr<String> String::Base64Encode( RefCountPtr<Array<byte> > rcp )
{
	StringBuffer ret(256);
	int i = 0;
	int j = 0;
	int in_len = rcp->Length();
	byte *cp = rcp->Data();
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) 
	{
		char_array_3[i++] = *(cp++);
		if (i == 3) 
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
			{
				ret.Append(base64_chars[char_array_4[i]]);
			}
			i = 0;
		}
	}
	if (i)
	{
		for(j = i; j < 3; j++)
		{
			char_array_3[j] = '\0';
		}
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
		{
			ret.Append(base64_chars[char_array_4[j]]);
		}
		while((i++ < 3))
		{
			ret.Append('=');
		}
	}
	return ret.ToString();
}

RefCountPtr<Array<byte> > String::Base64Decode( const String& s )
{
	int in_len = s.Length();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	Vector<byte> ret(256);

	while (in_len-- && ( s[in_] != '=') && is_base64(s[in_])) 
	{
		char_array_4[i++] = s[in_]; 
		in_++;
		if (i ==4) 
		{
			for (i = 0; i <4; i++)
			{
				char_array_4[i] = IndexOfCh(base64_chars, char_array_4[i]);
			}
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
			{
				ret.Add(char_array_3[i]);
			}
			i = 0;
		}
	}

	if (i) 
	{
		for (j = i; j <4; j++)
		{
			char_array_4[j] = 0;
		}
		for (j = 0; j <4; j++)
		{
			char_array_4[j] = IndexOfCh(base64_chars, char_array_4[j]);
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) 
		{
			ret.Add(char_array_3[j]);
		}
	}
	return ret.ToArray();
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static void _prints(StringBuffer *out, const char *string, int width, int pad)
{
	int padchar = ' ';

	if (width > 0) 
	{
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) 
			++len;
		if (len >= width) 
			width = 0;
		else 
			width -= len;
		if (pad & PAD_ZERO) 
			padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) 
	{
		for ( ; width > 0; --width) 
		{
			out->Append((char) padchar);
		}
	}
	for ( ; *string ; ++string) 
	{
		out->Append((char) *string);
	}
	for ( ; width > 0; --width) 
	{
		out->Append((char) padchar);
	}
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static void _printi(StringBuffer *out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0;
	register unsigned int u = i;

	if (i == 0) 
	{
		print_buf[0] = '0';
		print_buf[1] = '\0';
		_prints (out, print_buf, width, pad);
		return;
	}

	if (sg && b == 10 && i < 0) 
	{
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) 
	{
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) 
	{
		if( width && (pad & PAD_ZERO) ) 
		{
			out->Append((char) '-');
			--width;
		}
		else 
		{
			*--s = '-';
		}
	}

	_prints (out, s, width, pad);
}

static int _round_(char *bytes, int nDigits, int roundPos, bool roundDown) 
{
	int next = roundPos + 1;
	if (next >= nDigits || bytes[next] < '5' ||
		// MRI rounds up on nnn5nnn, but not nnn5 --
		// except for when they do
		(roundDown && bytes[next] == '5' && next == nDigits - 1)) 
	{
		return nDigits;
	}
	if (roundPos < 0) 
	{ // "%.0f" % 0.99
		memcpy(bytes, &bytes[1], sizeof(char) * nDigits);
		bytes[0] = '1';
		return nDigits + 1;
	}
	bytes[roundPos] += 1;
	while (bytes[roundPos] > '9') 
	{
		bytes[roundPos] = '0';
		roundPos--;
		if (roundPos >= 0) 
		{
			bytes[roundPos] += 1;
		} 
		else 
		{
			memcpy(bytes, &bytes[1], sizeof(char) * nDigits);
			bytes[0] = '1';
			return nDigits + 1;
		}
	}
	return nDigits;
}


static void _printdf(StringBuffer *out, char fchar, double d, int width, int pad)
{
//
//http://www.java2s.com/Open-Source/Java-Document/Open-Source-Script/jruby/org/jruby/util/Sprintf.java.htm
//
	int precision = 0;
	double dval = d;
	bool nan = Math::IsNaN(dval);
	bool inf = Math::IsINF(dval);

	bool negative = dval < 0.0;
	int nDigits = 0;
	int exponent = 0;
	bool expForm = false;

	int len = 0;
	char signChar;

    if (nan || inf) 
	{
		const char *cdigits = NULL;
        if (nan) 
		{
            cdigits = "NaN";
            len = 3;
        } 
		else 
		{
            cdigits = "Inf";
            len = 3;
       }
        if (negative) 
		{
            signChar = '-';
            width--;
        } 
		//else if (pad & PAD_PLUS) 
		//{
        //    signChar = '+';
        //    width--;
        //} 
		else 
		{
            signChar = 0;
        }
        width -= len;

		if (width > 0 && (pad & PAD_RIGHT) == 0) 
		{
		    out->Fill(' ', width);
		    width = 0;
		}
		if (signChar != 0)
		{
			out->Append(signChar);
		}

        if (width > 0 && (pad & PAD_ZERO) == 0) 
		{
            out->Fill('0', width);
			width = 0;
        }
        
		out->Append(cdigits);

        if (width > 0)
		{
            out->Fill(' ', width);
		}

        return;
    }

	StringPtr str = Double::ToString(dval);
	int strlen = str->Length();
	char *digits = new char[strlen];
	int nTrailingZeroes = 0;
	int i = negative ? 1 : 0;
	int decPos = 0;
	char ival;
	bool decfound = false;
	while( i < strlen && !decfound ) 
	{
		switch (ival = (byte) str->CharAt(i++)) 
		{
		case '0':
			if (nDigits > 0)
				nTrailingZeroes++;

			break; // switch
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (nTrailingZeroes > 0) 
			{
				for (; nTrailingZeroes > 0; nTrailingZeroes--) 
				{
					digits[nDigits++] = '0';
				}
			}
			digits[nDigits++] = ival;
			break; // switch
		case '.':
			decfound = true;
			break;
		}
	}
	decPos = nDigits + nTrailingZeroes;
	decfound = false;
	while(i < strlen && !decfound) 
	{
		switch (ival = (byte) str->CharAt(i++)) 
		{
		case '0':
			if (nDigits > 0) 
			{
				nTrailingZeroes++;
			} 
			else 
			{
				exponent--;
			}
			break; // switch
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (nTrailingZeroes > 0) 
			{
				for (; nTrailingZeroes > 0; nTrailingZeroes--) 
				{
					digits[nDigits++] = '0';
				}
			}
			digits[nDigits++] = ival;
			break; // switch
		case 'E':
			decfound = true;
			break;
		}
	}
	if (i < strlen) 
	{
		int expSign;
		int expVal = 0;
		if (str->CharAt(i) == '-') 
		{
			expSign = -1;
			i++;
		} 
		else 
		{
			expSign = 1;
		}
		for (; i < strlen;) 
		{
			expVal = expVal
					* 10
					+ ((int) str->CharAt(i++) - (int) '0');
		}
		exponent += expVal * expSign;
	}
	exponent += decPos - nDigits;

	// gotta have at least a zero...
	if (nDigits == 0) 
	{
		digits[0] = '0';
		nDigits = 1;
		exponent = 0;
	}

	// OK, we now have the significand in digits[0...nDigits]
	// and the exponent in exponent.  We're ready to format.

	int intDigits, intZeroes, intLength;
	int decDigits, decZeroes, decLength;
	char expChar;

	if (negative) 
	{
		signChar = '-';
		width--;
	} 
	//else if (pad & PAD_PLUS) 
	//{
	//	signChar = '+';
	//	width--;
	//} 
	//else if ((flags & FLAG_SPACE) != 0) 
	//{
	//	signChar = ' ';
	//	width--;
	//} 
	else 
	{
		signChar = 0;
	}
	//if ((flags & FLAG_PRECISION) == 0) 
	//{
		precision = 6;
	//}

	switch (fchar)
	{
	case 'E':
	case 'G':
		expChar = 'E';
		break;
	case 'e':
	case 'g':
		expChar = 'e';
		break;
	default:
		expChar = 0;
	}

	switch (fchar) 
	{
	case 'g':
	case 'G':
		// an empirically derived rule: precision applies to
		// significand length, irrespective of exponent

		// an official rule, clarified: if the exponent
		// <clarif>after adjusting for exponent form</clarif>
		// is < -4,  or the exponent <clarif>after adjusting 
		// for exponent form</clarif> is greater than the
		// precision, use exponent form
		expForm = (exponent + nDigits - 1 < -4 || exponent
			   + nDigits > (precision == 0 ? 1
				: precision));
		// it would be nice (and logical!) if exponent form 
		// behaved like E/e, and decimal form behaved like f,
		// but no such luck. hence: 
		if (expForm) 
		{
			// intDigits isn't used here, but if it were, it would be 1
			// intDigits = 1; 
			decDigits = nDigits - 1;
			// precision for G/g includes integer digits
			precision = Math::Max(0, precision - 1);

			if (precision < decDigits) 
			{
				int n = _round_(digits, nDigits, precision, precision != 0);
				ASSERT_MEM(digits, strlen*sizeof(char));
				if (n > nDigits) 
				{
					nDigits = n;
				}
				decDigits = Math::Min(nDigits - 1, precision);
			}
			exponent += nDigits - 1;
			if (precision > 0) 
			{
				len += 1 + precision; // n.prec
			} 
			else 
			{
				len += 1; // n
				//if ((flags & FLAG_SHARP) != 0) 
				//{
				//	len++; // will have a trailing '.'
				//}
		   }

		   width -= len + 5; // 5 -> e+nnn / e-nnn

			if (width > 0 && (pad & (PAD_ZERO/* | FLAG_MINUS*/)) == 0) 
			{
				out->Fill(' ', width);
				width = 0;
			}
			if (signChar != 0) 
			{
				out->Append(signChar);
			}
			//if (width > 0 && (flags & FLAG_MINUS) == 0) 
			//{
			//	buf.fill('0', width);
			//	width = 0;
			//}
			// now some data...
			out->Append(digits[0]);
			if (precision > 0) 
			{
				out->Append('.'); // '.'
				if (decDigits > 0) 
				{
					out->Append(digits, 1, decDigits);
					precision -= decDigits;
				}
			} 
			//else if ((flags & FLAG_SHARP) != 0) 
			//{
			//	out->Append('.');
			//}
			out->Append(expChar); // E or e
			if ( exponent >= 0 )
				out->Append('-');
			if (exponent < 0) 
			{
				exponent = -exponent;
			}
			out->Append((char)(exponent / 100 + '0'));
			out->Append((char)(exponent % 100 / 10 + '0'));
			out->Append((char)(exponent % 10 + '0'));
			if (width > 0) 
			{
				out->Fill(' ', width);
			}
		} 
		else  // decimal form, like (but not *just* like!) 'f'
		{
			intDigits = Math::Max(0, Math::Min(nDigits + exponent, nDigits));
			intZeroes = Math::Max(0, exponent);
			intLength = intDigits + intZeroes;
			decDigits = nDigits - intDigits; 
			decZeroes = Math::Max(0, -(decDigits + exponent));
			decLength = decZeroes + decDigits;
			precision = Math::Max(0, precision - intLength);

			if (precision < decDigits) 
			{
				int n = _round_(digits, nDigits,
						intDigits + precision - 1,
						precision != 0);
				ASSERT_MEM(digits, strlen*sizeof(char));
				if (n > nDigits) 
				{
					// digits array shifted, update all
					nDigits = n;
					intDigits = Math::Max(0, Math::Min(nDigits + exponent, nDigits));
					intLength = intDigits + intZeroes;
					decDigits = nDigits - intDigits;
					decZeroes = Math::Max(0, -(decDigits + exponent));
					precision = Math::Max(0, precision - 1);
				}
				decDigits = precision;
				decLength = decZeroes + decDigits;
			}
			len += intLength;
			if (decLength > 0) 
			{
				len += decLength + 1;
			} 
			else 
			{
				//if ((flags & FLAG_SHARP) != 0) 
				//{
				//	len++; // will have a trailing '.'
				//	if (precision > 0) // g fills trailing zeroes if #
				//	{ 
				//		len += precision;
				//	}
				//}
			}

			width -= len;

			if (width > 0 && (pad & (PAD_ZERO/* | FLAG_MINUS*/)) == 0) 
			{
				out->Fill(' ', width);
				width = 0;
			}
			if (signChar != 0) 
			{
				out->Append(signChar);
			}
			//if (width > 0 && (flags & FLAG_MINUS) == 0) 
			//{
			//	out->Fill('0', width);
			//	width = 0;
			//}
			// now some data...
			if (intLength > 0) 
			{
				if (intDigits > 0)  // s/b true, since intLength > 0
				{
					out->Append(digits, 0, intDigits);
				}
				if (intZeroes > 0) 
				{
					out->Fill('0', intZeroes);
				}
			} 
			else 
			{
				// always need at least a 0
				out->Append('0');
			}
			if (decLength > 0 /*|| (flags & FLAG_SHARP) != 0*/) 
			{
				out->Append('.');
			}
			if (decLength > 0) 
			{
				if (decZeroes > 0) 
				{
					out->Fill('0', decZeroes);
					precision -= decZeroes;
				}
				if (decDigits > 0) 
				{
					out->Append(digits, intDigits, decDigits);
					precision -= decDigits;
				}
				//if ((flags & FLAG_SHARP) != 0 && precision > 0) 
				//{
				//	out->Fill('0', precision);
				//}
			}
			//if ((flags & FLAG_SHARP) != 0 && precision > 0) 
			//{
			//	out->Fill('0', precision);
			//}
			if (width > 0) 
			{
				out->Fill(' ', width);
			}
		}
		break;

	case 'f':
		intDigits = Math::Max(0, Math::Min(nDigits + exponent, nDigits));
		intZeroes = Math::Max(0, exponent);
		intLength = intDigits + intZeroes;
		decDigits = nDigits - intDigits;
		decZeroes = Math::Max(0, -(decDigits + exponent));
		decLength = decZeroes + decDigits;

		if (precision < decLength) 
		{
			if (precision < decZeroes) 
			{
				decDigits = 0;
				decZeroes = precision;
			} 
			else 
			{
				int n = _round_(digits, nDigits, intDigits + precision	- decZeroes - 1, precision != 0);
				ASSERT_MEM(digits, strlen*sizeof(char));
				if (n > nDigits) 
				{
					// digits arr shifted, update all
					nDigits = n;
					intDigits = Math::Max(0, Math::Min(nDigits + exponent, nDigits));
					intLength = intDigits + intZeroes;
					decDigits = nDigits - intDigits;
					decZeroes = Math::Max(0, -(decDigits + exponent));
					decLength = decZeroes + decDigits;
				}
				decDigits = precision - decZeroes;
			}
			decLength = decZeroes + decDigits;
		}
		if (precision > 0) 
		{
			len += Math::Max(1, intLength) + 1 + precision;
			// (1|intlen).prec
		} 
		else 
		{
			len += Math::Max(1, intLength);
			// (1|intlen)
			//if ((flags & FLAG_SHARP) != 0) 
			//{
			//	len++; // will have a trailing '.'
			//}
		}

		width -= len;

		if (width > 0 && (pad & (PAD_ZERO/* | FLAG_MINUS*/)) == 0) 
		{
			out->Fill(' ', width);
			width = 0;
		}
		if (signChar != 0) 
		{
			out->Append(signChar);
		}
		//if (width > 0 && (flags & FLAG_MINUS) == 0) 
		//{
		//	out->Fill('0', width);
		//	width = 0;
		//}
		// now some data...
		if (intLength > 0) 
		{
			if (intDigits > 0) // s/b true, since intLength > 0
			{ 
				out->Append(digits, 0, intDigits);
			}
			if (intZeroes > 0) 
			{
				out->Fill('0', intZeroes);
			}
		} 
		else 
		{
			// always need at least a 0
			out->Append('0');
		}
		if (precision > 0 /*|| (flags & FLAG_SHARP) != 0*/) 
		{
			out->Append('.');
		}
		if (precision > 0) 
		{
			if (decZeroes > 0) 
			{
				out->Fill('0', decZeroes);
				precision -= decZeroes;
			}
			if (decDigits > 0) 
			{
				out->Append(digits, intDigits, decDigits);
				precision -= decDigits;
			}
			// fill up the rest with zeroes
			if (precision > 0) 
			{
				out->Fill('0', precision);
			}
		}
		if (width > 0) 
		{
			out->Fill(' ', width);
		}
		break;
	case 'E':
	case 'e':
		// intDigits isn't used here, but if it were, it would be 1
		// intDigits = 1; 
		decDigits = nDigits - 1;

		if (precision < decDigits) 
		{
			int n = _round_(digits, nDigits, precision, precision != 0);
			if (n > nDigits) 
			{
				nDigits = n;
			}
			decDigits = Math::Min(nDigits - 1, precision);
		}
		exponent += nDigits - 1;
		if (precision > 0) 
		{
			len += 2 + precision; // n.prec
		} 
		else 
		{
			len += 1; // n
			//if ((pad & FLAG_SHARP) != 0) 
			//{
			//	len++; // will have a trailing '.'
			//}
		}

		width -= len + 5; // 5 -> e+nnn / e-nnn

		if (width > 0 && (pad & (PAD_ZERO/* | FLAG_MINUS*/)) == 0) 
		{
			out->Fill(' ', width);
			width = 0;
		}
		if (signChar != 0) 
		{
			out->Append(signChar);
		}
		//if (width > 0 && (flags & FLAG_MINUS) == 0) 
		//{
		//	out->Fill('0', width);
		//   width = 0;
		//}
		// now some data...
		out->Append(digits[0]);
		if (precision > 0) 
		{
			out->Append('.'); // '.'
			if (decDigits > 0) 
			{
				out->Append(digits, 1, decDigits);
				precision -= decDigits;
			}
			if (precision > 0) 
			{
				out->Fill('0', precision);
			}

		} 
		//else if ((flags & FLAG_SHARP) != 0) 
		//{
		//	out->Append(args.getDecimalSeparator());
		//}
		out->Append(expChar); // E or e
		out->Append(exponent >= 0 ? '+' : '-');
		if (exponent < 0) 
		{
			exponent = -exponent;
		}
		out->Append((char)(exponent / 100 + '0'));
		out->Append((char)(exponent % 100 / 10 + '0'));
		out->Append((char)(exponent % 10 + '0'));
		if (width > 0) 
		{
			out->Fill(' ', width);
		}
		break;
	} // switch (format char E,e,f,G,g)

	delete [] digits;
}

// Do not use a reference for sformat -- va args don't work with reference.
StringPtr String::FormatVA(const String sformat, va_list args)
{
	StringBuffer out;
	const char *format = sformat.GetChars();

	register int width = 0, pad = 0;
	char scr[2];

	for (; *format != 0; ++format) 
	{
		if (*format == '%') 
		{
			++format;
			width = pad = 0;
			if (*format == '\0') 
				break;
			if (*format == '%') 
			{
				out.Append((char)*format);
				continue;
			}
			if (*format == '-') 
			{
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') 
			{
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) 
			{
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) 
			{
				register char *s = (char *)va_arg( args, char * );
				_prints (&out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) 
			{
				_printi (&out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) 
			{
				_printi (&out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) 
			{
				_printi (&out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) 
			{
				_printi (&out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) 
			{
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				_prints (&out, scr, width, pad);
				continue;
			}
			if ( *format == 'f' )
			{
				_printdf(&out, *format, va_arg(args, double), width, pad);
				continue;
			}
			if ( *format == 'e' || *format == 'E')
			{
				_printdf(&out, *format, va_arg(args, double), width, pad);
				continue;
			}
			if ( *format == 'g' || *format == 'G' )
			{
				_printdf(&out, *format, va_arg(args, double), width, pad);
				continue;
			}
		}
		else 
		{
			out.Append((char) *format);
		}
	}
	return out.ToString();
}

// Do not use reference for format -- breaks va args.
StringPtr String::Format(const String format, ...)
{
	va_list args;
	va_start(args, format);
	
	StringPtr ret = FormatVA(format, args);

	va_end( args );
	return ret;
}

StringPtr String::StripQuotes()
{
	if ( CharAt(0) == '"' && CharAt( Length() -1 ) == '"' )
	{
		StringBuffer buf(m_cstr);
		buf.RemoveCharAt(0);
		buf.RemoveCharAt(buf.Length() - 1);
		return buf.ToString();
	}
	else
	{
		return StringPtr(new String(*this));
	}
}

StringPtr String::ToUpper() const
{
	StringBuffer buf(m_len+1);
	for (int x = 0; x < m_len; x++)
	{
		buf.Append((char)toupper(m_cstr[x]));
	}
	return buf.ToString();
}

StringPtr String::ToLower() const
{
	StringBuffer buf(m_len+1);
	for (int x = 0; x < m_len; x++)
	{
		buf.Append((char)tolower(m_cstr[x]));
	}
	return buf.ToString();
}

bool String::EndsWith( const String& scp ) const
{
	int pos = m_len - scp.Length();
	const char *cp = scp.m_cstr;
	int x;

	if ( pos <= 0 )
	{
		return false;
	}
	for ( x = pos; x < m_len; x++ )
	{
		if ( m_cstr[x] != *cp++ )
		{
			return false;
		}
	}
	return true;
}

bool String::StartsWith( const String& cp ) const
{
	int len = cp.Length();
	int x;

	if ( len > m_len )
	{
		return false;
	}
	for ( x = 0; x < len; x++ )
	{
		if ( m_cstr[x] != cp[x] )
		{
			return false;
		}
	}
	return true;
}

int String::LastIndexOf( const String& str, const int start ) const
{
	for ( int pos = start; pos > -1; pos-- )
	{
		int cnt = 0;
		while (cnt < str.Length() && m_cstr[pos - cnt] == str.CharAt(str.Length() - (cnt + 1)) )
		{
			cnt++;
		}
		if (cnt == str.Length())
		{
			return pos - cnt + 1;
		}
	}
	return -1;
}

bool String::HasNonPrintableCharacters() const
{
	for ( int x = 0; x < m_len; x++ )
	{
		if (! Char::IsPrint(m_cstr[x]))
		{
			return true;
		}
	}
	
	return false;
}

StringPtr String::RemoveNonPrintableCharacters() const
{
	StringBuffer buf (m_len + 1);
	
	for ( int x = 0; x < m_len; x++ )
	{
		if (Char::IsPrint(m_cstr[x]))
		{
			buf.Append(m_cstr[x]);
		}
	}

	return buf.ToString();	
}

int String::CountChar( const char *str, const int len, const char ch )
{
	int count = 0;
	for ( int x = 0; x < len; x++ )
	{
		if ( ch == str[x] )
		{
			count++;
		}
	}
	return count;
}

RefCountPtr<Array<byte> > String::ToByteArray() const
{
	RefCountPtr<Array<byte> > bytes(new Array<byte>(m_len));
	for ( int x = 0; x < m_len; x++ )
	{
		(*bytes)[x] = m_cstr[x];
	}

	return bytes;
}

namespace spl
{
	String operator +(const char *cp, const String& str)
	{
		return *String(cp).Cat(str);
	}
}

#if defined(DEBUG) || defined(_DEBUG)
void String::CheckMem() const
{
	if ( ! m_isintern )
	{
		DEBUG_NOTE_MEM_ALLOCATION( m_cstr );
	}
}

void String::ValidateMem() const
{
	if ( ! m_isintern )
	{
		ASSERT_MEM( m_cstr, m_len );
	}
}
#endif
