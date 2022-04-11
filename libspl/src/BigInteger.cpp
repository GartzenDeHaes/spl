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
#include <spl/BigInteger.h>
#include <spl/Exception.h>
#include <spl/Int32.h>
#include <spl/Int64.h>
#include <spl/math/Math.h>
#include <spl/UInt32.h>

using namespace spl;

Random BigInteger::m_randomSource;

BigIntegerPtr BigInteger::m_zero;
BigIntegerPtr BigInteger::m_one;
BigIntegerPtr BigInteger::m_two;

// Each list has a product < 2^31
const int BigInteger::m_primeLists[BIGINTEGER_PRIMELIST_ROWS][BIGINTEGER_PRIMELIST_COLS] =
{
	{ 3, 5, 7, 11, 13, 17, 19, 23, -1 },
	{ 29, 31, 37, 41, 43, -1 },
	{ 47, 53, 59, 61, 67, -1 },
	{ 71, 73, 79, 83, -1 },
	{ 89, 97, 101, 103, -1 },

	{ 107, 109, 113, 127, -1 },
	{ 131, 137, 139, 149, -1 },
	{ 151, 157, 163, 167, -1 },
	{ 173, 179, 181, 191, -1 },
	{ 193, 197, 199, 211, -1 },

	{ 223, 227, 229, -1 },
	{ 233, 239, 241, -1 },
	{ 251, 257, 263, -1 },
	{ 269, 271, 277, -1 },
	{ 281, 283, 293, -1 },

	{ 307, 311, 313, -1 },
	{ 317, 331, 337, -1 },
	{ 347, 349, 353, -1 },
	{ 359, 367, 373, -1 },
	{ 379, 383, 389, -1 },

	{ 397, 401, 409, -1 },
	{ 419, 421, 431, -1 },
	{ 433, 439, 443, -1 },
	{ 449, 457, 461, -1 },
	{ 463, 467, 479, -1 },

	{ 487, 491, 499, -1 },
	{ 503, 509, 521, -1 },
	{ 523, 541, 547, -1 },
	{ 557, 563, 569, -1 },
	{ 571, 577, 587, -1 },

	{ 593, 599, 601, -1 },
	{ 607, 613, 617, -1 },
	{ 619, 631, 641, -1 },
	{ 643, 647, 653, -1 },
	{ 659, 661, 673, -1 },

	{ 677, 683, 691, -1 },
	{ 701, 709, 719, -1 },
	{ 727, 733, 739, -1 },
	{ 743, 751, 757, -1 },
	{ 761, 769, 773, -1 },

	{ 787, 797, 809, -1 },
	{ 811, 821, 823, -1 },
	{ 827, 829, 839, -1 },
	{ 853, 857, 859, -1 },
	{ 863, 877, 881, -1 },

	{ 883, 887, 907, -1 },
	{ 911, 919, 929, -1 },
	{ 937, 941, 947, -1 },
	{ 953, 967, 971, -1 },
	{ 977, 983, 991, -1 },

	{ 997, 1009, 1013, -1 },
	{ 1019, 1021, 1031, -1 }
};

bool BigInteger::m_primeProductsInited = false;

int BigInteger::m_primeProducts[BIGINTEGER_PRIMELIST_ROWS];

const int BigInteger::m_chunk2 = 1;
const int BigInteger::m_chunk10 = 19;
const int BigInteger::m_chunk16 = 16;

void BigInteger::Init2()
{
	for (int i = 0; i < BIGINTEGER_PRIMELIST_ROWS; ++i)
	{
		const int * const primeList = m_primeLists[i];
		int product = 1;
		for (int j = 0; j < BIGINTEGER_PRIMELIST_COLS; ++j)
		{
			if (-1 == primeList[j])
			{
				break;
			}
			product *= primeList[j];
		}
		m_primeProducts[i] = product;
	}
}

void BigInteger::ConstructWith(int signNum, RefCountPtr<Array<int32> > mag, bool checkMag)
{
	ConstructCommon();

	m_sign = mag->AreElementsEqualTo(0) ? 0 : signNum;

	if (checkMag)
	{
		int i = 0;
		while (i < mag->Length() && (*mag)[i] == 0)
		{
			++i;
		}

		if (i == 0)
		{
			m_magnitude = mag;
		}
		else
		{
			// strip leading 0 words
			m_magnitude = RefCountPtr<Array<int32> >(new Array<int32>(mag->Length() - i));
			Array<int32>::Copy(*mag, i, *m_magnitude, 0, m_magnitude->Length());
		}
	}
	else
	{
		m_magnitude = mag;
	}

	m_nBitLength = m_sign == 0 ? 0 : CalcBitLength(0, *m_magnitude);
}

void BigInteger::ConstructWith(Array<byte>& bytes, int offset, int length)
{
	ConstructCommon();

	if (length == 0)
	{
		throw new InvalidArgumentException("Zero length BigInteger");
	}

	//// TODO Move this processing into MakeMagnitude (provide sign argument)
	//if ((int8)bytes[offset] < 0)
	//{
	//	m_sign = -1;

	//	int end = offset + length;

	//	int iBval;
	//	// strip leading sign bytes
	//	for (iBval = offset; iBval < end && ((int8)bytes[iBval] == -1); iBval++)
	//	{
	//	}

	//	if (iBval >= end)
	//	{
	//		m_magnitude = RefCountPtr<Array<int32> >(new Array<int32>(1));
	//		m_magnitude->ElementAtRef(0) = 1;
	//	}
	//	else
	//	{
	//		int numBytes = end - iBval;
	//		Array<byte> inverse(numBytes);

	//		int index = 0;
	//		while (index < numBytes)
	//		{
	//			inverse[index++] = (byte)~bytes[iBval++];
	//		}

	//		ASSERT(iBval == end);

	//		while (inverse[--index] == 255)
	//		{
	//			inverse[index] = 0;
	//		}

	//		inverse[index]++;

	//		m_magnitude = MakeMagnitude(inverse, 0, inverse.Length());
	//	}
	//}
	//else
	//{
		// strip leading zero bytes and return magnitude bytes
		m_magnitude = MakeMagnitude(bytes, offset, length);
		m_sign = m_magnitude->Length() > 0 ? 1 : 0;
	//}

	m_sign = m_magnitude->AreElementsEqualTo(0) ? 0 : m_sign;

	m_nBitLength = m_sign == 0 ? 0 : CalcBitLength(0, *m_magnitude);
}

void BigInteger::ConstructWith(int sign, Array<byte>& bytes, int offset, int length)
{
	ConstructCommon();

	if (sign < -1 || sign > 1)
	{
		throw new InvalidArgumentException("Invalid sign value");
	}

	if (sign == 0)
	{
		m_sign = 0;
		m_magnitude = RefCountPtr<Array<int32> >(new Array<int32>());
	}
	else
	{
		// copy bytes
		m_magnitude = MakeMagnitude(bytes, offset, length);
		m_sign = m_magnitude->Length() == 0 ? 0 : sign;
	}

	m_nBitLength = m_sign == 0 ? 0 : CalcBitLength(0, *m_magnitude);
}

BigInteger::BigInteger()
{
	// ZERO
	ConstructWith(0, RefCountPtr<Array<int32> >(new Array<int32>()), false);
}

BigInteger::BigInteger(int signNum, RefCountPtr<Array<int32> >mag, bool checkMag)
{
	ConstructWith(signNum, mag, checkMag);
}

BigInteger::BigInteger(Array<byte>& bytes)
{
	ConstructWith(bytes, 0, bytes.Length());
}

BigInteger::BigInteger(Array<byte>& bytes, int offset, int length)
{
	ConstructWith(bytes, offset, length);
}

BigInteger::BigInteger(int sign, Array<byte>& bytes, int offset, int length)
{
	ConstructWith(sign, bytes, offset, length);
}

BigInteger::BigInteger(int sign, Array<byte>& bytes)
{
	ConstructWith(sign, bytes, 0, bytes.Length());
}

const byte BigInteger::m_rndMask[] = { 255, 127, 63, 31, 15, 7, 3, 1 };


BigInteger::BigInteger(int sizeInBits, Random& random)
{
	ConstructCommon();

	if (sizeInBits < 0)
	{
		throw new InvalidArgumentException("sizeInBits must be non-negative");
	}

	m_nBits = -1;
	m_nBitLength = -1;

	if (sizeInBits == 0)
	{
		m_magnitude = RefCountPtr<Array<int32> >(new Array<int32>());
		return;
	}

	int nBytes = GetByteLength(sizeInBits);
	Array<byte> b(nBytes);
	random.NextBytes(b, nBytes);

	// strip off any excess bits in the MSB
	b[0] &= m_rndMask[BitsPerByte * nBytes - sizeInBits];

	m_magnitude = MakeMagnitude(b, 0, nBytes);
	m_sign = m_magnitude->Length() < 1 ? 0 : 1;	

	m_nBitLength = m_sign == 0 ? 0 : CalcBitLength(0, *m_magnitude);
}

