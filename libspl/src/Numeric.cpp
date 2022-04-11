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

#include <spl/types.h>

#ifdef HAVE_LIMITS_H
#include <limits.h>
#include <limits>
#endif

#include <spl/Char.h>
#include <spl/Exception.h>
#include <spl/Numeric.h>
#include <spl/Double.h>
#include <spl/IConvertable.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
#include <spl/math/Math.h>
#include <spl/text/StringBuffer.h>
#include <spl/UInt32.h>
#include <spl/UInt64.h>

using namespace spl;

char *int32toa( int32 i, char *buf, const int buflen )
{
	int pos = 0;
	if ( 0 == i )
	{
		buf[pos++] = '0';
	}
	else
	{
		while ( i > 0 && pos < buflen-1 )
		{
			buf[pos++] = (char)((i % 10) + (int)'0');
			i /= 10;
		}
		for ( i = 0; i < (pos)/2; i++ )
		{
			char ch = buf[i];
			buf[i] = buf[pos-1-i];
			buf[pos-1-i] = ch;
		}
	}
	buf[pos] = '\0';
	return buf;
}

static char *int64toa( int64 i, char *buf, const int buflen )
{
	int pos = 0;
	if ( 0 == i )
	{
		buf[pos++] = '0';
	}
	else
	{
		while ( i > 0 && pos < buflen-1 )
		{
			buf[pos++] = (char)((i % 10) + (int)'0');
			i /= 10;
		}
		for ( i = 0; i < pos-1; i++ )
		{
			char ch = buf[i];
			buf[i] = buf[pos-1-i];
			buf[pos-1-i] = ch;
		}
	}
	buf[pos] = '\0';
	return buf;
}

static bool isint( const char *cstr, const int len )
{
	int x = 0;
	if ( cstr[0] == '-' || cstr[0] == '+' )
	{
		x = 1;
	}
	for ( ; x < len; x++ )
	{
		if ( ! isdigit( cstr[x] ) )
		{
			return false;
		}
	}
	return true;
}

static const int base10 = 10;
static char cHexa[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
static int iHexaNumeric[] = { 10, 11, 12, 13, 14, 15 };
static int iHexaIndices[] = { 0, 1, 2, 3, 4, 5 };
static const int asciiDiff = 48;

static uint64 _baseToInt(const char *sbase, int numbase)
{
	int i;
	uint64 dec = 0;
	int b;
	int iProduct = 1;
	int sbaselen = (int)strlen(sbase);
	int stopAt = 0;
	
	if ( sbase[0] == '0' && (sbase[1] == 'x' || sbase[1] == 'X') )
	{
		stopAt = 2;
	}

	for ( i = sbaselen - 1; i >= stopAt; i--, iProduct *= numbase )
	{
		char ch = sbase[i];
		if ( numbase > base10 && isalpha(ch) )
		{
			ASSERT( 'A' > 'a' );
			if ( ch >= 'A' )
			{
				b = iHexaNumeric[ch-'A'];
			}
			else
			{
				b = iHexaNumeric[ch-'a'];
			}
		}
		else 
		{
			b = (int) sbase[i] - asciiDiff;
		}
		dec += (b * iProduct);
	} 
	return dec; 
}

static void FormatHex( byte b, char *buf )
{
	int nibble = b & 0xF;
	if ( nibble > 9 )
	{
		buf[1] = nibble - 10 + 'A';
	}
	else
	{
		buf[1] = nibble + '0';
	}
	nibble = b >> 4;
	if ( nibble > 9 )
	{
		buf[0] = nibble - 10 + 'A';
	}
	else
	{
		buf[0] = nibble + '0';
	}
	buf[2] = '\0';
}

Numeric::Numeric()
{
}

Numeric::~Numeric()
{
}

bool Numeric::IsNumeric(const char *cp, const int cplen)
{
	int x = 0;
	bool sawdot = false;
	bool sawe = false;

	if (cplen == 0)
	{
		return false;
	}
	if ( cp[0] == '-' || cp[x] == '+' )
	{
		x = 1;
	}
	for ( ; x < cplen && cp[x] != '\0'; x++ )
	{
		if ( ! isdigit( cp[x] ) )
		{
			if ( cp[x] == '.' )
			{
				if ( sawdot )
				{
					return false;
				}
				sawdot = true;
			}
			else if ( cp[x] == 'e' || cp[x] == 'E' )
			{
				if ( sawe )
				{
					return false;
				}
				sawe = true;
			}
			else if ( cp[x] == '-' )
			{
				if ( ! sawe )
				{
					return false;
				}
				if ( cp[x-1] != 'e' && cp[x] != 'E' )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	if (sawdot && cplen == 1)
	{
		return false;
	}
	return true;
}

Int32::~Int32()
{
}

uint32 Int32::HashCode() const
{
	return Math::Hash(m_val);
}

bool Int32::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Int32 *vp = static_cast<const Int32 *>(a);
	vp->ValidateMem();
	return m_val == vp->m_val;
}

int Int32::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Int32 *vp = static_cast<const Int32 *>(a);
	vp->ValidateMem();
	return m_val - vp->m_val;
}

bool Int32::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Int32& vp = static_cast<const Int32&>(a);
	vp.ValidateMem();
	return m_val == vp.m_val;
}

int Int32::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Int32& vp = static_cast<const Int32&>(a);
	vp.ValidateMem();
	return m_val - vp.m_val;
}

