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
#include <spl/math/Des.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifdef DEBUG

static void _TestDes1()
{
	const char *data = "Hi there, this is the text";
	Array<byte> dataa((const byte *)data, (int)strlen(data));
	RefCountPtr<String> enc = DES::EncryptString("thepassword", dataa);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	enc.CheckMem();
	dataa.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DES a");

	RefCountPtr<Array<byte> > result = DES::DecryptString("thepassword", enc->GetChars());
	
	UNIT_ASSERT("DES len", strlen(data) == strlen((const char *)result->Data()));
	UNIT_ASSERT("DES out", strcmp(data, (const char *)result->Data()) == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	enc.CheckMem();
	dataa.CheckMem();
	result.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("DES 1.0");
		
	Log::SWriteOkFail( "DES test 1" );
}

void _TestDes()
{
	_TestDes1();
}

#endif