BigInteger::BigInteger(int bitLength, int certainty, Random& random)
{
	ConstructCommon();

	if (bitLength < 2)
	{
		throw new InvalidArgumentException("bitLength < 2");
	}

	m_sign = 1;
	m_nBitLength = bitLength;

	if (bitLength == 2)
	{
		m_magnitude = RefCountPtr<Array<int32> >(new Array<int32>(1));
		m_magnitude->ElementAtRef(0) = random.Next(2) == 0
			?	2
			:	3;
		return;
	}

	int nBytes = GetByteLength(bitLength);
	Array<byte> b(nBytes);

	int xBits = BitsPerByte * nBytes - bitLength;
	byte mask = m_rndMask[xBits];

	for (;;)
	{
		random.NextBytes(b, nBytes);

		// strip off any excess bits in the MSB
		b[0] &= mask;

		// ensure the leading bit is 1 (to meet the strength requirement)
		b[0] |= (byte)(1 << (7 - xBits));

		// ensure the trailing bit is 1 (i.e. must be odd)
		b[nBytes - 1] |= 1;

		m_magnitude = MakeMagnitude(b, 0, b.Length());
		m_nBits = -1;
		m_mQuote = -1L;

		if (certainty < 1)
		{
			break;
		}

		if (CheckProbablePrime(certainty, random))
		{
			break;
		}

		if (bitLength > 32)
		{
			for (int rep = 0; rep < 10000; ++rep)
			{
				int n = 33 + random.Next(bitLength - 2);
				(*m_magnitude)[m_magnitude->Length() - (n >> 5)] ^= (1 << (n & 31));
				(*m_magnitude)[m_magnitude->Length() - 1] ^= ((random.Next() + 1) << 1);
				m_mQuote = -1L;

				if (CheckProbablePrime(certainty, random))
				{
					return;
				}
			}
		}
	}

	m_nBitLength = m_sign == 0 ? 0 : CalcBitLength(0, *m_magnitude);
}

RefCountPtr<BigInteger> BigInteger::CreateValueOf(int64 value)
{
	if (value < 0)
	{
		if (value == Int64::MinValue())
		{
			return CreateValueOf(~value)->Not();
		}

		return CreateValueOf(-value)->Negate();
	}

	return CreateUValueOf((uint64)value);
}

RefCountPtr<BigInteger> BigInteger::CreateUValueOf(uint64 value)
{
	uint32 *valp = reinterpret_cast<uint32 *>(&value);
	int32 msw = (int32)valp[1];
	int32 lsw = (int32)valp[0];

	if (msw != 0)
	{
		Array<int32> *ar = new Array<int32>(2);
		ar->ElementAtRef(0) = msw;
		ar->ElementAtRef(1) = lsw;

		BigInteger *bp = new BigInteger(1, RefCountPtr<Array<int32> >(ar), false);
		return RefCountPtr<BigInteger>(bp);
	}

	if (lsw != 0)
	{
		Array<int32> *ar = new Array<int32>(1);
		ar->ElementAtRef(0) = lsw;

		RefCountPtr<BigInteger> n = RefCountPtr<BigInteger>(new BigInteger(1, RefCountPtr<Array<int32> >(ar), false));
		
		// Check for a power of two
		if ((lsw & -lsw) == lsw)
		{
			n->m_nBits = 1;
		}

		return n;
	}

	// Zero
	return RefCountPtr<BigInteger>(new BigInteger());
}

RefCountPtr<Array<int32> > BigInteger::MakeMagnitude(const Array<byte>& bytes, int offset, int length)
{
	int end = offset + length;
	if (end > bytes.Length())
	{
		end = bytes.Length();
	}

	// strip leading zeros
	int firstSignificant;
	for (firstSignificant = offset; firstSignificant < end && bytes[firstSignificant] == 0; firstSignificant++)
	{
	}

	if (firstSignificant >= end)
	{
		// Zero
		return RefCountPtr<Array<int32> >(new Array<int32>());
	}

	int nInts = (end - firstSignificant + 3) / BytesPerInt;
	int bCount = (end - firstSignificant) % BytesPerInt;
	if (bCount == 0)
	{
		bCount = BytesPerInt;
	}

	if (nInts < 1)
	{
		// Zero
		return RefCountPtr<Array<int32> >(new Array<int32>());
	}

	Array<int32> *mag = new Array<int32>(nInts);

	int v = 0;
	int magnitudeIndex = 0;
	for (int i = firstSignificant; i < end; ++i)
	{
		v <<= 8;
		v |= bytes[i] & 0xff;
		bCount--;
		if (bCount <= 0)
		{
			(*mag)[magnitudeIndex] = v;
			magnitudeIndex++;
			bCount = BytesPerInt;
			v = 0;
		}
	}

	if (magnitudeIndex < mag->Length())
	{
		(*mag)[magnitudeIndex] = v;
	}

	return RefCountPtr<Array<int32> >(mag);
}

bool BigInteger::CheckProbablePrime(int certainty, Random& random) const
{
	ASSERT(certainty > 0);
	ASSERT(Compare(Two()) > 0);
	ASSERT(TestBit(0));

	// Try to reduce the penalty for really small numbers
	int numLists = Math::Min(GetBitLength() - 1, BIGINTEGER_PRIMELIST_ROWS);

	for (int i = 0; i < numLists; ++i)
	{
		int test = Remainder(m_primeProducts[i]);

		const int *primeList = m_primeLists[i];
		for (int j = 0; j < BIGINTEGER_PRIMELIST_COLS; ++j)
		{
			int prime = primeList[j];
			if (-1 == prime)
			{
				break;
			}
			int qRem = test % prime;
			if (qRem == 0)
			{
				// We may find small numbers in the list
				return GetBitLength() < 16 && ToInt() == prime;
			}
		}
	}

	return RabinMillerTest(certainty, random);
}

bool BigInteger::RabinMillerTest(int certainty, Random& random) const
{
	ASSERT(certainty > 0);
	ASSERT(GetBitLength() > 2);
	ASSERT(TestBit(0));

	// let n = 1 + d . 2^s
	BigInteger n = *this;
	RefCountPtr<BigInteger> nMinusOne = n.Subtract(One());
	int s = nMinusOne->GetLowestSetBit();
	RefCountPtr<BigInteger> r = nMinusOne->ShiftRight(s);

	do
	{
		// TODO Make a method for random BigIntegers in range 0 < x < n)
		// - Method can be optimized by only replacing examined bits at each trial
		BigInteger a;
		do
		{
			a = BigInteger(n.GetBitLength(), random);
		}
		while (a.Compare(One()) <= 0 || a.Compare(*nMinusOne) >= 0);

		RefCountPtr<BigInteger> y = a.ModPow(*r, n);

		if (!y->Equals(One()))
		{
			int j = 0;
			while (!y->Equals(*nMinusOne))
			{
				if (j++ == s)
					return false;

				y = y->ModPow(Two(), n);

				if (y->Equals(One()))
					return false;
			}
		}

		certainty -= 2; // composites pass for only 1/4 possible 'a'
	}
	while (certainty > 0);

	return true;
}

int BigInteger::CalcBitLength(int indx, const Array<int32>& mag) const
{
	for (;;)
	{
		if (indx >= mag.Length())
			return 0;

		if (mag.ElementAt(indx) != 0)
			break;

		++indx;
	}

	// bit length for everything after the first int
	int bitLength = 32 * ((mag.Length() - indx) - 1);

	// and determine bitlength of first int
	int firstMag = mag.ElementAt(indx);
	bitLength += BitLen(firstMag);

	// Check for negative powers of two
	if (m_sign < 0 && ((firstMag & -firstMag) == firstMag))
	{
		do
		{
			if (++indx >= mag.Length())
			{
				--bitLength;
				break;
			}
		}
		while (mag.ElementAt(indx) == 0);
	}

	return bitLength;
}

int BigInteger::GetLowestSetBit() const
{
	if (m_sign == 0)
	{
		return -1;
	}

	int w = m_magnitude->Length();

	while (--w > 0)
	{
		if ((*m_magnitude)[w] != 0)
		{
			break;
		}
	}

	int word = (int) (*m_magnitude)[w];
	ASSERT(word != 0);

	int b = (word & 0x0000FFFF) == 0
		?	(word & 0x00FF0000) == 0
			?	7
			:	15
		:	(word & 0x000000FF) == 0
			?	23
			:	31;

	while (b > 0)
	{
		//if ((word << b) == Int32::Min())
		if ((word & (1 << b)) != 0)
		{
			break;
		}

		b--;
	}

	return ((m_magnitude->Length() - w) * 32 - (b + 1));
}

bool BigInteger::TestBit(int n) const
{
	if (n < 0)
	{
		throw new InvalidArgumentException("Bit position must not be negative");
	}

	if (m_sign < 0)
	{
		return !Not()->TestBit(n);
	}

	int wordNum = n / 32;
	if (wordNum >= m_magnitude->Length())
	{
		return false;
	}

	int word = (*m_magnitude)[m_magnitude->Length() - 1 - wordNum];
	return ((word >> (n % 32)) & 1) > 0;
}

