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
#ifndef _BigInteger_h
#define _BigInteger_h

#include <spl/types.h>
#include <spl/collection/Array.h>
#include <spl/math/Random.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define BIGINTEGER_MAJIC 721		//< Majic number for ASSERT's in Compare and Convert

class BigInteger;
typedef RefCountPtr<BigInteger> BigIntegerPtr;
typedef WeakReference<BigInteger, BigIntegerPtr> BigIntegerRef;

///@brief Ported from http://www.bouncycastle.org/csharp/index.html
class BigInteger : public IComparable, public IMemoryValidate
{
// STATIC's
private:

#define BIGINTEGER_PRIMELIST_ROWS 52
#define BIGINTEGER_PRIMELIST_COLS 9

	static const int m_primeLists[BIGINTEGER_PRIMELIST_ROWS][BIGINTEGER_PRIMELIST_COLS];
	static int m_primeProducts[BIGINTEGER_PRIMELIST_ROWS];
	static bool m_primeProductsInited;

	static const byte m_rndMask[];

#define BIGINTEGER_IMASK ((int64)0xffffffffL)
#define	BIGINTEGER_UIMASK ((uint64)0xffffffffL)

	static const int m_chunk2; // TODO Parse 64 bits at a time
	static const int m_chunk10;
	static const int m_chunk16;

#define BitsPerByte 8
#define BitsPerInt 32
#define BytesPerInt 4

	static Random m_randomSource;
	static BigIntegerPtr m_zero;
	static BigIntegerPtr m_one;
	static BigIntegerPtr m_two;

	static void Init2();

	inline static void Init()
	{
		if (m_primeProductsInited)
		{
			return;
		}
		Init2();
	}

	static int GetByteLength(int nBits)
	{
		return (nBits + BitsPerByte - 1) / BitsPerByte;
	}

	static BigIntegerPtr CreateUValueOf(uint64 value);
	static BigIntegerPtr CreateValueOf(int64 value);

	static RefCountPtr<Array<int32> > MakeMagnitude(const Array<byte>& bytes, int offset, int length);

	static int CompareNoLeadingZeroes(int xIndx, const Array<int32>& x, int yIndx, const Array<int32>& y);

	inline static int BitLen(int w)
	{
		// Binary search - decision tree (5 tests, rarely 6)
		return (w < 1 << 15 ? (w < 1 << 7
			? (w < 1 << 3 ? (w < 1 << 1
			? (w < 1 << 0 ? (w < 0 ? 32 : 0) : 1)
			: (w < 1 << 2 ? 2 : 3)) : (w < 1 << 5
			? (w < 1 << 4 ? 4 : 5)
			: (w < 1 << 6 ? 6 : 7)))
			: (w < 1 << 11
			? (w < 1 << 9 ? (w < 1 << 8 ? 8 : 9) : (w < 1 << 10 ? 10 : 11))
			: (w < 1 << 13 ? (w < 1 << 12 ? 12 : 13) : (w < 1 << 14 ? 14 : 15)))) : (w < 1 << 23 ? (w < 1 << 19
			? (w < 1 << 17 ? (w < 1 << 16 ? 16 : 17) : (w < 1 << 18 ? 18 : 19))
			: (w < 1 << 21 ? (w < 1 << 20 ? 20 : 21) : (w < 1 << 22 ? 22 : 23))) : (w < 1 << 27
			? (w < 1 << 25 ? (w < 1 << 24 ? 24 : 25) : (w < 1 << 26 ? 26 : 27))
			: (w < 1 << 29 ? (w < 1 << 28 ? 28 : 29) : (w < 1 << 30 ? 30 : 31)))));
	}

	static RefCountPtr<Array<int32> > DoSubBigLil(Array<int32>& bigMag, Array<int32>& lilMag);

	static void ShiftRightOneInPlace(int start, Array<int32>& mag);
	static void ShiftRightInPlace(int start, Array<int32>& mag, int n);
	static RefCountPtr<Array<int32> > ShiftLeft(Array<int32>& mag, int n);

