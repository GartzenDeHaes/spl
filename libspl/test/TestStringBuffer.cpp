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
#include <spl/Int32.h>
#include <spl/text/StringBuffer.h>

#ifdef DEBUG

#include <spl/io/log/Log.h>

using namespace spl;

static void _TestStrBuf1()
{
	StringBuffer *sb = new StringBuffer();
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(sb);
	sb->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StringBuffer test 1.0");

	sb->Append("hi");
	sb->Append(Int32::ToString(2));
	sb->Append('z');
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(sb);
	sb->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StringBuffer test 1.0");

	UNIT_ASSERT("Contains hi2z", sb->Equals("hi2z"));
	UNIT_ASSERT("len==4", sb->Length() == 4);
	UNIT_ASSERT("sb[2]", (*sb)[2] == '2');
	sb->SetCharAt(2, '3');
	UNIT_ASSERT("Contains hi3z", sb->Equals("hi3z"));
	sb->Insert(2, "there");
	sb->ValidateMem();
	UNIT_ASSERT("Contains hithere3z", sb->Equals("hithere3z"));
	UNIT_ASSERT("not numeric", !sb->IsNumeric());
	UNIT_ASSERT("indexof he", sb->IndexOf("he", 0) == 3);
	UNIT_ASSERT("indexof HE", sb->IndexOf("HE", 0) == -1);
	UNIT_ASSERT("indexofIgnoreCase", sb->IndexOfIgnoreCase("HE", 0) == 3);
	sb->Append("     ");
	UNIT_ASSERT("check append 5 spaces", sb->Equals("hithere3z     "));
	sb->Insert(0, " ");
	UNIT_ASSERT("appended 6 spaces", sb->Length() == 15);
	UNIT_ASSERT(sb->GetChars(), sb->Equals(" hithere3z     "));
	sb->Trim();
	UNIT_ASSERT("trim", sb->Length() == 9); 
	UNIT_ASSERT("trim cont", sb->Equals("hithere3z"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(sb);
	sb->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StringBuffer test 1.2");

	delete sb;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StringBuffer test 1.3");
	Log::SWriteOkFail( "StringBuffer test 1" );
}

static void _TestStrBuf2()
{
	StringBuffer sb;
	sb.Append("1230");
	UNIT_ASSERT("1234 IsNumeric", sb.IsNumeric());
	sb.SetLength(0);
	sb.Append("22.2");
	UNIT_ASSERT("22.2 IsNumeric", sb.IsNumeric());
	Log::SWriteOkFail( "StringBuffer test 2" );
}

static void _TestStrBuf3()
{
	StringBuffer sb;
	sb.Append("1");
	sb.Fill('x', 2);
	UNIT_ASSERT("1xx", sb.Equals("1xx"));
	Log::SWriteOkFail( "StringBuffer Fill test" );
}


static void _TestStringBufferRemoveChar()
{
	StringBuffer *str = new StringBuffer("bob");
	str->RemoveCharAt(0);
	UNIT_ASSERT( "_TestStringBufferRemoveChar 1", strcmp(str->GetChars(), "ob") == 0 );
	UNIT_ASSERT( "_TestStringBufferRemoveChar 2", str->Length() == 2 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferRemoveChar 1.0");

	str->RemoveCharAt(1);
	UNIT_ASSERT( "_TestTStringRemoveChar 3", strcmp(str->GetChars(), "o") == 0 );
	UNIT_ASSERT( "_TestTStringRemoveChar 4", str->Length() == 1 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferRemoveChar 1.1");

	str->RemoveCharAt(0);
	UNIT_ASSERT( "_TestStringBufferRemoveChar 5", strcmp(str->GetChars(), "") == 0 );
	UNIT_ASSERT( "_TestStringBufferRemoveChar 6", str->Length() == 0 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferRemoveChar 1.2");

	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferRemoveChar 1.3");

	Log::SWriteOkFail( "StringBuffer test RemoveChar" );
}

static void _TestStringBufferInsertChar()
{
	StringBuffer *str = new StringBuffer("abc");
	str->InsertCharAt('1', 0);
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringBufferInsertChar 1", strcmp(str->GetChars(), "1abc") == 0 );
	UNIT_ASSERT( "_TestStringBufferInsertChar 2", str->Length() == 4 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferInsertChar 1.0");
	
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferInsertChar 1.1");

	str = new StringBuffer("abc");
	str->InsertCharAt('1', 3);
	str->ValidateMem();
	str->InsertCharAt('2', 4);
	str->ValidateMem();
	str->InsertCharAt('3', 5);
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringBufferInsertChar 1", strcmp(str->GetChars(), "abc123") == 0 );
	UNIT_ASSERT( "_TestStringBufferInsertChar 2", str->Length() == 6 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferInsertChar 1.2");

	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestStringBufferInsertChar 1.3");
	
	Log::SWriteOkFail( "StringBuffer test InsertChar" );
}

void TestStringBuffer()
{
	_TestStrBuf1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestStrBuf2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestStrBuf3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestStringBufferRemoveChar();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestStringBufferInsertChar();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
