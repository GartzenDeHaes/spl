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
#include <spl/Debug.h>
#include <spl/io/log/Log.h>
#include <spl/BigInteger.h>
#include <spl/math/Math.h>
#include <spl/UInt32.h>

using namespace spl;

#ifdef DEBUG

static void _TestBigIntegerIdentities()
{
	{
		UNIT_ASSERT("0==0", BigInteger::Zero().Equals((int64)0));
		UNIT_ASSERT("1==1", BigInteger::One().Equals((int64)1));
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 0");

	Log::SWriteOkFail( "BigInteger identities test" );
}

static void _TestBigIntegerAddSub()
{
	{
		RefCountPtr<BigInteger> a = BigInteger::ValueOf(1);
		RefCountPtr<BigInteger> b = BigInteger::ValueOf(1);

		BigInteger result = *a + *b;
		UNIT_ASSERT("1+1", result == 2);
		UNIT_ASSERT("1==1", *a == *b);
		UNIT_ASSERT("1==1", a->Compare(*b) == 0);
		UNIT_ASSERT("! 1!=1", !(*a != *b));
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 1");

	{
		RefCountPtr<BigInteger> a = BigInteger::ValueOf(3);
		RefCountPtr<BigInteger> b = BigInteger::ValueOf(1);

		BigInteger result = *a - *b;
		UNIT_ASSERT("3-1", result == 2);
		UNIT_ASSERT("3!=1", *a != *b);
		UNIT_ASSERT("! 3==1", !(*a == *b));
		UNIT_ASSERT("3.Compare(1) > 0", a->Compare(*b) > 0);
		UNIT_ASSERT("1.Compare(3) < 0", b->Compare(*a) < 0);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 2");

	{
		RefCountPtr<BigInteger> a = BigInteger::ValueOf(3);
		RefCountPtr<BigInteger> b = BigInteger::ValueOf(2);

		BigInteger result = *a * *b;
		UNIT_ASSERT("3*2", result == 6);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 3");

	{
		RefCountPtr<BigInteger> a = BigInteger::ValueOf(6);
		RefCountPtr<BigInteger> b = BigInteger::ValueOf(2);

		BigInteger result = *a / *b;
		UNIT_ASSERT("6/2", result == 3);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 4");

	{
		RefCountPtr<BigInteger> a = BigInteger::ValueOf(6);
		RefCountPtr<BigInteger> b = BigInteger::ValueOf(7);

		BigInteger result = *a % *b;
		UNIT_ASSERT("6%2", result == 6);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 5");

	{
		RefCountPtr<BigInteger> a = BigInteger::ValueOf(6);

		BigInteger result = *a >> 1;
		UNIT_ASSERT("6>>1", result == 3);
		result = result << 2;
		UNIT_ASSERT("3<<2", result == 12);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 6");

	{
		RefCountPtr<BigInteger> a = BigInteger::ValueOf(1);
		RefCountPtr<BigInteger> b = BigInteger::ValueOf(2);

		BigInteger result = *a | *b;
		UNIT_ASSERT("1|2", result == 3);

		result = *a & *b;
		UNIT_ASSERT("1&2", result == 0);

		result = *a ^ *b;
		UNIT_ASSERT("1^2", result == 3);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 7");

	Log::SWriteOkFail( "BigInteger small number bin ops test" );
}

static void _TestBigIntegerToByteArray()
{
	{
#if defined(_MSC_VER) && _MSC_VER < 1300
		int64 num = 0xFFFFFFFFF;
#else
		int64 num = 0xFFFFFFFFFLL;
#endif
		RefCountPtr<BigInteger> i = BigInteger::ValueOf(num);
		Array<byte> bytes = i->ToByteArrayUnsigned();
		BigInteger i2(bytes);

		UNIT_ASSERT("i==i2", *i == i2);
		UNIT_ASSERT("i2==0xFFFFFFFFFL", i2 == num);

		bytes = i->ToByteArray();
		i2 = BigInteger(bytes);

		UNIT_ASSERT("i==i2", *i == i2);
		UNIT_ASSERT("i2==0xFFFFFFFFFL", i2 == num);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 0");

	Log::SWriteOkFail( "BigInteger ToByteArray() test" );
}

static void _TestBigIntegerModPow()
{
	{
		int x = 6;
		BigInteger bx = *BigInteger::ValueOf(x);
		UNIT_ASSERT("6", bx.ToInt() == 6);

		int dp = 2;
		BigInteger bdp = *BigInteger::ValueOf(dp);

		int p = 3;
		BigInteger bp = *BigInteger::ValueOf(p);

		int iResult = ((int)(Math::Pow((double)x, (double)dp)) % p);
		BigInteger bResult = *bx.ModPow(bdp, bp);
		UNIT_ASSERT("(x ^ dp) % p", bResult == iResult);

		iResult = ((int)(Math::Pow((double)p, (double)dp)) % x);
		bResult = *bp.ModPow(bdp, bx);
		UNIT_ASSERT("(p ^ dp) % x", bResult == iResult);

		int iFive = 5;
		BigInteger bFive = *BigInteger::ValueOf(iFive);

		iResult = (int)Math::Remainder((double)x, (double)iFive);
		bResult = *bx.Remainder(bFive);
		UNIT_ASSERT("x % p", bResult == iResult);

		bResult = *bx.Mod(bFive);
		UNIT_ASSERT("x % p", bResult == iResult);
	}
	{
		int64 m = UInt32::MaxValue();
		int64 e = 18;
		int64 v = 5;

		BigIntegerPtr bm = BigInteger::ValueOf(m);
		BigIntegerPtr be = BigInteger::ValueOf(e);
		BigIntegerPtr bv = BigInteger::ValueOf(v);

		double dRes = Math::Pow((double)v, (double)e);
		int64 iRes = (int64)dRes;
		iRes = iRes % m;
		BigInteger bRes = *bv->ModPow(*be, *bm);

		UNIT_ASSERT("no", *bm == m);
		UNIT_ASSERT("no", *be == e);
		UNIT_ASSERT("no", *bv == v);
		UNIT_ASSERT("large int", bRes == iRes);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 0");

	Log::SWriteOkFail( "BigInteger ModPow ((x ^ dP) % p) test" );
}

static void _TestBigIntegerDivideRemain()
{
	int numer = 65535;
	int demon = -1919574108;

	{
		BigInteger bnumer = *BigInteger::ValueOf(numer);
		BigInteger bdemon = *BigInteger::ValueOf(demon);

		//UNIT_ASSERT("65535 / -1919574108",  (bnumer / bdemon) == numer / bdemon);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 0");

	Log::SWriteOkFail( "BigInteger DivideAndRemainder test" );
}

static void _TestBigInteger32Bits()
{
	int64 i = UInt32::MaxValue();
	int64 j = 65527;

	{
		BigInteger bi = *BigInteger::ValueOf(i);
		BigInteger bj = *BigInteger::ValueOf(j);

		UNIT_ASSERT("UInt32::MaxValue()", bi == i);
		UNIT_ASSERT("UInt32::MaxValue()", bj == j);
		UNIT_ASSERT("UInt32::MaxValue() / 65527",  (bi / bj) == i / j);
		UNIT_ASSERT("UInt32::MaxValue()", bi == i);
		UNIT_ASSERT("UInt32::MaxValue()", bj == j);

		int64 ires = j / i;
		BigInteger bres = bj / bi;
		UNIT_ASSERT("UInt32::MaxValue()", bi == i);
		UNIT_ASSERT("UInt32::MaxValue()", bj == j);

		int bresi = bres.ToInt();
		UNIT_ASSERT("65527 / UInt32::MaxValue()",  (bres) == ires);

		UNIT_ASSERT("UInt32::MaxValue() % 65527",  (bi % bj) == i % j);
		UNIT_ASSERT("65527 % UInt32::MaxValue()",  (bj % bi) == j % i);

		i--;

		bi = *BigInteger::ValueOf(i);

		UNIT_ASSERT("UInt32::MaxValue() - 1", bi == i);
		UNIT_ASSERT("UInt32::MaxValue() - 1 / 65527",  (bi / bj) == i / j);
		UNIT_ASSERT("65527 / UInt32::MaxValue() - 1",  (bj / bi) == j / i);

		UNIT_ASSERT("UInt32::MaxValue() - 1 % 65527",  (bi % bj) == i % j);
		UNIT_ASSERT("65527 % UInt32::MaxValue() - 1",  (bj % bi) == j % i);
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("BigInteger test 0");

	Log::SWriteOkFail( "BigInteger 32 bit boundry test" );
}

void _TestBigInteger()
{
	_TestBigIntegerIdentities();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestBigIntegerAddSub();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestBigIntegerToByteArray();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestBigIntegerModPow();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestBigIntegerDivideRemain();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestBigInteger32Bits();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