int32 Int32::MajicNumber() const
{
	return INT32_MAJIC;
}

int Int32::ToInt() const
{
	return m_val;
}

double Int32::ToDouble() const
{
	return (double)m_val;
}

StringPtr Int32::ToString() const
{
	return ToString(m_val);
}

#ifdef DEBUG
void Int32::ValidateMem() const
{
}

void Int32::CheckMem() const
{
}
#endif

int32 Int32::Parse(const char *cp, const int cplen, int radix)
{
	if (radix == 10)
	{
		return atoi(cp);
	}
	return (int32)_baseToInt(cp, radix);
}

bool Int32::IsInt(const char *cp, const int cplen, int radix)
{
	if (radix == 10)
	{
		return isint(cp, cplen);
	}
	if (radix != 16)
	{
		throw new InvalidArgumentException("Unsupported number base in Int32::IsInt().");
	}
	int pos = 0;

	if (cp[0] == '0' && (cp[1] == 'x' || cp[1] == 'X'))
	{
		pos = 2;
	}
	char ch;
	for ( int x = pos; x < cplen && cp[x] != '\0'; x++ )
	{
		ch = cp[x];
		if (Char::IsDigit(ch))
		{
			continue;
		}
		if (!isalpha(ch))
		{
			return false;
		}
		if (!((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')))
		{
			return false;
		}
	}
	return true;
}

StringPtr Int32::ToString(int32 i, int radix)
{
	if (10 == radix)
	{
		char buf[128];
		bool neg = i < 0;
		int bufpos = neg ? 1 : 0;
		if (neg)
		{
			i *= -1;
			buf[0] = '-';
		}

		int32toa(i, &buf[bufpos], sizeof(buf));

		return StringPtr(new String( buf ));
	}
	if (16 != radix)
	{
		throw new InvalidArgumentException("Unsupported radix in Int32::IsInt().");
	}

	char buf[3];
	StringBuffer hex;
	FormatHex( ((byte *)&i)[3], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[2], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[1], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[0], &buf[0] );
	hex.Append(buf);

	return hex.ToString()->LTrim('0');
}


Int64::Int64()
: m_val(0)
{
}

Int64::Int64(const int64 i)
: m_val(i)
{
}

Int64::Int64(const Int64& i)
: m_val(i.m_val)
{
}

Int64::~Int64()
{
}

uint32 Int64::HashCode() const
{
	return Math::Hash(m_val);
}

bool Int64::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Int64 *vp = static_cast<const Int64 *>(a);
	vp->ValidateMem();
	return m_val == vp->m_val;
}

int Int64::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Int64 *vp = static_cast<const Int64 *>(a);
	vp->ValidateMem();
	return (int)(m_val - vp->m_val);
}

bool Int64::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Int64& vp = static_cast<const Int64&>(a);
	vp.ValidateMem();
	return m_val == vp.m_val;
}

int Int64::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Int64& vp = static_cast<const Int64&>(a);
	vp.ValidateMem();
	return (int)(m_val - vp.m_val);
}

int32 Int64::MajicNumber() const
{
	return INT64_MAJIC;
}

int Int64::ToInt() const
{
	return (int)m_val;
}

double Int64::ToDouble() const
{
	return (double)m_val;
}

StringPtr Int64::ToString() const
{
	return ToString(m_val);
}

#ifdef DEBUG
void Int64::ValidateMem() const
{
}

void Int64::CheckMem() const
{
}
#endif

int64 Int64::Parse(const char *cp, const int cplen)
{
	return atol(cp);
}

bool Int64::IsInt(const char *cp, const int cplen)
{
	return isint(cp, cplen);
}