RefCountPtr<Array<int32> > BigInteger::LastNBits(int n) const
{
	if (n < 1)
	{
		return Zero().m_magnitude->Clone();
	}

	int numWords = (n + BitsPerInt - 1) / BitsPerInt;
	numWords = Math::Min(numWords, m_magnitude->Length());
	RefCountPtr<Array<int32> > result(new Array<int32>(numWords));

	Array<int32>::Copy(*m_magnitude, m_magnitude->Length() - numWords, *result, 0, numWords);

	int hiBits = n % 32;
	if (hiBits != 0)
	{
		(*result)[0] &= ~(-1 << hiBits);
	}

	return result;
}

RefCountPtr<Array<int32> > BigInteger::DoSubBigLil(Array<int32>& bigMag, Array<int32>& lilMag)
{
	RefCountPtr<Array<int32> > x(bigMag.Clone());
	if (lilMag.Length() > x->Length())
	{
		Subtract(0, lilMag, 0, x);
	}
	else
	{
		Subtract(0, x, 0, lilMag);
	}
	return x;
}

const BigInteger& BigInteger::One()
{
	return *OnePtr();
}

BigIntegerPtr BigInteger::OnePtr()
{
	///BUG: Not thread safe
	if (m_one.IsNull())
	{
		Array<int32> *mag = new Array<int32>(1);
		mag->SetElementAt(0, 1);
		m_one = RefCountPtr<BigInteger>(new BigInteger(1, RefCountPtr<Array<int32> >(mag), false));
	}
	ASSERT(m_one->ToInt() == 1);
	return m_one;
}

const BigInteger& BigInteger::Two()
{
	///BUG: Not thread safe
	if (m_two.IsNull())
	{
		Array<int32> *mag = new Array<int32>(1);
		mag->SetElementAt(0, 2);
		m_two = RefCountPtr<BigInteger>(new BigInteger(1, RefCountPtr<Array<int32> >(mag), false));
	}
	ASSERT(m_two->ToInt() == 2);
	return m_two;
}

const BigInteger& BigInteger::Zero()
{
	return *ZeroPtr();
}

BigIntegerPtr BigInteger::ZeroPtr()
{
	///BUG: Not thread safe
	if (m_zero.IsNull())
	{
		m_zero = RefCountPtr<BigInteger>(new BigInteger());
	}
	ASSERT(m_zero->ToInt() == 0);
	return m_zero;
}

RefCountPtr<BigInteger> BigInteger::ShiftRight(int n) const
{
	if (n == 0)
	{
		return RefCountPtr<BigInteger>(new BigInteger(*this));
	}

	if (n < 0)
	{
		return ShiftLeft(-n);
	}

	if (n >= GetBitLength())
	{
		return (m_sign < 0 ? One().Negate() : ZeroPtr());
	}

	int resultLength = (GetBitLength() - n + 31) >> 5;
	Array<int> *res = new Array<int>(resultLength);

	int numInts = n >> 5;
	int numBits = n & 31;

	if (numBits == 0)
	{
		Array<int32>::Copy(*m_magnitude, 0, *res, 0, res->Length());
	}
	else
	{
		int numBits2 = 32 - numBits;

		int magPos = m_magnitude->Length() - 1 - numInts;
		for (int i = resultLength - 1; i >= 0; --i)
		{
			(*res)[i] = (int)((uint32) (*m_magnitude)[magPos--] >> numBits);

			if (magPos >= 0)
			{
				(*res)[i] |= (*m_magnitude)[magPos] << numBits2;
			}
		}
	}

	ASSERT((*res)[0] != 0);

	return RefCountPtr<BigInteger>(new BigInteger(m_sign, RefCountPtr<Array<int32> >(res), false));
}

RefCountPtr<Array<int32> > BigInteger::ShiftLeft(Array<int32>& mag, int n)
{
	int nInts = (int)((uint32)n >> 5);
	int nBits = n & 0x1f;
	int magLen = mag.Length();
	RefCountPtr<Array<int32> > newMag;

	if (nBits == 0)
	{
		newMag = RefCountPtr<Array<int32> >(new Array<int32>(magLen + nInts));
		mag.CopyTo(*newMag, 0);
	}
	else
	{
		int i = 0;
		int nBits2 = 32 - nBits;
		int highBits = (int)((uint32)mag[0] >> nBits2);

		if (highBits != 0)
		{
			newMag = RefCountPtr<Array<int32> >(new Array<int32>(magLen + nInts + 1));
			(*newMag)[i++] = highBits;
		}
		else
		{
			newMag = RefCountPtr<Array<int32> >(new Array<int32>(magLen + nInts));
		}

		int m = mag[0];
		for (int j = 0; j < magLen - 1; j++)
		{
			int next = mag[j + 1];

			(*newMag)[i++] = (m << nBits) | (int)((uint32)next >> nBits2);
			m = next;
		}

		(*newMag)[i] = mag[magLen - 1] << nBits;
	}

	return newMag;
}

RefCountPtr<BigInteger> BigInteger::ShiftLeft(int n) const
{
	if (m_sign == 0 || m_magnitude->Length() == 0)
	{
		return ZeroPtr();
	}

	if (n == 0)
	{
		return RefCountPtr<BigInteger>(new BigInteger(*this));
	}

	if (n < 0)
	{
		return ShiftRight(-n);
	}

	RefCountPtr<BigInteger> result = RefCountPtr<BigInteger>(new BigInteger(m_sign, ShiftLeft(m_magnitude, n), true));

	if (m_nBits != -1)
	{
		result->m_nBits = m_sign > 0
			?	m_nBits
			:	m_nBits + n;
	}

	if (m_nBitLength != -1)
	{
		result->m_nBitLength = m_nBitLength + n;
	}

	return result;
}

RefCountPtr<BigInteger> BigInteger::Inc() const
{
	if (m_sign < 0)
	{
		return RefCountPtr<BigInteger>(new BigInteger(-1, DoSubBigLil(m_magnitude, One().m_magnitude), true));
	}
	return AddToMagnitude(One().m_magnitude);
}

RefCountPtr<BigInteger> BigInteger::Add(const BigInteger& value) const
{
	if (m_sign == 0)
	{
		return RefCountPtr<BigInteger>(new BigInteger(value));
	}

	if (m_sign != value.m_sign)
	{
		if (value.m_sign == 0)
		{
			return RefCountPtr<BigInteger>(new BigInteger(*this));
		}

		if (value.m_sign < 0)
		{
			return Subtract(*value.Negate());
		}

		return value.Subtract(*Negate());
	}

	return AddToMagnitude(value.m_magnitude);
}

void BigInteger::Subtract
(
	int xStart,
	Array<int32>& x,
	int yStart,
	Array<int32>& y
)
{
	ASSERT(yStart < y.Length());

	if (!(x.Length() - xStart >= y.Length() - yStart))
	{
		ASSERT(x.Length() - xStart >= y.Length() - yStart);
	}

	int32 iT = x.Length();
	int32 iV = y.Length();
	int64 m;
	int32 borrow = 0;

	do
	{
		m = ((x[--iT]) & BIGINTEGER_IMASK) - ((y[--iV]) & BIGINTEGER_IMASK) + borrow;
		x[iT] = (int) m;

//				borrow = (m < 0) ? -1 : 0;
		borrow = (int32)(m >> 63);
	}
	while (iV > yStart);

	if (borrow != 0 && iT > 0)
	{
		while (iT > 0 && --x[--iT] == -1)
		{
		}
	}

	//return x;
}

RefCountPtr<BigInteger> BigInteger::Subtract(const BigInteger& n) const
{
	if (n.m_sign == 0)
		return RefCountPtr<BigInteger>(new BigInteger(*this));

	if (m_sign == 0)
		return n.Negate();

	if (m_sign != n.m_sign)
	{
		RefCountPtr<BigInteger> nNeg = n.Negate();
		return Add(*nNeg);
	}
	
	int compare = CompareNoLeadingZeroes(0, m_magnitude, 0, n.m_magnitude);
	if (compare == 0)
		return ZeroPtr();

	BigInteger bigun, lilun;
	if (compare < 0)
	{
		ASSERT(n > *this);
		bigun = n;
		lilun = *this;
	}
	else
	{
		ASSERT(n < *this);
		bigun = *this;
		lilun = n;
	}

	return RefCountPtr<BigInteger>(new BigInteger(m_sign * compare, DoSubBigLil(bigun.m_magnitude, lilun.m_magnitude), true));
}

void BigInteger::AddMagnitudes(Array<int32>& a, Array<int32>& b)
{
	int tI = a.Length() - 1;
	int vI = b.Length() - 1;
	int64 m = 0;

	while (vI >= 0)
	{
		m += ((int64)(uint32)a[tI] + (int64)(uint32)b[vI--]);
		a[tI--] = (int)m;
		m = (int64)((uint64)m >> 32);
	}

	if (m != 0)
	{
		while (tI >= 0 && ++(a[tI--]) == 0)
		{
		}
	}

	//return a;
}