	static void AddMagnitudes(Array<int32>& a, Array<int32>& b);

	static const BigInteger& Two();

	static void Subtract
	(
		int xIndx,
		Array<int32>& x,
		int yIndx,
		Array<int32>& y
	);

	static void Multiply
	(
		Array<int32>&	x,
		Array<int32>&	y,
		Array<int32>&	z
	);

	static void MultiplyMonty
	(
		Array<int32>&	a,
		Array<int32>&	x,
		Array<int32>&	y,
		Array<int32>&	m,
		int64			mQuote
	);

	static uint32 MultiplyMontyNIsOne
	(
		uint32	x,
		uint32	y,
		uint32	m,
		uint64	mQuote
	);

	static int CompareTo
	(
		int xIndx,
		Array<int32>& x,
		int yIndx,
		Array<int32>& y
	);

	static RefCountPtr<Array<int32> > Square
	(
		RefCountPtr<Array<int32> >	w,
		RefCountPtr<Array<int32> >	x
	);

	static BigIntegerPtr ExtEuclid
	(
		const BigInteger&	a,
		const BigInteger&	b,
		BigIntegerPtr u1Out,
		BigIntegerPtr u2Out
	);

// CLASS MEMBERS
private:
	int32 m_sign;				// -1 means -ve; +1 means +ve; 0 means 0;
	RefCountPtr<Array<int32> > m_magnitude;	// array of ints with [0] being the most significant
	int32 m_nBits;				// cache BitCount() value
	int32 m_nBitLength;			// cache calcBitLength() value
	int64 m_mQuote;				// -m^(-1) mod b, b = 2^32 (see Montgomery mult.)

	inline void ConstructCommon()
	{
		Init();
		m_nBits = -1;
		m_nBitLength = -1;
		m_mQuote = -1L;
	}

	inline bool QuickPow2Check() const
	{
		return m_sign > 0 && m_nBits == 1;
	}

	BigInteger(int signNum, RefCountPtr<Array<int32> > mag, bool checkMag);

	void ConstructWith(int signNum, RefCountPtr<Array<int32> > mag, bool checkMag);
	void ConstructWith(Array<byte>& bytes, int offset, int length);
	void ConstructWith(int sign, Array<byte>& bytes, int offset, int length);

	int64 GetMQuote();
	bool CheckProbablePrime(int certainty, Random& random) const;
	bool RabinMillerTest(int certainty, Random& random) const;

	int CalcBitLength(int indx, const Array<int32>& mag) const;
	RefCountPtr<Array<int32> > LastNBits(int n) const;

	BigIntegerPtr AddToMagnitude(RefCountPtr<Array<int32> > magToAdd) const;

	int32 Remainder(int m) const;
	void Remainder(RefCountPtr<Array<int32> > x, RefCountPtr<Array<int32> > y) const;

	RefCountPtr<Array<int32> > Divide
	(
		RefCountPtr<Array<int32> > x,
		RefCountPtr<Array<int32> > y
	) const;

	BigIntegerPtr Inc() const;
	BigIntegerPtr FlipExistingBit(int n) const;

	inline BigIntegerPtr AndNot(const BigInteger& val) const
	{
		BigIntegerPtr tmp = val.Not();
		return And(*tmp);
	}

	void ToByteArray(Array<byte>& a, bool isUnsigned) const;

public:
	BigInteger();
	BigInteger(Array<byte>& bytes);
	BigInteger(Array<byte>& bytes, int offset, int length);
	BigInteger(int sign, Array<byte>& bytes, int offset, int length);
	BigInteger(int sign, Array<byte>& bytes);
	BigInteger(int sizeInBits, Random& random);
	BigInteger(int bitLength, int certainty, Random& random);

	BigInteger(const BigInteger& toCopy);
	BigInteger& operator =(const BigInteger& toAssign);
	BigInteger& operator =(const int64 i);
	virtual ~BigInteger();

	bool TestBit(int n) const;

	inline int GetBitLength() const
	{
		return m_nBitLength;
	}

	int GetLowestSetBit() const;