StringPtr Int64::ToString(int64 i, int radix)
{
	if (10 == radix)
	{
		char buf[128];
		bool neg = i < 0;
		int bufpos = neg ? 1 : 0;
		if (neg)
		{
			i *= -1;
			buf[0] = '-';
		}
		int64toa(i, &buf[bufpos], 128);
		return StringPtr(new String( buf ));
	}
	if (16 != radix)
	{
		throw new InvalidArgumentException("Unsupported radix in Int32::IsInt().");
	}

	char buf[3];
	StringBuffer hex;
	FormatHex( ((byte *)&i)[7], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[6], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[5], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[4], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[3], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[2], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[1], &buf[0] );
	hex.Append(buf);
	FormatHex( ((byte *)&i)[0], &buf[0] );
	hex.Append(buf);

	return hex.ToString()->LTrim('0');
}


UInt32::UInt32()
: m_val(0)
{
}

UInt32::UInt32(const uint32 i)
: m_val(i)
{
}

UInt32::UInt32(const UInt32& i)
: m_val(i.m_val)
{
}

UInt32::~UInt32()
{
}

uint32 UInt32::HashCode() const
{
	return Math::Hash(m_val);
}

bool UInt32::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const UInt32 *vp = static_cast<const UInt32 *>(a);
	vp->ValidateMem();
	return m_val == vp->m_val;
}

int UInt32::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const UInt32 *vp = static_cast<const UInt32 *>(a);
	vp->ValidateMem();
	return m_val - vp->m_val;
}

bool UInt32::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const UInt32& vp = static_cast<const UInt32&>(a);
	vp.ValidateMem();
	return m_val == vp.m_val;
}

int UInt32::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const UInt32& vp = static_cast<const UInt32&>(a);
	vp.ValidateMem();
	return m_val - vp.m_val;
}

int32 UInt32::MajicNumber() const
{
	return UINT32_MAJIC;
}

int UInt32::ToInt() const
{
	return (int)m_val;
}

double UInt32::ToDouble() const
{
	return (double)m_val;
}

StringPtr UInt32::ToString() const
{
	return ToString(m_val);
}

#ifdef DEBUG
void UInt32::ValidateMem() const
{
}

void UInt32::CheckMem() const
{
}
#endif

uint32 UInt32::Parse(const char *cp, const int cplen, int radix)
{
	if (radix == 10)
	{
		return atoi(cp);
	}
	return (uint32)_baseToInt(cp, radix);
}

StringPtr UInt32::ToString(uint32 i, int radix)
{
	return Int64::ToString((int64)i, radix);
}


UInt64::UInt64()
: m_val(0)
{
}

UInt64::UInt64(const uint64 i)
: m_val(i)
{
}

UInt64::UInt64(const UInt64& i)
: m_val(i.m_val)
{
}

UInt64::~UInt64()
{
}

uint32 UInt64::HashCode() const
{
	return Math::Hash(m_val);
}

bool UInt64::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const UInt64 *vp = static_cast<const UInt64 *>(a);
	vp->ValidateMem();
	return m_val == vp->m_val;
}

int UInt64::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const UInt64 *vp = static_cast<const UInt64 *>(a);
	vp->ValidateMem();
	return (int)(m_val - vp->m_val);
}

bool UInt64::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const UInt64& vp = static_cast<const UInt64&>(a);
	vp.ValidateMem();
	return m_val == vp.m_val;
}

int UInt64::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const UInt64& vp = static_cast<const UInt64&>(a);
	vp.ValidateMem();
	return (int)(m_val - vp.m_val);
}

int32 UInt64::MajicNumber() const
{
	return UINT64_MAJIC;
}

int UInt64::ToInt() const
{
	return (int)m_val;
}

double UInt64::ToDouble() const
{
	return (double)(int64)m_val;
}

StringPtr UInt64::ToString() const
{
	return ToString(m_val);
}

#ifdef DEBUG
void UInt64::ValidateMem() const
{
}

void UInt64::CheckMem() const
{
}
#endif

uint64 UInt64::Parse(const char *cp, const int cplen, int radix)
{
	if (radix == 10)
	{
		return atoi(cp);
	}
	return _baseToInt(cp, radix);
}

StringPtr UInt64::ToString(uint64 i, int radix)
{
	return Int64::ToString((int64)i, radix);
}


Double::Double()
: m_val(0)
{
}

Double::Double(const double d)
: m_val(d)
{
}

Double::Double(const Double& d)
: m_val(d.m_val)
{
}