RefCountPtr<BigInteger> BigInteger::AddToMagnitude(RefCountPtr<Array<int32> > magToAdd) const
{
	RefCountPtr<Array<int32> > big, small;
	if (m_magnitude->Length() < magToAdd->Length())
	{
		big = magToAdd;
		small = m_magnitude;
	}
	else
	{
		big = m_magnitude;
		small = magToAdd;
	}

	// Conservatively avoid over-allocation when no overflow possible
	uint32 limit = UInt32::MaxValue();
	if (big->Length() == small->Length())
	{
		limit -= (uint32)(*small)[0];
	}

	bool possibleOverflow = (uint32)(*big)[0] >= limit;

	RefCountPtr<Array<int32> > bigCopy;
	if (possibleOverflow)
	{
		bigCopy = RefCountPtr<Array<int32> >(new Array<int32>(big->Length() + 1));
		big->CopyTo(*bigCopy, 1);
	}
	else
	{
		bigCopy = big->Clone();
	}

	AddMagnitudes(bigCopy, small);

	return RefCountPtr<BigInteger>(new BigInteger(m_sign, bigCopy, possibleOverflow));
}

static int64 FastExtEuclid(int64 a, int64 b, int64 uOut[2])
{
	int64 u1 = 1;
	int64 u3 = a;
	int64 v1 = 0;
	int64 v3 = b;

	while (v3 > 0)
	{
		int64 q, tn;

		q = u3 / v3;

		tn = u1 - (v1 * q);
		u1 = v1;
		v1 = tn;

		tn = u3 - (v3 * q);
		u3 = v3;
		v3 = tn;
	}

	uOut[0] = u1;
	uOut[1] = (u3 - (u1 * a)) / b;

	return u3;
}

static int64 FastModInverse(int64 v, int64 m)
{
	if (m < 1)
	{
		throw new InvalidArgumentException("Modulus must be positive");
	}

	int64 x[2];
	int64 gcd = FastExtEuclid(v, m, x);

	if (gcd != 1)
	{
		throw new InvalidArgumentException("Numbers not relatively prime.");
	}

	if (x[0] < 0)
	{
		x[0] += m;
	}

	return x[0];
}

int64 BigInteger::GetMQuote()
{
	ASSERT(m_sign > 0);

	if (m_mQuote != -1)
	{
		return m_mQuote; // already calculated
	}

	if (m_magnitude->Length() == 0 || ((*m_magnitude)[m_magnitude->Length() - 1] & 1) == 0)
	{
		return -1; // not for even numbers
	}

	int64 v = (((~(*m_magnitude)[m_magnitude->Length() - 1]) | 1) & 0xffffffffL);

#if defined(_MSC_VER) && _MSC_VER < 1300
	m_mQuote = FastModInverse(v, 0x100000000);
#else
	m_mQuote = FastModInverse(v, 0x100000000LL);
#endif

	return m_mQuote;
}

void BigInteger::ShiftRightInPlace(int start, Array<int32>& mag, int n)
{
	int nInts = (int)((uint32)n >> 5) + start;
	int nBits = n & 0x1f;
	int magEnd = mag.Length() - 1;

	if (nInts != start)
	{
		int delta = (nInts - start);
		int i;

		for (i = magEnd; i >= nInts; i--)
		{
			mag[i] = mag[i - delta];
		}
		for (i = nInts - 1; i >= start; i--)
		{
			mag[i] = 0;
		}
	}

	if (nBits != 0)
	{
		int nBits2 = 32 - nBits;
		int m = mag[magEnd];

		for (int i = magEnd; i > nInts; --i)
		{
			int next = mag[i - 1];

			mag[i] = (int)((uint32)m >> nBits) | (next << nBits2);
			m = next;
		}

		mag[nInts] = (int)((uint32)mag[nInts] >> nBits);
	}
}

void BigInteger::ShiftRightOneInPlace(int start, Array<int32>& mag)
{
	int i = mag.Length();
	int m = mag[i - 1];

	while (--i > start)
	{
		int next = mag[i - 1];
		mag[i] = ((int)((uint32)m >> 1)) | (next << 31);
		m = next;
	}

	mag[start] = (int)((uint32)mag[start] >> 1);
}

void BigInteger::Remainder(RefCountPtr<Array<int32> > x, RefCountPtr<Array<int32> > y) const
{
	int xStart = 0;
	while (xStart < x->Length() && (*x)[xStart] == 0)
	{
		++xStart;
	}

	int yStart = 0;
	while (yStart < y->Length() && (*y)[yStart] == 0)
	{
		++yStart;
	}

	ASSERT(yStart < y->Length());

	int xyCmp = CompareNoLeadingZeroes(xStart, x, yStart, y);

	if (xyCmp > 0)
	{
		int yBitLength = CalcBitLength(yStart, *y);
		int xBitLength = CalcBitLength(xStart, *x);
		int shift = xBitLength - yBitLength;

		RefCountPtr<Array<int32> > c;
		int cStart = 0;
		int cBitLength = yBitLength;
		if (shift > 0)
		{
			c = ShiftLeft(y, shift);
			cBitLength += shift;
			ASSERT((*c)[0] != 0);
		}
		else
		{
			int len = y->Length() - yStart;
			c = RefCountPtr<Array<int32> >(new Array<int32>(len));
			Array<int32>::Copy(*y, yStart, *c, 0, len);
		}

		for (;;)
		{
			if (cBitLength < xBitLength || CompareNoLeadingZeroes(xStart, x, cStart, c) >= 0)
			{
				Subtract(xStart, x, cStart, c);

				while ((*x)[xStart] == 0)
				{
					if (++xStart == x->Length())
						return /*x*/;
				}

				//xBitLength = calcBitLength(xStart, x);
				xBitLength = 32 * (x->Length() - xStart - 1) + BitLen((*x)[xStart]);

				if (xBitLength <= yBitLength)
				{
					if (xBitLength < yBitLength)
					{
						return /*x*/;
					}

					xyCmp = CompareNoLeadingZeroes(xStart, x, yStart, y);

					if (xyCmp <= 0)
					{
						break;
					}
				}
			}

			shift = cBitLength - xBitLength;

			// NB: The case where c[cStart] is 1-bit is harmless
			if (shift == 1)
			{
				uint32 firstC = (uint32)(*c)[cStart] >> 1;
				uint32 firstX = (uint32)(*x)[xStart];
				if (firstC > firstX)
				{
					++shift;
				}
			}

			if (shift < 2)
			{
				ShiftRightOneInPlace(cStart, c);
				--cBitLength;
			}
			else
			{
				ShiftRightInPlace(cStart, c, shift);
				cBitLength -= shift;
			}

			//cStart = c.Length - ((cBitLength + 31) / 32);
			while ((*c)[cStart] == 0)
			{
				++cStart;
			}
		}
	}

	if (xyCmp == 0)
	{
		x->Clear(xStart, x->Length() - xStart);
	}

	return /*x*/;
}

int32 BigInteger::Remainder(int m) const
{
	ASSERT(m > 0);

	int64 acc = 0;
	for (int pos = 0; pos < m_magnitude->Length(); ++pos)
	{
		int64 posVal = (uint32)(*m_magnitude)[pos];
		acc = (acc << 32 | posVal) % m;
	}

	return (int32) acc;
}

RefCountPtr<BigInteger> BigInteger::Remainder(const BigInteger& n) const
{
	if (n.m_sign == 0)
	{
		throw new InvalidArgumentException("Division by zero error");
	}

	if (m_sign == 0)
	{
		return ZeroPtr();
	}

	// For small values, use fast remainder method
	if (n.m_magnitude->Length() == 1)
	{
		int val = (*n.m_magnitude)[0];

		if (val > 0)
		{
			if (val == 1)
			{
				return ZeroPtr();
			}

			// TODO Make this func work on uint, and handle val == 1?
			int rem = Remainder(val);
			RefCountPtr<Array<int32> > ar(new Array<int32>(1));
			ar->SetElementAt(0, rem);
			return rem == 0
				?	ZeroPtr()
				:	RefCountPtr<BigInteger>(new BigInteger(m_sign, ar, false));
		}
	}

	if (CompareNoLeadingZeroes(0, m_magnitude, 0, n.m_magnitude) < 0)
	{
		return RefCountPtr<BigInteger>(new BigInteger(*this));
	}

	RefCountPtr<Array<int32> > result;
	if (n.QuickPow2Check())  // n is power of two
	{
		// TODO Move before small values branch above?
		result = LastNBits(n.Abs()->GetBitLength() - 1);
	}
	else
	{
		result = m_magnitude->Clone();
		Remainder(result, n.m_magnitude);
	}

	return RefCountPtr<BigInteger>(new BigInteger(m_sign, result->Clone(), true));
}

/**
 * return z = x / y - done in place (z value preserved, x contains the
 * remainder)
 */