	inline BigIntegerPtr Abs() const
	{
		return m_sign >= 0 ? BigIntegerPtr(new BigInteger(*this)) : Negate();
	}

	inline BigIntegerPtr Not() const
	{
		return Inc()->Negate();
	}

	BigIntegerPtr And(const BigInteger& value) const;
	BigIntegerPtr Or(const BigInteger& value) const;
	BigIntegerPtr Xor(const BigInteger& value) const;
	BigIntegerPtr SetBit(int n) const;
	BigIntegerPtr ClearBit(int n) const;
	BigIntegerPtr FlipBit(int n) const;

	inline BigIntegerPtr Negate() const
	{
		if (m_sign == 0)
			return BigIntegerPtr(new BigInteger(*this));

		return BigIntegerPtr(new BigInteger(-m_sign, RefCountPtr<Array<int32> >(m_magnitude->Clone()), false));
	}

	BigIntegerPtr ShiftRight(int n) const;
	BigIntegerPtr ShiftLeft(int n) const;

	BigIntegerPtr Remainder(const BigInteger& n) const;
	BigIntegerPtr ModPow(const BigInteger& exponent, BigInteger& m);
	BigIntegerPtr Mod(const BigInteger &m) const;
	BigIntegerPtr ModInverse(const BigInteger& m) const;

	BigIntegerPtr Multiply(const BigInteger& val) const;

	BigIntegerPtr Divide(const BigInteger& val) const;
	Array<BigIntegerPtr> DivideAndRemainder(const BigInteger& val) const;

	BigIntegerPtr Add(const BigInteger& value) const;
	BigIntegerPtr Subtract(const BigInteger& n) const;

	BigIntegerPtr Gcd(const BigInteger& value) const;
	bool IsProbablePrime(int certainty) const;

	inline BigInteger& operator +=(const BigInteger& bi) { *this = *Add(bi); return *this; }
	inline BigInteger& operator -=(const BigInteger& bi) { *this = *Subtract(bi); return *this; }
	inline BigInteger& operator *=(const BigInteger& bi) { *this = *Multiply(bi); return *this; }
	inline BigInteger& operator /=(const BigInteger& bi) { *this = *Divide(bi); return *this; }

	inline BigInteger& operator <<=(int shiftVal) { *this = *ShiftLeft(shiftVal); return *this; }
	inline BigInteger& operator >>=(int shiftVal) { *this = *ShiftRight(shiftVal); return *this; }

	BigInteger& operator %=(const BigInteger& bi) { *this = *Mod(bi); return *this; }
	BigInteger& operator &=(const BigInteger& bi) { *this = *And(bi); return *this; }
	BigInteger& operator |=(const BigInteger& bi) { *this = *Or(bi); return *this; }
	BigInteger& operator ^=(const BigInteger& bi) { *this = *Xor(bi); return *this; }

	inline friend BigInteger operator +(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.Add(bi2); }
	inline friend BigInteger operator -(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.Subtract(bi2); }
	inline friend BigInteger operator *(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.Multiply(bi2); }
	inline friend BigInteger operator /(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.Divide(bi2); }
	inline friend BigInteger operator %(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.Mod(bi2); }

	inline friend BigInteger operator +(const BigInteger& bi1, int64 i) { BigInteger tmp = *ValueOf(i); return *bi1.Add(tmp); }
	inline friend BigInteger operator -(const BigInteger& bi1, int64 i) { BigInteger tmp = *ValueOf(i); return *bi1.Subtract(tmp); }
	inline friend BigInteger operator *(const BigInteger& bi1, int64 i) { BigInteger tmp = *ValueOf(i); return *bi1.Multiply(tmp); }
	inline friend BigInteger operator /(const BigInteger& bi1, int64 i) { BigInteger tmp = *ValueOf(i); return *bi1.Divide(tmp); }
	inline friend BigInteger operator %(const BigInteger& bi1, int64 i) { BigInteger tmp = *ValueOf(i); return *bi1.Mod(tmp); }