Double::~Double()
{
}

uint32 Double::HashCode() const
{
	return Math::Hash(m_val);
}

bool Double::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Double *vp = static_cast<const Double *>(a);
	vp->ValidateMem();
	return m_val == vp->m_val;
}

int Double::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Double *vp = static_cast<const Double *>(a);
	vp->ValidateMem();
	return (int)(m_val - vp->m_val);
}

bool Double::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Double& vp = static_cast<const Double&>(a);
	vp.ValidateMem();
	return m_val == vp.m_val;
}

int Double::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Double& vp = static_cast<const Double&>(a);
	vp.ValidateMem();
	return (int)(m_val - vp.m_val);
}

int32 Double::MajicNumber() const
{
	return DOUBLE_MAJIC;
}

int Double::ToInt() const
{
	return (int)m_val;
}

double Double::ToDouble() const
{
	return (double)m_val;
}

StringPtr Double::ToString() const
{
	return ToString(m_val);
}

bool Double::IsNaN(double d)
{
	return std::numeric_limits<double>::quiet_NaN() == d;
}

bool Double::IsPositiveZero(double d)
{
	return d == 0.0;
}

bool Double::IsNegativeZero(double d)
{
	return d == -0.0;
}

double Double::PositiveInfinity()
{
	return HUGE_VAL;
}

double Double::NegativeInfinity()
{
	return -HUGE_VAL;
}

#ifdef DEBUG
void Double::ValidateMem() const
{
}

void Double::CheckMem() const
{
}
#endif

float64 Double::Parse(const char *cp, const int cplen)
{
	return atof(cp);
}

bool Double::IsDouble(const char *cp, const int cplen)
{
	return Numeric::IsNumeric(cp, cplen);
}

StringPtr Double::ToString(float64 d)
{
	char buf[256];
	sprintf(buf, "%f", d);
	StringPtr ret = String( buf ).RTrim('0');
	if (ret->EndsWith('.'))
	{
		return ret->Substring(0, ret->Length()-1);
	}
	return ret;
}

Char::Char()
: m_val(0)
{
}

Char::Char(const Char& i)
: m_val(i.m_val)
{
}

Char::Char(const int i)
: m_val(i)
{
}

Char::~Char()
{
}

bool Char::IsWhiteSpace(const int32 c)
{
	switch(c)
	{
	case 0x0020:
	case 0x1680:
	case 0x180E:
	case 0x2000:
	case 0x2001:
	case 0x2002:
	case 0x2003:
	case 0x2004:
	case 0x2005:
	case 0x2006:
	case 0x2007:
	case 0x2008:
	case 0x2009:
	case 0x200A:
	case 0x202F:
	case 0x205F:
	case 0x3000:
	case 0x2028:
	case 0x2029:
	case 0x0009:
	case 0x000A:
	case 0x000B:
	case 0x000C:
	case 0x000D:
	case 0x0085:
	case 0x0000A0:
		return true;
	}
	return false;
}

uint32 Char::HashCode() const
{
	if (m_val < 256)
	{
		return Math::Hash((char)m_val);
	}
	return Math::Hash(m_val);
}

bool Char::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Char *c = static_cast<const Char *>(a);
	return m_val == c->m_val;
}

int Char::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Char *c = static_cast<const Char *>(a);
	return m_val - c->m_val;
}

bool Char::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Char& c = static_cast<const Char&>(a);
	return m_val == c.m_val;
}

int Char::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Char& c = static_cast<const Char&>(a);
	return m_val - c.m_val;
}

int32 Char::MajicNumber() const
{
	return CHAR_MAJIC;
}

int Char::ToInt() const
{
	return m_val;
}

double Char::ToDouble() const
{
	return (double)m_val;
}

StringPtr Char::ToString() const
{
	return Char::ToString(m_val);
}

StringPtr Char::ToString(char c)
{
	return Char::ToString((int32)c);
}

StringPtr Char::ToString(int32 c)
{
	char buf[5];
	buf[0] = 0xFF & c;
	if (c < 256)
	{
		buf[1] = '\0';
	}
	else
	{
		buf[1] = 0xFF & (c >> 8);
		buf[2] = 0xFF & (c >> 16);
		buf[3] = 0xFF & (c >> 24);
		buf[4] = '\0';
	}
	return StringPtr(new String(buf));
}

#ifdef DEBUG
void Char::ValidateMem() const
{
}

void Char::CheckMem() const
{
}
#endif

IConvertable::IConvertable()
{
}

IConvertable::~IConvertable()
{
}
