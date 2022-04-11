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

#if defined(DEBUG)

#include <stdio.h>
#include <stdlib.h>
#include <spl/io/log/Log.h>
#include <spl/math/Math.h>
#include <spl/String.h>

using namespace spl;

static void _TestTStringNull()
{
	DEBUG_FREE_HEAP();

	String *str = new String("bob");
	UNIT_ASSERT( "String 1 content", strcmp(str->GetChars(), "bob") == 0 );
	UNIT_ASSERT( "String 1 len", str->Length() == 3 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringNull 1.0");

	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestStringNull 1.1");

	_debugValidateHeap();

	Log::SWriteOkFail( "String test null" );
}

static void _TestStringTrim()
{
	String *str = new String("abc");
	StringPtr ret = str->Trim();
	UNIT_ASSERT( "_TestStringTrim 1", strcmp(ret->GetChars(), "abc") == 0 );
	UNIT_ASSERT( "_TestStringTrim 2", ret->Length() == 3 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 1");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 2");

	str = new String(" abc");
	ret = str->Trim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringTrim 3", strcmp(ret->GetChars(), "abc") == 0 );
	UNIT_ASSERT( "_TestStringTrim 4", ret->Length() == 3 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 3");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 4");

	str = new String("abc ");
	ret = str->Trim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringTrim 5", strcmp(ret->GetChars(), "abc") == 0 );
	UNIT_ASSERT( "_TestStringTrim 6", ret->Length() == 3 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 5");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 6");

	str = new String(" abc ");
	ret = str->Trim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringTrim 7", strcmp(ret->GetChars(), "abc") == 0 );
	UNIT_ASSERT( "_TestStringTrim 8", ret->Length() == 3 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 7");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 8");

	str = new String("  ab c  ");
	ret = str->Trim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringTrim 9", strcmp(ret->GetChars(), "ab c") == 0 );
	UNIT_ASSERT( "_TestStringTrim 10", ret->Length() == 4 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 9");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 10");

	Log::SWriteOkFail( "String test Trim" );
}

static void _TestStringRTrim()
{
	String *str = new String("abc");
	StringPtr ret = str->RTrim();
	UNIT_ASSERT( "_TestStringRTrim 1", strcmp(ret->GetChars(), "abc") == 0 );
	UNIT_ASSERT( "_TestStringRTrim 2", ret->Length() == 3 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 1");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringTrim 2");

	str = new String(" abc");
	ret = str->RTrim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringRTrim 3", strcmp(ret->GetChars(), " abc") == 0 );
	UNIT_ASSERT( "_TestStringRTrim 4", ret->Length() == 4 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 3");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 4");

	str = new String("abc ");
	ret = str->RTrim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringRTrim 5", strcmp(ret->GetChars(), "abc") == 0 );
	UNIT_ASSERT( "_TestStringRTrim 6", ret->Length() == 3 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 5");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 6");

	str = new String(" abc ");
	ret = str->RTrim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringRTrim 7", strcmp(ret->GetChars(), " abc") == 0 );
	UNIT_ASSERT( "_TestStringRTrim 8", ret->Length() == 4 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 7");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 8");

	str = new String("  ab c  ");
	ret = str->RTrim();
	str->ValidateMem();
	UNIT_ASSERT( "_TestStringRTrim 9", strcmp(ret->GetChars(), "  ab c") == 0 );
	UNIT_ASSERT( "_TestStringRTrim 10", ret->Length() == 6 );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( str );
	str->CheckMem();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 9");
	delete str;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	ret.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringRTrim 10");

	Log::SWriteOkFail( "String test Trim" );
}

static void _TestStringRTrimRegression1()
{
	String base("42.4200000");
	StringPtr trimmed = base.RTrim('0');
	UNIT_ASSERT("Should be 42.42", *trimmed == "42.42");

	Log::SWriteOkFail( "String test RTrim regression 1" );
}

static void _TestStringSet()
{
	String *a = new String("a");
	a->Set("b");
	UNIT_ASSERT( "_TestStringSet 1.3", a->Equals("b") );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(a);
	a->CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringSet 1.4");

	delete a;
	Log::SWriteOkFail( "String test Set" );
}

static void _TestStringCopyConstruct()
{
	String a("a");
	UNIT_ASSERT( "_TestStringCopyConstruct 1.0", a.Equals("a") );
	UNIT_ASSERT( "_TestStringCopyConstruct 1.1", a == a );
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	a.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringCopyConstruct 1.2");

	String b("b");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	a.CheckMem();
	b.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringCopyConstruct 1.3");
	UNIT_ASSERT( "_TestStringCopyConstruct 1,2", b.Equals("b") );
	UNIT_ASSERT( "_TestStringCopyConstruct 1,2a", b != a );
	UNIT_ASSERT( "_TestStringCopyConstruct 1,2a", a != b );

	b = a;
	UNIT_ASSERT( "_TestStringCopyConstruct 1.3", a.Equals("a") );
	UNIT_ASSERT( "_TestStringCopyConstruct 1,4", b.Equals("a") );
	UNIT_ASSERT( "_TestStringCopyConstruct 1,5", b == a );
	UNIT_ASSERT( "_TestStringCopyConstruct 1,5", b == a );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	a.CheckMem();
	b.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestStringCopyConstruct 1.4");

	Log::SWriteOkFail( "String test copy constructor 1" );
}

void _TestStringFormat()
{
	const char *ptr = "Hello world!";
	char *np = 0;
	int i = 5;
	unsigned int bs = sizeof(int)*8;
	int mi;

	mi = (1 << (bs-1)) + 1;

	StringPtr str = String::Format("printf test\n");
	UNIT_ASSERT("printf test\\n", str->Equals("printf test\n"));
	str = String::Format("%s\n", ptr);
	UNIT_ASSERT("Hello world!\\n", str->Equals("Hello world!\n"));
	str = String::Format("%s is null pointer\n", np);
	UNIT_ASSERT("%s is null pointer\\n", str->Equals("(null) is null pointer\n"));
	str = String::Format("%d = 5\n", i);
	UNIT_ASSERT("%d = 5\\n", str->Equals("5 = 5\n"));
	str = String::Format("%d = - max int\n", mi);
	UNIT_ASSERT("%d = - max int\\n", str->Equals("-2147483647 = - max int\n"));
	str = String::Format("char %c = 'a'\n", 'a');
	UNIT_ASSERT("char %c = 'a'\\n", str->Equals("char a = 'a'\n"));
	str = String::Format("hex %x = ff\n", 0xff);
	UNIT_ASSERT("hex %x = ff\\n", str->Equals("hex ff = ff\n"));
	str = String::Format("hex %02x = 00\n", 0);
	UNIT_ASSERT("hex %02x = 00\\n", str->Equals("hex 00 = 00\n"));
	str = String::Format("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	UNIT_ASSERT("signed %d = unsigned %u = hex %x\\n", str->Equals("signed -3 = unsigned 4294967293 = hex fffffffd\n"));
	str = String::Format("%d %s(s)%", 0, "message");
	UNIT_ASSERT("%d %s(s)%", str->Equals("0 message(s)"));
	str = String::Format("\n");
	UNIT_ASSERT("\\n", str->Equals("\n"));
	str = String::Format("%d %s(s) with %%\n", 0, "message");
	UNIT_ASSERT("%d %s(s) with %%\\n", str->Equals("0 message(s) with %\n"));
	str = String::Format("justif: \"%-10s\"\n", "left"); 
	UNIT_ASSERT("justif: \"%-10s\"\\n", str->Equals("justif: \"left      \"\n"));
	str = String::Format("justif: \"%10s\"\n", "right"); 
	UNIT_ASSERT("justif: \"%10s\"\\n", str->Equals("justif: \"     right\"\n"));
	str = String::Format(" 3: %04d zero padded\n", 3); 
	UNIT_ASSERT(" 3: %04d zero padded\\n", str->Equals(" 3: 0003 zero padded\n"));
	str = String::Format(" 3: %-4d left justif.\n", 3); 
	UNIT_ASSERT(" 3: %-4d left justif.\\n", str->Equals(" 3: 3    left justif.\n"));
	str = String::Format(" 3: %4d right justif.\n", 3); 
	UNIT_ASSERT(" 3: %4d right justif.\\n", str->Equals(" 3:    3 right justif.\n"));
	str = String::Format("-3: %04d zero padded\n", -3); 
	UNIT_ASSERT("-3: %04d zero padded\\n", str->Equals("-3: -003 zero padded\n"));
	str = String::Format("-3: %-4d left justif.\n", -3); 
	UNIT_ASSERT("-3: %-4d left justif.\\n", str->Equals("-3: -3   left justif.\n"));
	str = String::Format("-3: %4d right justif.\n", -3); 
	UNIT_ASSERT("-3: %4d right justif.\\n", str->Equals("-3:   -3 right justif.\n"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Format 1.4");

	Log::SWriteOkFail( "String::Format" );
}

void _TestStringFormat2()
{
	char buf[256];
	sprintf(buf, "%f", Math::PI());

	StringPtr str = String::Format("%f", Math::PI());
	UNIT_ASSERT("%f test", str->Equals(buf));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Format 2.1");

	Log::SWriteOkFail( "String::Format" );
}

void _TestStringInter()
{
	{
		String s;
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		UNIT_ASSERT_MEM_NOTED("String::Inter 1.0");
	}

	StringPtr str = String::Intern("The time has come");
	UNIT_ASSERT("The time has come", str->Equals("The time has come"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Format 1.1");

	Log::SWriteOkFail( "String::Intern"  );
}

void _TestStringReplace()
{
	String str("abcabcabc");
	StringPtr repl = str.Replace('a', 'z');
	UNIT_ASSERT("Replace 1", repl->Equals("zbczbczbc"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	repl.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Replace 2");

	repl = str.Replace('c', 'z');
	UNIT_ASSERT("Replace 1.1", repl->Equals("abzabzabz"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	repl.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Replace 2.1");

	repl = str.Replace("a", "z");
	UNIT_ASSERT("Replace 3", repl->Equals("zbczbczbc"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	repl.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Replace 4");

	repl = str.Replace("c", "z");
	UNIT_ASSERT("Replace 5", repl->Equals("abzabzabz"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	repl.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Replace 6");

	repl = str.Replace("ab", "z");
	UNIT_ASSERT("Replace 7", repl->Equals("zczczc"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	repl.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Replace 8");

	repl = str.Replace("ab", "z23");
	UNIT_ASSERT("Replace 9", repl->Equals("z23cz23cz23c"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	repl.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Replace 10");

	repl = str.Replace("bc", "qpr");
	UNIT_ASSERT("Replace 11", repl->Equals("aqpraqpraqpr"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	repl.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Replace 12");

	Log::SWriteOkFail( "String::Replace" );
}

static void _TestStringLeft()
{
	String str("abcabcabc");
	StringPtr left = str.Left(3);
	UNIT_ASSERT("Left 1", left->Equals("abc"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	left.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Left::Left 2");

	left = str.Left(1);
	UNIT_ASSERT("Left 3", left->Equals("a"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	left.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Left::Left 4");

	left = str.Left(0);
	UNIT_ASSERT("Left 5", left->Equals(""));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	left.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Left::Left 6");

	left = left->Left(1);
	UNIT_ASSERT("Left 7", left->Equals(""));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	left.CheckMem();
	UNIT_ASSERT_MEM_NOTED("Left::Left 8");

	Log::SWriteOkFail( "String::Left" );
}

static void _TestStringRight()
{
	String str("abcabcabc1");
	StringPtr right = str.Right(3);
	UNIT_ASSERT("Right 1", right->Equals("bc1"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	right.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Right 2");
	
	right = str.Right(1);
	UNIT_ASSERT("Right 3", right->Equals("1"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	right.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Right 4");

	right = str.Right(0);
	UNIT_ASSERT("Right 5", right->Equals(""));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	right.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Right 6");

	right = right->Right(1);
	UNIT_ASSERT("Right 7", right->Equals(""));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	right.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String::Right 8");

	Log::SWriteOkFail( "String::Right"  );
}

static void _TestStringCompOps()
{
	String left("abc");
	String right("xyz");
	UNIT_ASSERT(">", left < right);
	UNIT_ASSERT("<", !(left > right));
	UNIT_ASSERT("==", !(left == right));
	UNIT_ASSERT("!=", (left != right));
	UNIT_ASSERT("<=", (left <= right));
	UNIT_ASSERT(">=", !(left >= right));
	UNIT_ASSERT("Comapre", left.Compare(right) < 0);
	UNIT_ASSERT("Comapre", right.Compare(left) > 0);
	UNIT_ASSERT("Equals", !left.Equals(right));
	UNIT_ASSERT("Equals", !right.Equals(left));
	UNIT_ASSERT("CompareNoCase", left.CompareNoCase("ABC") == 0);
	UNIT_ASSERT("EqualsIgnoreCase", left.EqualsIgnoreCase("ABC"));

	left = "xyz";
	UNIT_ASSERT(">", !(left < right));
	UNIT_ASSERT("<", !(left > right));
	UNIT_ASSERT("== 2", (left == right));
	UNIT_ASSERT("!= ", !(left != right));
	UNIT_ASSERT("<=", (left <= right));
	UNIT_ASSERT(">=", (left >= right));
	UNIT_ASSERT("Comapre", left.Compare(right) == 0);
	UNIT_ASSERT("Comapre", right.Compare(left) == 0);
	UNIT_ASSERT("Equals" ,left.Equals(right));
	UNIT_ASSERT("Equals", right.Equals(left));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	left.CheckMem();
	right.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String comparision mem");

	Log::SWriteOkFail( "String comparison operations" );
}

static void _TestStringMid()
{
	String str("abc");
	StringPtr mid = str.Mid(0, 1);

	UNIT_ASSERT("Mid", *mid == "a");
	mid = str.Mid(1, 2);
	UNIT_ASSERT("Mid", *mid == "b");
	mid = str.Mid(2, 3);
	UNIT_ASSERT("Mid", *mid == "c");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	mid.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String comparision mem");

	Log::SWriteOkFail( "String::Mid" );
}

static void _TestStringUpper()
{
	String str("abc");
	StringPtr upper = str.ToUpper();

	UNIT_ASSERT("Upper", *upper == "ABC");
	upper = upper->ToLower();
	UNIT_ASSERT("Lower", *upper == "abc");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	str.CheckMem();
	upper.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String upper mem");

	Log::SWriteOkFail( "String::Upper" );
}

static void _TestStringCat()
{
	String a("ABC");
	String b("xyz");

	UNIT_ASSERT("+", (a + b) == "ABCxyz");
	UNIT_ASSERT("+", ("ABC" + b) == "ABCxyz");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	a.CheckMem();
	b.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String +");

	Log::SWriteOkFail( "String::+" );
}

static void _TestStringBase64()
{
	const char* txt = "Now is the time";
	RefCountPtr<String> enc = String::Base64Encode(txt, (int)strlen(txt));
	RefCountPtr<Array<byte> > dec = String::Base64Decode(*enc);
	String decStr((const char *)dec->Data(), dec->Length());

	UNIT_ASSERT("Base64 text", decStr == txt);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	enc.CheckMem();
	dec.CheckMem();
	decStr.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String Base64");

	Log::SWriteOkFail( "String::Base64" );
}

static void _TestLastIndexOf()
{
	String a("ABCB");

	UNIT_ASSERT("A", a.LastIndexOf("A") == 0);
	UNIT_ASSERT("B", a.LastIndexOf("B") == 3);
	UNIT_ASSERT("B", a.LastIndexOf("B", 1) == 1);
	UNIT_ASSERT("C", a.LastIndexOf("C") == 2);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	a.CheckMem();
	UNIT_ASSERT_MEM_NOTED("String LastIndexOf");

	Log::SWriteOkFail( "String::LastIndexOf" );
}

void TestTString(  )
{
	_TestTStringNull();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString A");
	DEBUG_FREE_HEAP();

	_TestStringTrim();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString D");
	DEBUG_FREE_HEAP();

	_TestStringRTrim();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestString RTrim");
	DEBUG_FREE_HEAP();

	_TestStringSet();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString E");
	DEBUG_FREE_HEAP();

	_TestStringCopyConstruct();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString F");
	DEBUG_FREE_HEAP();
	
	_TestStringFormat();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString G");
	DEBUG_FREE_HEAP();

	_TestStringFormat2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString H");
	DEBUG_FREE_HEAP();

	_TestStringInter();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString I");
	DEBUG_FREE_HEAP();

	_TestStringReplace();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString J");
	DEBUG_FREE_HEAP();

	_TestStringLeft();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString K");
	DEBUG_FREE_HEAP();

	_TestStringRight();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString L");
	DEBUG_FREE_HEAP();

	_TestStringCompOps();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString M");
	DEBUG_FREE_HEAP();

	_TestStringMid();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString N");
	DEBUG_FREE_HEAP();

	_TestStringUpper();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString O");
	DEBUG_FREE_HEAP();

	_TestStringCat();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString P");
	DEBUG_FREE_HEAP();

	_TestStringRTrimRegression1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString Q");
	DEBUG_FREE_HEAP();

	_TestStringBase64();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestTString 64");
	DEBUG_FREE_HEAP();
	
	_TestLastIndexOf();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("TestString LastIndexOf");
	DEBUG_FREE_HEAP();
}

#endif