	inline friend BigInteger operator &(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.And(bi2); }
	inline friend BigInteger operator |(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.Or(bi2); }
	inline friend BigInteger operator ^(const BigInteger& bi1, const BigInteger& bi2) { return *bi1.Xor(bi2); }
	
	inline BigInteger operator <<(int shiftVal) const { return *ShiftLeft(shiftVal); }
	inline BigInteger operator >>(int shiftVal) const { return *ShiftRight(shiftVal); }
	
	inline friend BigInteger operator -(const BigInteger& bi) { return *bi.Negate(); }
	friend BigInteger operator ~(const BigInteger& bi);
	inline friend BigInteger operator !(const BigInteger& bi) { return *bi.Not(); }

	inline friend bool operator ==(const BigInteger& bi1, const BigInteger& bi2) { return bi1.Equals(bi2); }
	inline friend bool operator ==(const BigInteger& bi1, int64 i) { BigInteger tmp = *ValueOf(i); return bi1 == tmp; }
	inline friend bool operator !=(const BigInteger& bi1, const BigInteger& bi2) { return !bi1.Equals(bi2); }
	inline friend bool operator !=(const BigInteger& bi1, int64 i) { return bi1 != i; }
	inline friend bool operator >(const BigInteger& bi1, const BigInteger& bi2) { return bi1.Compare(bi2) > 0; }
	inline friend bool operator <(const BigInteger& bi1, const BigInteger& bi2) { return bi1.Compare(bi2) < 0; }
	inline friend bool operator >=(const BigInteger& bi1, const BigInteger& bi2) { return bi1.Compare(bi2) >= 0; }
	inline friend bool operator <=(const BigInteger& bi1, const BigInteger& bi2) { return bi1.Compare(bi2) <= 0; }

	inline friend bool operator &&(const BigInteger& bi1, const BigInteger& bi2) { BigInteger tmp = *bi1.And(bi2); return tmp != 0; }
	inline friend bool operator ||(const BigInteger& bi1, const BigInteger& bi2) { BigInteger tmp = *bi1.Or(bi2); return tmp != 0; }

	String ToString() const;

	inline Array<byte> ToByteArrayUnsigned() const
	{
		Array<byte> a;
		ToByteArray(a, true);
		return a;
	}

	inline void ToByteArrayUnsigned(Array<byte>& a) const
	{
		ToByteArray(a, true);
	}

	inline Array<byte> ToByteArray() const
	{
		Array<byte> a;
		ToByteArray(a, false);
		return a;
	}

	inline void ToByteArray(Array<byte>& a) const
	{
		ToByteArray(a, false);
	}

	inline int32 ToInt() const
	{
		return m_sign == 0 ? 0
			: m_sign > 0 ? (*m_magnitude)[m_magnitude->Length() - 1]
			: -(*m_magnitude)[m_magnitude->Length() - 1];
	}

	inline int64 ToInt64() const
	{
		if( m_sign == 0 )
		{
			return 0;
		}

		int64 i = (uint32)(*m_magnitude)[m_magnitude->Length() - 1];
		if (m_magnitude->Count() > 1)
		{
			int64 i2 = (uint32)(*m_magnitude)[m_magnitude->Length() - 2];
			i |= i2 << 32;
		}

		if (m_sign < 0)
		{
			return -i;
		}
		return i;
	}

	inline uint32 ToUInt() const
	{
		return (*m_magnitude)[m_magnitude->Length() - 1];
	}

	virtual bool Equals( const IComparable *a ) const;
	virtual int Compare( const IComparable *a ) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	bool Equals(int64 i) const;

	///@brief Class instances with the same majic number are of the same type (can be casted).
	/// Majic numbers above 0xFFFF are available for user applications.
	virtual int32 MajicNumber() const;
	virtual uint32 HashCode() const;

	inline static BigIntegerPtr ValueOf(int64 i) { return CreateValueOf(i); }
	static const BigInteger& Zero();
	static BigIntegerPtr ZeroPtr();
	static const BigInteger& One();
	static BigIntegerPtr OnePtr();

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;

	static void CheckMemStatics();
	static void DebugClearStatics();
#endif
};

/** @} */
}
#endif