RefCountPtr<Array<int32> > BigInteger::Divide
(
	RefCountPtr<Array<int32> > 	x,
	RefCountPtr<Array<int32> > 	y
) const
{
	int xStart = 0;
	while (xStart < x->Length() && (*x)[xStart] == 0)
	{
		++xStart;
	}

	int yStart = 0;
	while (yStart < y->Length() && (*y)[yStart] == 0)
	{
		++yStart;
	}

	ASSERT(yStart < y->Length());

	int xyCmp = CompareNoLeadingZeroes(xStart, x, yStart, y);
	RefCountPtr<Array<int32> > count;

	if (xyCmp > 0)
	{
		int yBitLength = CalcBitLength(yStart, *y);
		int xBitLength = CalcBitLength(xStart, *x);
		int shift = xBitLength - yBitLength;

		RefCountPtr<Array<int32> > iCount;
		int iCountStart = 0;

		RefCountPtr<Array<int32> > c;
		int cStart = 0;
		int cBitLength = yBitLength;
		if (shift > 0)
		{
//					iCount = ShiftLeft(One.magnitude, shift);
			iCount = RefCountPtr<Array<int32> >(new Array<int32>((shift >> 5) + 1));
			(*iCount)[0] = 1 << (shift % 32);

			c = ShiftLeft(y, shift);
			cBitLength += shift;
		}
		else
		{
			iCount = RefCountPtr<Array<int32> >(new Array<int32>(1));
			(*iCount)[0] = 1;

			int len = y->Length() - yStart;
			c = RefCountPtr<Array<int32> >(new Array<int32>(len));
			Array<int32>::Copy(*y, yStart, *c, 0, len);
		}

		count = RefCountPtr<Array<int32> >(new Array<int32>(iCount->Length()));

		for (;;)
		{
			if (cBitLength < xBitLength || CompareNoLeadingZeroes(xStart, x, cStart, c) >= 0)
			{
				Subtract(xStart, x, cStart, c);
				AddMagnitudes(count, iCount);

				while ((*x)[xStart] == 0)
				{
					if (++xStart == x->Length())
						return count;
				}

				//xBitLength = calcBitLength(xStart, x);
				xBitLength = 32 * (x->Length() - xStart - 1) + BitLen((*x)[xStart]);

				if (xBitLength <= yBitLength)
				{
					if (xBitLength < yBitLength)
						return count;

					xyCmp = CompareNoLeadingZeroes(xStart, x, yStart, y);

					if (xyCmp <= 0)
						break;
				}
			}

			shift = cBitLength - xBitLength;

			// NB: The case where c[cStart] is 1-bit is harmless
			if (shift == 1)
			{
				uint32 firstC = (uint32) (*c)[cStart] >> 1;
				uint32 firstX = (uint32) (*x)[xStart];
				if (firstC > firstX)
				{
					++shift;
				}
			}

			if (shift < 2)
			{
				ShiftRightOneInPlace(cStart, c);
				--cBitLength;
				ShiftRightOneInPlace(iCountStart, iCount);
			}
			else
			{
				ShiftRightInPlace(cStart, c, shift);
				cBitLength -= shift;
				ASSERT(cBitLength >= 0);
				ShiftRightInPlace(iCountStart, iCount, shift);
			}

			//cStart = c.Length - ((cBitLength + 31) / 32);
			while ((*c)[cStart] == 0)
			{
				++cStart;
			}

			while ((*iCount)[iCountStart] == 0)
			{
				++iCountStart;
			}
		}
	}
	else
	{
		count = RefCountPtr<Array<int32> >(new Array<int32>(1));
		count->SetElementAt(0, 0);
	}

	if (xyCmp == 0)
	{
		AddMagnitudes(count, One().m_magnitude);
		x->Clear(xStart, x->Length() - xStart);
	}

	return count;
}

RefCountPtr<BigInteger> BigInteger::Divide(const BigInteger& val) const
{
	if (val.m_sign == 0)
	{
		throw new InvalidArgumentException("Division by zero error");
	}

	if (m_sign == 0)
	{
		return ZeroPtr();
	}

	if (val.QuickPow2Check()) // val is power of two
	{
		RefCountPtr<BigInteger> result = Abs()->ShiftRight(val.Abs()->GetBitLength() - 1);
		return val.m_sign == m_sign ? result : result->Negate();
	}

	return RefCountPtr<BigInteger>(new BigInteger(m_sign * val.m_sign, Divide(m_magnitude->Clone(), val.m_magnitude), true));
}

Array<BigIntegerPtr> BigInteger::DivideAndRemainder(const BigInteger& val) const
{
	if (val.m_sign == 0)
	{
		throw new InvalidArgumentException("Division by zero error");
	}

	Array<BigIntegerPtr> biggies(2);

	if (m_sign == 0)
	{
		biggies[0] = biggies[1] = ZeroPtr();
	}
	else if (val.QuickPow2Check()) // val is power of two
	{
		int e = val.Abs()->GetBitLength() - 1;
		RefCountPtr<BigInteger> quotient = Abs()->ShiftRight(e);
		RefCountPtr<Array<int32> > remainder = LastNBits(e);

		biggies[0] = (val.m_sign == m_sign ? quotient : quotient->Negate());
		biggies[1] = BigIntegerPtr(new BigInteger(m_sign, remainder, true));
	}
	else
	{
		RefCountPtr<Array<int32> > remainder = m_magnitude->Clone();
		RefCountPtr<Array<int32> > quotient = Divide(remainder, val.m_magnitude);

		biggies[0] = BigIntegerPtr(new BigInteger(m_sign * val.m_sign, quotient, true));
		biggies[1] = BigIntegerPtr(new BigInteger(m_sign, remainder, true));
	}

	return biggies;
}

RefCountPtr<BigInteger> BigInteger::Multiply(const BigInteger& val) const
{
	if (m_sign == 0 || val.m_sign == 0)
		return ZeroPtr();

	if (val.QuickPow2Check()) // val is power of two
	{
		RefCountPtr<BigInteger> result = ShiftLeft(val.Abs()->GetBitLength() - 1);
		return val.m_sign > 0 ? result : result->Negate();
	}

	if (QuickPow2Check()) // this is power of two
	{
		RefCountPtr<BigInteger> result = val.ShiftLeft(Abs()->GetBitLength() - 1);
		return m_sign > 0 ? result : result->Negate();
	}

	int maxBitLength = GetBitLength() + val.GetBitLength();
	int resLength = (maxBitLength + BitsPerInt - 1) / BitsPerInt;

	RefCountPtr<Array<int32> > res(new Array<int32>(resLength));

	if (val == *this)
	{
		Square(res, m_magnitude);
	}
	else
	{
		Multiply(res, m_magnitude, val.m_magnitude);
	}

	return RefCountPtr<BigInteger>(new BigInteger(m_sign * val.m_sign, res, true));
}

void BigInteger::Multiply
(
	Array<int32>&	x,
	Array<int32>&	y,
	Array<int32>&	z
)
{
	int i = z.Length();

	if (i < 1)
	{
		//return x;
		return;
	}

	int xBase = x.Length() - y.Length();

	for (;;)
	{
		int64 a = z[--i] & BIGINTEGER_IMASK;
		int64 val = 0;

		for (int j = y.Length() - 1; j >= 0; j--)
		{
			val += a * (y[j] & BIGINTEGER_IMASK) + (x[xBase + j] & BIGINTEGER_IMASK);

			x[xBase + j] = (int)val;

			val = (int64)((uint64)val >> 32);
		}

		--xBase;

		if (i < 1)
		{
			if (xBase >= 0)
			{
				x[xBase] = (int)val;
			}
			else
			{
				ASSERT(val == 0);
			}
			break;
		}

		x[xBase] = (int)val;
	}

	//return x;
}

uint32 BigInteger::MultiplyMontyNIsOne
(
	uint32	x,
	uint32	y,
	uint32	m,
	uint64	mQuote
)
{
	uint64 um = m;
	uint64 prod1 = (uint64)x * (uint64)y;
	uint64 u = (prod1 * mQuote) & BIGINTEGER_UIMASK;
	uint64 prod2 = u * um;
	uint64 tmp = (prod1 & BIGINTEGER_UIMASK) + (prod2 & BIGINTEGER_UIMASK);
	uint64 carry = (prod1 >> 32) + (prod2 >> 32) + (tmp >> 32);

	if (carry > um)
	{
		carry -= um;
	}

	ASSERT(carry <= UInt32::MaxValue());
	return (uint32)(carry & BIGINTEGER_UIMASK);
}

/**
 * Montgomery multiplication: a = x * y * R^(-1) mod m
 * <br/>
 * Based algorithm 14.36 of Handbook of Applied Cryptography.
 * <br/>
 * <li> m, x, y should have length n </li>
 * <li> a should have length (n + 1) </li>
 * <li> b = 2^32, R = b^n </li>
 * <br/>
 * The result is put in x
 * <br/>
 * NOTE: the indices of x, y, m, a different in HAC and in Java
 */
