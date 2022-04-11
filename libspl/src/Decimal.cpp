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
#include <math.h>
#include <spl/Decimal.h>
#include <spl/Double.h>
#include <spl/math/Math.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>
#include <stdio.h>

using namespace spl;

int const Decimal::precision = 4;
int64 const Decimal::q = (int64)pow(10.0, (double)precision);

Decimal::Decimal(void)
: n(0)
{
}

Decimal::Decimal(const Decimal& d)
: n(d.n)
{
}

Decimal::Decimal(const int64 i)
: n(i * q)
{
}

Decimal::Decimal(const int32 i)
: n(i * q)
{
}

Decimal::Decimal(const int16 i)
: n(i * q)
{
}

Decimal::Decimal(int64 intPart, int64 fractPart)
: n(intPart * q + fractPart)
{
}

Decimal::Decimal(const double d)
{
	double intPart;
	double fract = modf(d, &intPart);
	Init((int64)intPart, (int64)(fract*q));
}

Decimal::Decimal(const float d)
{
	double intPart;
	float fract = (float)modf((const double)d, &intPart);
	Init((int64)intPart, (int64)(fract*q));
}

Decimal Decimal::Parse(const String& num)
{
	return Decimal(Double::Parse(num));
}

Decimal::~Decimal(void)
{
}

Decimal& Decimal::operator =(const Decimal& d)
{
	n = d.n;
	return *this;
}

Decimal& Decimal::operator =(const int32 d)
{
	n=d;
	n*=q;
	return *this;
}

Decimal& Decimal::operator =(const int64 d)
{
	n=d;
	n*=q;
	return *this;
}

Decimal& Decimal::operator =(const float d)
{
	*this = Decimal(d);
	return *this;
}

Decimal& Decimal::operator =(const double d)
{
	double intPart;
	double fract = modf(d, &intPart);
	Init((int64)intPart, (int64)(fract*q));
	return *this;
}

Decimal Decimal::operator +(const Decimal& d) const
{
	return Decimal::Intern(n + d.n);
}

Decimal Decimal::operator -(const Decimal& d) const
{
	return Decimal::Intern(n - d.n);
}

Decimal Decimal::operator *(const Decimal& d) const
{
	return Decimal::Intern(n * d.n / q);
}

Decimal Decimal::operator /(const Decimal& d) const
{
	return Decimal::Intern(n * q / d.n);
}

Decimal Decimal::operator +=(const Decimal& d)
{
	n = n + d.n;
	return *this;
}

Decimal Decimal::operator -=(const Decimal& d)
{
	n = n - d.n;
	return *this;
}

Decimal Decimal::operator *=(const Decimal& d)
{
	n = n * d.n / q;
	return *this;
}

Decimal Decimal::operator /=(const Decimal& d)
{
	n = n * q / d.n;
	return *this;
}

bool Decimal::operator ==(const Decimal& d) const
{
	return n == d.n;
}

bool Decimal::operator !=(const Decimal& d) const
{
	return n != d.n;
}

bool Decimal::operator <(const Decimal& d) const
{
	return n < d.n;
}

bool Decimal::operator <=(const Decimal& d) const
{
	return n <= d.n;
}

bool Decimal::operator >(const Decimal& d) const
{
	return n > d.n;
}

bool Decimal::operator >=(const Decimal& d) const
{
	return n >= d.n;
}

Decimal Decimal::Round()
{
	int64 n2=n/q;
	int fract=(int)(n-n2*q);
	if ( fract > q/2 )
	{
		return Decimal(n2+1, 0);
	}
	return Decimal(n2, 0);
}

uint32 Decimal::HashCode() const
{
	return Math::Hash((n & 0xFFFFFFFF) ^ (n >> 32));
}

bool Decimal::Equals(Decimal *d) const
{
	return n == d->n;
}

bool Decimal::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Decimal *vp = static_cast<const Decimal *>(a);
	vp->ValidateMem();
	return Equals(vp);
}

int Decimal::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Decimal *vp = static_cast<const Decimal *>(a);
	vp->ValidateMem();
	return (int)(n - vp->n);
}

bool Decimal::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const Decimal& vp = static_cast<const Decimal&>(a);
	vp.ValidateMem();
	return Equals(vp);
}

int Decimal::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Decimal& vp = static_cast<const Decimal&>(a);
	vp.ValidateMem();
	return (int)(n - vp.n);
}

int32 Decimal::MajicNumber() const
{
	return DECIMAL_MAJIC;
}

int Decimal::ToInt() const
{
	return (int)ToLongInt();
}

double Decimal::ToDouble() const
{
	return Double::Parse(*ToString());
}

StringPtr Decimal::ToString() const
{
	char s[64];
	int64 n2=n / q;
	int fract = (int)(n-n2*q);
	sprintf(s, "%d.%0*d", (int)n2, precision, fract);
	return StringPtr(new String(s));
}

#ifdef DEBUG
void Decimal::ValidateMem() const
{
}

void Decimal::CheckMem() const
{
}
#endif
