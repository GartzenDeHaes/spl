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
#include <spl/math/RSA.h>

#ifdef DEBUG

using namespace spl;

static void _TestRSA1()
{
	RSA rsa(32);

	const byte *txt = (const byte *)"No is the time";
	Array<byte> pt(txt, (int)strlen((const char *)txt));
	rsa.PadInputBuffer(pt);

	RefCountPtr<Array<byte> > enc = rsa.EncryptBinary(pt);
	RefCountPtr<Array<byte> > dec = rsa.DecryptBinary(*enc);
	UNIT_ASSERT("bin comp", pt.Equals(*dec));
	String str((const char *)dec->Data(), dec->Length());
	UNIT_ASSERT("str", str.Equals((const char *)txt));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	rsa.CheckMem();
	pt.CheckMem();
	enc.CheckMem();
	dec.CheckMem();
	str.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RSA 1.0");
		
	Log::SWriteOkFail( "RSA test" );
}

static void _TestRSA384Bits()
{
	RSA rsa(128, 1);

	const byte *txt = (const byte *)"Now is the time for all good monkeys to come to the aid of their party.";
	Array<byte> pt(txt, (int)strlen((const char *)txt));
	rsa.PadInputBuffer(pt);

	RefCountPtr<Array<byte> > enc = rsa.EncryptBinary(pt);
	RefCountPtr<Array<byte> > dec = rsa.DecryptBinary(*enc);
	UNIT_ASSERT("bin comp", pt.Equals(*dec));
	String str((const char *)dec->Data(), dec->Length());
	UNIT_ASSERT("str", str.Equals((const char *)txt));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	rsa.CheckMem();
	pt.CheckMem();
	enc.CheckMem();
	dec.CheckMem();
	str.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RSA 1.0");
		
	Log::SWriteOkFail( "RSA 128 bit test" );
}

static void _TestRSAFizz()
{
	const byte *txt = (const byte *)"Now is the time$~ ";
	Array<byte> pt(txt, (int)strlen((const char *)txt));

	for( int x = 0; x < 32; x++)
	{
		RSA rsa(32);

		RefCountPtr<Array<byte> > enc = rsa.EncryptBinary(pt);
		RefCountPtr<Array<byte> > dec = rsa.DecryptBinary(*enc);
		UNIT_ASSERT("bin comp", pt.Equals(*dec));
		String str((const char *)dec->Data(), dec->Length());
		UNIT_ASSERT("str", str.Equals((const char *)txt));
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	pt.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RSA 1.0");
		
	Log::SWriteOkFail( "RSA fizzing" );
}

void _TestRSA()
{
	_TestRSA1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRSA384Bits();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRSAFizz();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