void BigInteger::MultiplyMonty
(
	Array<int32>&	a,
	Array<int32>&	x,
	Array<int32>&	y,
	Array<int32>&	m,
	int64			mQuote	// mQuote = -m^(-1) mod b
)
{
	if (m.Length() == 1)
	{
		x[0] = (int)MultiplyMontyNIsOne((uint32)x[0], (uint32)y[0], (uint32)m[0], (uint64)mQuote);
		return;
	}

	int n = m.Length();
	int nMinus1 = n - 1;
	int64 y_0 = y[nMinus1] & BIGINTEGER_IMASK;

	// 1. a = 0 (Notation: a = (a_{n} a_{n-1} ... a_{0})_{b} )
	a.Clear(0, n + 1);

	// 2. for i from 0 to (n - 1) do the following:
	for (int i = n; i > 0; i--)
	{
		int64 x_i = x[i - 1] & BIGINTEGER_IMASK;

		int64 u = ((((a[n] & BIGINTEGER_IMASK) + ((x_i * y_0) & BIGINTEGER_IMASK)) & BIGINTEGER_IMASK) * mQuote) & BIGINTEGER_IMASK;

		// 2.2 a = (a + x_i * y + u * m) / b
		int64 prod1 = x_i * y_0;
		int64 prod2 = u * (m[nMinus1] & BIGINTEGER_IMASK);
		int64 tmp = (a[n] & BIGINTEGER_IMASK) + (prod1 & BIGINTEGER_IMASK) + (prod2 & BIGINTEGER_IMASK);
		int64 carry = (int64)((uint64)prod1 >> 32) + (int64)((uint64)prod2 >> 32) + (int64)((uint64)tmp >> 32);
		for (int j = nMinus1; j > 0; j--)
		{
			prod1 = x_i * (y[j - 1] & BIGINTEGER_IMASK);
			prod2 = u * (m[j - 1] & BIGINTEGER_IMASK);
			tmp = (a[j] & BIGINTEGER_IMASK) + (prod1 & BIGINTEGER_IMASK) + (prod2 & BIGINTEGER_IMASK) + (carry & BIGINTEGER_IMASK);
			carry = (int64)((uint64)carry >> 32) + (int64)((uint64)prod1 >> 32) +
				(int64)((uint64)prod2 >> 32) + (int64)((uint64)tmp >> 32);
			a[j + 1] = (int)tmp; // division by b
		}
		carry += (a[0] & BIGINTEGER_IMASK);
		a[1] = (int)carry;
		a[0] = (int)((uint64)carry >> 32); // OJO!!!!!
	}

	// 3. if x >= m the x = x - m
	if (CompareTo(0, a, 0, m) >= 0)
	{
		Subtract(0, a, 0, m);
	}

	// put the result in x
	Array<int32>::Copy(a, 1, x, 0, n);
}

/**
 * return w with w = x * x - w is assumed to have enough space.
 */
RefCountPtr<Array<int32> > BigInteger::Square
(
	RefCountPtr<Array<int32> >	w,
	RefCountPtr<Array<int32> >	x
)
{
	// Note: this method allows w to be only (2 * x.Length - 1) words if result will fit
//			if (w.Length != 2 * x.Length)
//				throw new ArgumentException("no I don't think so...");

	uint64 u1, u2, c;

	int wBase = w->Length() - 1;

	for (int i = x->Length() - 1; i != 0; i--)
	{
		uint64 v = (uint64)(uint32) (*x)[i];

		u1 = v * v;
		u2 = u1 >> 32;
		u1 = (uint32) u1;

		u1 += (uint64)(uint32) (*w)[wBase];

		(*w)[wBase] = (int)(uint32) u1;
		c = u2 + (u1 >> 32);

		for (int j = i - 1; j >= 0; j--)
		{
			--wBase;
			u1 = v * (uint64)(uint32) (*x)[j];
			u2 = u1 >> 31; // multiply by 2!
			u1 = (uint32)(u1 << 1); // multiply by 2!
			u1 += c + (uint64)(uint32) (*w)[wBase];

			(*w)[wBase] = (int)(uint32) u1;
			c = u2 + (u1 >> 32);
		}

		c += (uint64)(uint32) (*w)[--wBase];
		(*w)[wBase] = (int)(uint32) c;

		if (--wBase >= 0)
		{
			(*w)[wBase] = (int)(uint32)(c >> 32);
		}
		else
		{
			ASSERT((uint32)(c >> 32) == 0);
		}
		wBase += i;
	}

	u1 = (uint64)(uint32) (*x)[0];
	u1 = u1 * u1;
	u2 = u1 >> 32;
	u1 = u1 & BIGINTEGER_IMASK;

	u1 += (uint64)(uint32) (*w)[wBase];

	(*w)[wBase] = (int)(uint32) u1;
	if (--wBase >= 0)
	{
		(*w)[wBase] = (int)(uint32)(u2 + (u1 >> 32) + (uint64)(uint32) (*w)[wBase]);
	}
	else
	{
		ASSERT((uint32)(u2 + (u1 >> 32)) == 0);
	}

	return w;
}

/**
 * Calculate the numbers u1, u2, and u3 such that:
 *
 * u1 * a + u2 * b = u3
 *
 * where u3 is the greatest common divider of a and b.
 * a and b using the extended Euclid algorithm (refer p. 323
 * of The Art of Computer Programming vol 2, 2nd ed).
 * This also seems to have the side effect of calculating
 * some form of multiplicative inverse.
 *
 * @param a    First number to calculate gcd for
 * @param b    Second number to calculate gcd for
 * @param u1Out      the return object for the u1 value
 * @param u2Out      the return object for the u2 value
 * @return     The greatest common divisor of a and b
 */
RefCountPtr<BigInteger> BigInteger::ExtEuclid
(
	const BigInteger&	a,
	const BigInteger&	b,
	RefCountPtr<BigInteger> u1Out,
	RefCountPtr<BigInteger> u2Out
)
{
	BigInteger u1 = One();
	BigInteger u3 = a;
	BigInteger v1 = Zero();
	BigInteger v3 = b;

	while (v3.m_sign > 0)
	{
		Array<BigIntegerPtr> q = u3.DivideAndRemainder(v3);

		BigIntegerPtr tmp = v1.Multiply(*q[0]);
		BigIntegerPtr tn = u1.Subtract(*tmp);
		u1 = v1;
		v1 = *tn;

		u3 = v3;
		v3 = *q[1];
	}

	if (u1Out.Get() != NULL)
	{
		u1Out->m_sign = u1.m_sign;
		*u1Out->m_magnitude = *u1.m_magnitude;
	}

	if (u2Out.Get() != NULL)
	{
		RefCountPtr<BigInteger> tmp = u1.Multiply(a);
		tmp = u3.Subtract(*tmp);
		RefCountPtr<BigInteger> res = tmp->Divide(b);
		u2Out->m_sign = res->m_sign;
		*u2Out->m_magnitude = *res->m_magnitude;
	}

	return RefCountPtr<BigInteger>(new BigInteger(u3));
}

RefCountPtr<BigInteger> BigInteger::ModInverse(const BigInteger& m) const
{
	if (m.m_sign < 1)
	{
		throw new InvalidArgumentException("Modulus must be positive");
	}

	RefCountPtr<BigInteger> x(new BigInteger());
	RefCountPtr<BigInteger> nil;
	RefCountPtr<BigInteger> modM = Mod(m);
	RefCountPtr<BigInteger> gcd = ExtEuclid(*modM, m, x, nil);

	if (!gcd->Equals(One()))
	{
		throw new InvalidArgumentException("Numbers not relatively prime.");
	}

	if (x->m_sign < 0)
	{
		x->m_sign = 1;
		x->m_magnitude = DoSubBigLil(m.m_magnitude, x->m_magnitude);
	}

	return x;
}

RefCountPtr<BigInteger> BigInteger::Mod(const BigInteger &m) const
{
	if (m.m_sign < 1)
	{
		throw new InvalidArgumentException("Modulus must be positive");
	}

	RefCountPtr<BigInteger> biggie = Remainder(m);

	return biggie->m_sign >= 0 ? biggie : biggie->Add(m);
}

RefCountPtr<BigInteger> BigInteger::ModPow(const BigInteger& exponent, BigInteger& m)
{
	if (m.m_sign < 1)
	{
		throw new InvalidArgumentException("Modulus must be positive");
	}

	if (m.Equals(One()))
	{
		return ZeroPtr();
	}

	if (exponent.m_sign == 0)
	{
		return OnePtr();
	}

	if (m_sign == 0)
	{
		return ZeroPtr();
	}

	RefCountPtr<Array<int32> > zVal;
	RefCountPtr<Array<int32> > yAccum;
	RefCountPtr<Array<int32> > yVal;

	// Montgomery exponentiation is only possible if the modulus is odd,
	// but AFAIK, this is always the case for crypto algo's
	bool useMonty = (((*m.m_magnitude)[m.m_magnitude->Length() - 1] & 1) == 1);
	int64 mQ = 0;
	if (useMonty)
	{
		mQ = m.GetMQuote();

		// tmp = this * R mod m
		RefCountPtr<BigInteger> tmp = ShiftLeft(32 * m.m_magnitude->Length())->Mod(m);
		zVal = tmp->m_magnitude->Clone();

		useMonty = (zVal->Length() <= m.m_magnitude->Length());

		if (useMonty)
		{
			yAccum = RefCountPtr<Array<int32> >(new Array<int32>(m.m_magnitude->Length() + 1));
			if (zVal->Length() < m.m_magnitude->Length())
			{
				Array<int32> *longZ = new Array<int32>(m.m_magnitude->Length());
				zVal->CopyTo(*longZ, longZ->Length() - zVal->Length());
				zVal = RefCountPtr<Array<int32> >(longZ);
			}
		}
	}

	if (!useMonty)
	{
		if (m_magnitude->Length() <= m.m_magnitude->Length())
		{
			//zAccum = new int[m.magnitude.Length * 2];
			zVal = RefCountPtr<Array<int32> >(new Array<int32>(m.m_magnitude->Length()));
			m_magnitude->CopyTo(*zVal, zVal->Length() - m_magnitude->Length());
		}
		else
		{
			//
			// in normal practice we'll never see this...
			//
			RefCountPtr<BigInteger> tmp = Remainder(m);

			//zAccum = new int[m.magnitude.Length * 2];
			zVal = RefCountPtr<Array<int32> >(new Array<int32>(m.m_magnitude->Length()));
			tmp->m_magnitude->CopyTo(*zVal, zVal->Length() - tmp->m_magnitude->Length());
		}

		yAccum = RefCountPtr<Array<int32> >(new Array<int32>(m.m_magnitude->Length() * 2));
	}

	yVal = RefCountPtr<Array<int32> >(new Array<int32>(m.m_magnitude->Length()));

	//
	// from LSW to MSW
	//
	for (int i = 0; i < exponent.m_magnitude->Length(); i++)
	{
		int v = (*exponent.m_magnitude)[i];
		int bits = 0;

		if (i == 0)
		{
			while (v > 0)
			{
				v <<= 1;
				bits++;
			}

			//
			// first time in initialise y
			//
			zVal->CopyTo(*yVal, 0);

			v <<= 1;
			bits++;
		}

		while (v != 0)
		{
			if (useMonty)
			{
				// Montgomery square algo doesn't exist, and a normal
				// square followed by a Montgomery reduction proved to
				// be almost as heavy as a Montgomery mulitply.
				MultiplyMonty(yAccum, yVal, yVal, m.m_magnitude, mQ);
			}
			else
			{
				Square(yAccum, yVal);
				Remainder(yAccum, m.m_magnitude);
				Array<int32>::Copy(*yAccum, yAccum->Length() - yVal->Length(), *yVal, 0, yVal->Length());
				yAccum->Clear();
			}
			bits++;

			if (v < 0)
			{
				if (useMonty)
				{
					MultiplyMonty(yAccum, yVal, zVal, m.m_magnitude, mQ);
				}
				else
				{
					Multiply(yAccum, yVal, zVal);
					Remainder(yAccum, m.m_magnitude);
					Array<int32>::Copy(*yAccum, yAccum->Length() - yVal->Length(), *yVal, 0, yVal->Length());
					yAccum->Clear();
				}
			}

			v <<= 1;
		}

		while (bits < 32)
		{
			if (useMonty)
			{
				MultiplyMonty(yAccum, yVal, yVal, m.m_magnitude, mQ);
			}
			else
			{
				Square(yAccum, yVal);
				Remainder(yAccum, m.m_magnitude);
				Array<int32>::Copy(*yAccum, yAccum->Length() - yVal->Length(), *yVal, 0, yVal->Length());
				yAccum->Clear();
			}
			bits++;
		}
	}

	if (useMonty)
	{
		// Return y * R^(-1) mod m by doing y * 1 * R^(-1) mod m
		zVal->Clear();
		(*zVal)[zVal->Length() - 1] = 1;
		MultiplyMonty(yAccum, yVal, zVal, m.m_magnitude, mQ);
	}

	RefCountPtr<BigInteger> result(new BigInteger(1, yVal, true));

	return exponent.m_sign > 0
		?	result
		:	result->ModInverse(m);
}

RefCountPtr<BigInteger> BigInteger::And(const BigInteger& value) const
{
	if (m_sign == 0 || value.m_sign == 0)
	{
		return ZeroPtr();
	}

	const BigInteger& one = One();
	
	Array<int32> aMag = m_sign > 0
		? *m_magnitude
		: *Add(one)->m_magnitude;

	Array<int32> bMag = value.m_sign > 0
		? *value.m_magnitude
		: *value.Add(one)->m_magnitude;

	bool resultNeg = m_sign < 0 && value.m_sign < 0;
	int resultLength = Math::Max(aMag.Length(), bMag.Length());
	Array<int32> resultMag(resultLength);

	int aStart = resultMag.Length() - aMag.Length();
	int bStart = resultMag.Length() - bMag.Length();

	for (int i = 0; i < resultMag.Length(); ++i)
	{
		int aWord = i >= aStart ? aMag[i - aStart] : 0;
		int bWord = i >= bStart ? bMag[i - bStart] : 0;

		if (m_sign < 0)
		{
			aWord = ~aWord;
		}

		if (value.m_sign < 0)
		{
			bWord = ~bWord;
		}

		resultMag[i] = aWord & bWord;

		if (resultNeg)
		{
			resultMag[i] = ~resultMag[i];
		}
	}

	RefCountPtr<BigInteger> result(new BigInteger(1, resultMag.Clone(), true));

	// TODO Optimise this case
	if (resultNeg)
	{
		result = result->Not();
	}

	return result;
}

RefCountPtr<BigInteger> BigInteger::Or(const BigInteger& value) const
{
	if (m_sign == 0)
		return RefCountPtr<BigInteger>(new BigInteger(value));

	if (value.m_sign == 0)
		return RefCountPtr<BigInteger>(new BigInteger(*this));

	const BigInteger& one = One();

	Array<int32> aMag = m_sign > 0
		? *m_magnitude
		: *Add(one)->m_magnitude;

	Array<int32> bMag = value.m_sign > 0
		? *value.m_magnitude
		: *value.Add(one)->m_magnitude;

	bool resultNeg = m_sign < 0 || value.m_sign < 0;
	int resultLength = Math::Max(aMag.Length(), bMag.Length());
	Array<int32> resultMag(resultLength);

	int aStart = resultMag.Length() - aMag.Length();
	int bStart = resultMag.Length() - bMag.Length();

	for (int i = 0; i < resultMag.Length(); ++i)
	{
		int aWord = i >= aStart ? aMag[i - aStart] : 0;
		int bWord = i >= bStart ? bMag[i - bStart] : 0;

		if (m_sign < 0)
		{
			aWord = ~aWord;
		}

		if (value.m_sign < 0)
		{
			bWord = ~bWord;
		}

		resultMag[i] = aWord | bWord;

		if (resultNeg)
		{
			resultMag[i] = ~resultMag[i];
		}
	}

	RefCountPtr<BigInteger> result(new BigInteger(1, resultMag.Clone(), true));

	// TODO Optimise this case
	if (resultNeg)
	{
		result = result->Not();
	}

	return result;
}

RefCountPtr<BigInteger> BigInteger::Xor(const BigInteger& value) const
{
	if (m_sign == 0)
		return RefCountPtr<BigInteger>(new BigInteger(value));

	if (value.m_sign == 0)
		return RefCountPtr<BigInteger>(new BigInteger(*this));

	const BigInteger& one = One();
	
	Array<int32> aMag = m_sign > 0
		? *m_magnitude
		: *Add(one)->m_magnitude;

	Array<int32> bMag = value.m_sign > 0
		? *value.m_magnitude
		: *value.Add(one)->m_magnitude;

	// TODO Can just replace with sign != value.sign?
	bool resultNeg = (m_sign < 0 && value.m_sign >= 0) || (m_sign >= 0 && value.m_sign < 0);
	int resultLength = Math::Max(aMag.Length(), bMag.Length());
	Array<int32> resultMag(resultLength);

	int aStart = resultMag.Length() - aMag.Length();
	int bStart = resultMag.Length() - bMag.Length();

	for (int i = 0; i < resultMag.Length(); ++i)
	{
		int aWord = i >= aStart ? aMag[i - aStart] : 0;
		int bWord = i >= bStart ? bMag[i - bStart] : 0;

		if (m_sign < 0)
		{
			aWord = ~aWord;
		}

		if (value.m_sign < 0)
		{
			bWord = ~bWord;
		}

		resultMag[i] = aWord ^ bWord;

		if (resultNeg)
		{
			resultMag[i] = ~resultMag[i];
		}
	}

	RefCountPtr<BigInteger> result(new BigInteger(1, resultMag.Clone(), true));

	// TODO Optimise this case
	if (resultNeg)
	{
		result = result->Not();
	}

	return result;
}

RefCountPtr<BigInteger> BigInteger::SetBit(int n) const
{
	if (n < 0)
	{
		throw new InvalidArgumentException("Bit address less than zero");
	}

	if (TestBit(n))
	{
		return RefCountPtr<BigInteger>(new BigInteger(*this));
	}

	// TODO Handle negative values and zero
	if (m_sign > 0 && n < (GetBitLength() - 1))
	{
		return FlipExistingBit(n);
	}

	RefCountPtr<BigInteger> ret = One().ShiftLeft(n);
	return Or(*ret);
}

RefCountPtr<BigInteger> BigInteger::ClearBit(int n) const
{
	if (n < 0)
	{
		throw new InvalidArgumentException("Bit address less than zero");
	}

	if (!TestBit(n))
	{
		return RefCountPtr<BigInteger>(new BigInteger(*this));
	}

	// TODO Handle negative values
	if (m_sign > 0 && n < (GetBitLength() - 1))
	{
		return FlipExistingBit(n);
	}


	RefCountPtr<BigInteger> ret = One().ShiftLeft(n);
	return AndNot(*ret);
}

RefCountPtr<BigInteger> BigInteger::FlipBit(int n) const
{
	if (n < 0)
	{
		throw new InvalidArgumentException("Bit address less than zero");
	}

	// TODO Handle negative values and zero
	if (m_sign > 0 && n < (GetBitLength() - 1))
		return FlipExistingBit(n);

	RefCountPtr<BigInteger> ret = One().ShiftLeft(n);
	return Xor(*ret);
}

RefCountPtr<BigInteger> BigInteger::FlipExistingBit(int n) const
{
	ASSERT(m_sign > 0);
	ASSERT(n >= 0);
	ASSERT(n < GetBitLength() - 1);

	RefCountPtr<Array<int32> > mag = m_magnitude->Clone();
	(*mag)[mag->Length() - 1 - (n >> 5)] ^= (1 << (n & 31)); // Flip bit
	return RefCountPtr<BigInteger>(new BigInteger(m_sign, mag, false));
}

RefCountPtr<BigInteger> BigInteger::Gcd(const BigInteger& value) const
{
	if (value.m_sign == 0)
	{
		return Abs();
	}

	if (m_sign == 0)
	{
		return value.Abs();
	}

	RefCountPtr<BigInteger> r;
	BigInteger u = *this;
	BigInteger v = value;

	while (v.m_sign != 0)
	{
		r = u.Mod(v);
		u = v;
		v = *r;
	}

	return RefCountPtr<BigInteger>(new BigInteger(u));
}

/**
 * return whether or not a BigInteger is probably prime with a
 * probability of 1 - (1/2)**certainty.
 * <p>From Knuth Vol 2, pg 395.</p>
 */
bool BigInteger::IsProbablePrime(int certainty) const
{
	if (certainty <= 0)
	{
		return true;
	}

	RefCountPtr<BigInteger> n = Abs();

	if (!n->TestBit(0))
		return n->Equals(Two());

	if (n->Equals(One()))
		return false;

	return n->CheckProbablePrime(certainty, m_randomSource);
}

BigInteger::~BigInteger()
{
}

BigInteger::BigInteger(const BigInteger& toCopy)
:	m_sign(toCopy.m_sign),
	m_magnitude(toCopy.m_magnitude->Clone()),
	m_nBits(toCopy.m_nBits),
	m_nBitLength(toCopy.m_nBitLength),
	m_mQuote(toCopy.m_mQuote)
{
}

BigInteger& BigInteger::operator =(const BigInteger& toAssign)
{
	m_sign = toAssign.m_sign;
	if (m_magnitude->Length() == toAssign.m_magnitude->Length())
	{
		Array<int32>::Copy(*toAssign.m_magnitude, 0, *m_magnitude, 0, m_magnitude->Length());
	}
	else
	{
		m_magnitude = toAssign.m_magnitude->Clone();
	}
	m_nBits = toAssign.m_nBits;
	m_nBitLength = toAssign.m_nBitLength;
	m_mQuote = toAssign.m_mQuote;

	return *this;
}

BigInteger& BigInteger::operator =(const int64 i)
{
	*this = *CreateValueOf(i);
	return *this;
}

String BigInteger::ToString() const
{
	return "TODO";
}

namespace spl
{
BigInteger operator~(const BigInteger& bi)
{
	RefCountPtr<Array<int32> > mag = bi.m_magnitude->Clone();
	for (int x = 0; x < mag->Length(); x++)
	{
		(*mag)[x] = ~(*mag)[x];
	}
	return BigInteger(bi.m_sign, mag, false);
}
}
bool BigInteger::Equals( const IComparable *a ) const
{
	return Equals(*a);
}

int BigInteger::Compare( const IComparable *a ) const
{
	return Compare(*a);
}

bool BigInteger::Equals( const IComparable& a ) const
{
	return 0 == Compare(a);
}

int BigInteger::Compare( const IComparable& a ) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return -1;
	}

	const BigInteger& value = (const BigInteger&)a;

	return m_sign < value.m_sign ? -1
		: m_sign > value.m_sign ? 1
		: m_sign == 0 ? 0
		: m_sign * CompareNoLeadingZeroes(0, m_magnitude, 0, value.m_magnitude);
}

bool BigInteger::Equals(int64 i) const
{
	return Equals(*ValueOf(i));
}

int BigInteger::CompareTo
(
	int xIndx,
	Array<int32>& x,
	int yIndx,
	Array<int32>& y
)
{
	while (xIndx != x.Length() && x[xIndx] == 0)
	{
		xIndx++;
	}

	while (yIndx != y.Length() && y[yIndx] == 0)
	{
		yIndx++;
	}

	return CompareNoLeadingZeroes(xIndx, x, yIndx, y);
}

int BigInteger::CompareNoLeadingZeroes
(
	int xIndx,
	const Array<int32>& x,
	int yIndx,
	const Array<int32>& y
)
{
	int diff = (x.Length() - y.Length()) - (xIndx - yIndx);

	if (diff != 0)
	{
		return diff < 0 ? -1 : 1;
	}

	// lengths of magnitudes the same, test the magnitude values

	while (xIndx < x.Length())
	{
		uint32 v1 = (uint32)x[xIndx++];
		uint32 v2 = (uint32)y[yIndx++];

		if (v1 != v2)
		{
			return v1 < v2 ? -1 : 1;
		}
	}

	return 0;
}

int32 BigInteger::MajicNumber() const
{
	return BIGINTEGER_MAJIC;
}

uint32 BigInteger::HashCode() const
{
	uint32 hash = 0;
	int count = m_magnitude->Length();

	for(int i = 0; i < count; i++)
	{
		hash ^= ~m_magnitude->ElementAt(i);
	}

	return hash;
}

void BigInteger::ToByteArray(Array<byte>& bytes, bool isUnsigned) const
{
	if (m_sign == 0)
	{
		if (isUnsigned )
		{
			bytes = Array<byte>(0);
		}
		else
		{
			bytes = Array<byte>(1);
			bytes[0] = 0;
		}
		return;
	}

	int nBits = (isUnsigned && m_sign > 0)
		?	GetBitLength()
		:	GetBitLength() + 1;

	int nBytes = GetByteLength(nBits);
	if (bytes.Length() != nBytes)
	{
		bytes = Array<byte>(nBytes);
	}

	int magIndex = m_magnitude->Length();
	int bytesIndex = bytes.Length();

	if (m_sign > 0)
	{
		while (magIndex > 1)
		{
			uint32 mag = (uint32) (*m_magnitude)[--magIndex];
			bytes[--bytesIndex] = (byte) mag;
			bytes[--bytesIndex] = (byte)(mag >> 8);
			bytes[--bytesIndex] = (byte)(mag >> 16);
			bytes[--bytesIndex] = (byte)(mag >> 24);
		}

		uint32 lastMag = (uint32) (*m_magnitude)[0];
		while (lastMag > 255)
		{
			bytes[--bytesIndex] = (byte) lastMag;
			lastMag >>= 8;
		}

		bytes[--bytesIndex] = (byte) lastMag;
	}
	else // sign < 0
	{
		bool carry = true;

		while (magIndex > 1)
		{
			uint32 mag = ~((uint32)(*m_magnitude)[--magIndex]);

			if (carry)
			{
				carry = (++mag == UInt32::MinValue());
			}

			bytes[--bytesIndex] = (byte) mag;
			bytes[--bytesIndex] = (byte)(mag >> 8);
			bytes[--bytesIndex] = (byte)(mag >> 16);
			bytes[--bytesIndex] = (byte)(mag >> 24);
		}

		uint32 lastMag = (uint32)(*m_magnitude)[0];

		if (carry)
		{
			// Never wraps because magnitude[0] != 0
			--lastMag;
		}

		while (lastMag > 255)
		{
			bytes[--bytesIndex] = (byte) ~lastMag;
			lastMag >>= 8;
		}

		bytes[--bytesIndex] = (byte) ~lastMag;

		if (bytesIndex > 0)
		{
			bytes[--bytesIndex] = 255;
		}
	}
}

#if defined(DEBUG) || defined(_DEBUG)
void BigInteger::CheckMem() const
{
	m_magnitude.CheckMem();
}

void BigInteger::ValidateMem() const
{
	m_magnitude.ValidateMem();
}

void BigInteger::CheckMemStatics()
{
	m_zero.CheckMem();
	m_one.CheckMem();
	m_two.CheckMem();
}

void BigInteger::DebugClearStatics()
{
	m_zero = NULL;
	m_one = NULL;
	m_two = NULL;
}
#endif
