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
#include <spl/interp/JsLex.h>

using namespace spl;

#ifdef DEBUG

static void _TestJsLexSingles()
{
	JsLex lex("; ! % ^ & * ( ) - + = < > , . / : ~ | { } [ ]");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_SEMI );
	UNIT_ASSERT( "Lex !", lex.Next() == JsLex::T_BANG );
	UNIT_ASSERT( "Lex %", lex.Next() == JsLex::T_MOD );
	UNIT_ASSERT( "Lex ^", lex.Next() == JsLex::T_XOR );
	UNIT_ASSERT( "Lex &", lex.Next() == JsLex::T_AMPR );
	UNIT_ASSERT( "Lex *", lex.Next() == JsLex::T_STAR );
	UNIT_ASSERT( "Lex (", lex.Next() == JsLex::T_LPAR );
	UNIT_ASSERT( "Lex )", lex.Next() == JsLex::T_RPAR );
	UNIT_ASSERT( "Lex -", lex.Next() == JsLex::T_MINUS );
	UNIT_ASSERT( "Lex +", lex.Next() == JsLex::T_PLUS );
	UNIT_ASSERT( "Lex ==", lex.Next() == JsLex::T_ASSIGN );
	UNIT_ASSERT( "Lex <", lex.Next() == JsLex::T_LT );
	UNIT_ASSERT( "Lex >", lex.Next() == JsLex::T_GT );
	UNIT_ASSERT( "Lex ,", lex.Next() == JsLex::T_COMMA );
	UNIT_ASSERT( "Lex .", lex.Next() == JsLex::T_DOT );
	UNIT_ASSERT( "Lex /", lex.Next() == JsLex::T_SLASH );
	UNIT_ASSERT( "Lex :", lex.Next() == JsLex::T_COLON );
	UNIT_ASSERT( "Lex ~", lex.Next() == JsLex::T_COMP );
	UNIT_ASSERT( "Lex |", lex.Next() == JsLex::T_PIPE );
	UNIT_ASSERT( "Lex [", lex.Next() == JsLex::T_LBRACE );
	UNIT_ASSERT( "Lex ]", lex.Next() == JsLex::T_RBRACE );
	UNIT_ASSERT( "Lex {", lex.Next() == JsLex::T_LBRAC );
	UNIT_ASSERT( "Lex }", lex.Next() == JsLex::T_RBRAC );
	UNIT_ASSERT( "Lex EOF", lex.Next() == JsLex::T_JEOF );
	
	lex.Init(";!%^&*()-+<>,./:~|{}[]");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_SEMI );
	UNIT_ASSERT( "Lex !", lex.Next() == JsLex::T_BANG );
	UNIT_ASSERT( "Lex %", lex.Next() == JsLex::T_MOD );
	UNIT_ASSERT( "Lex ^", lex.Next() == JsLex::T_XOR );
	UNIT_ASSERT( "Lex &", lex.Next() == JsLex::T_AMPR );
	UNIT_ASSERT( "Lex *", lex.Next() == JsLex::T_STAR );
	UNIT_ASSERT( "Lex (", lex.Next() == JsLex::T_LPAR );
	UNIT_ASSERT( "Lex )", lex.Next() == JsLex::T_RPAR );
	UNIT_ASSERT( "Lex -", lex.Next() == JsLex::T_MINUS );
	UNIT_ASSERT( "Lex +", lex.Next() == JsLex::T_PLUS );
	UNIT_ASSERT( "Lex <", lex.Next() == JsLex::T_LT );
	UNIT_ASSERT( "Lex >", lex.Next() == JsLex::T_GT );
	UNIT_ASSERT( "Lex ,", lex.Next() == JsLex::T_COMMA );
	UNIT_ASSERT( "Lex .", lex.Next() == JsLex::T_DOT );
	UNIT_ASSERT( "Lex /", lex.Next() == JsLex::T_SLASH );
	UNIT_ASSERT( "Lex :", lex.Next() == JsLex::T_COLON );
	UNIT_ASSERT( "Lex ~", lex.Next() == JsLex::T_COMP );
	UNIT_ASSERT( "Lex |", lex.Next() == JsLex::T_PIPE );
	UNIT_ASSERT( "Lex [", lex.Next() == JsLex::T_LBRACE );
	UNIT_ASSERT( "Lex ]", lex.Next() == JsLex::T_RBRACE );
	UNIT_ASSERT( "Lex {", lex.Next() == JsLex::T_LBRAC );
	UNIT_ASSERT( "Lex }", lex.Next() == JsLex::T_RBRAC );
	UNIT_ASSERT( "Lex EOF", lex.Next() == JsLex::T_JEOF );
	
	Log::SWriteOkFail( "JS lex singles" );
}

static void _TestJsLexSemi()
{
	JsLex lex(";");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_SEMI );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	lex.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestJsLexSemi 1");
	
	Log::SWriteOkFail( "JS lex ;" );
}

static void _TestJsLexKeywords(  )
{
	JsLex lex("finally try catch null const new delete switch case if else while for var return public private protected break do continue");
	UNIT_ASSERT( "Lex finally", lex.Next() == JsLex::T_FINALLY );
	UNIT_ASSERT( "Lex try", lex.Next() == JsLex::T_TRY );
	UNIT_ASSERT( "Lex catch", lex.Next() == JsLex::T_CATCH );
	UNIT_ASSERT( "Lex null", lex.Next() == JsLex::T_JNULL );
	UNIT_ASSERT( "Lex const", lex.Next() == JsLex::T_CONST );
	UNIT_ASSERT( "Lex new", lex.Next() == JsLex::T_NEW );
	UNIT_ASSERT( "Lex delete", lex.Next() == JsLex::T_DELETE );
	UNIT_ASSERT( "Lex switch", lex.Next() == JsLex::T_SWITCH );
	UNIT_ASSERT( "Lex case", lex.Next() == JsLex::T_CASE );
	UNIT_ASSERT( "Lex if", lex.Next() == JsLex::T_IF );
	UNIT_ASSERT( "Lex else", lex.Next() == JsLex::T_ELSE );
	UNIT_ASSERT( "Lex while", lex.Next() == JsLex::T_WHILE );
	UNIT_ASSERT( "Lex for", lex.Next() == JsLex::T_FOR );
	UNIT_ASSERT( "Lex var", lex.Next() == JsLex::T_VAR );
	UNIT_ASSERT( "Lex return", lex.Next() == JsLex::T_RETURN );
	UNIT_ASSERT( "Lex public", lex.Next() == JsLex::T_PUBLIC );
	UNIT_ASSERT( "Lex private", lex.Next() == JsLex::T_PRIVATE );
	UNIT_ASSERT( "Lex protected", lex.Next() == JsLex::T_PROTECTED );
	UNIT_ASSERT( "Lex break", lex.Next() == JsLex::T_BREAK );
	UNIT_ASSERT( "Lex do", lex.Next() == JsLex::T_DO );
	UNIT_ASSERT( "Lex continue", lex.Next() == JsLex::T_CONTINUE );
	UNIT_ASSERT( "Lex EOF", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex keywords" );
}

static void _TestJsLexTwoChar(  )
{
	JsLex lex ( "~= != %= ^= &= *= /= -- ++ <= >=" );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_COMPEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_ISNEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_MODEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_XOREQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_ANDEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_TIMESEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_DIVEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_DEC );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_INC );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_LTEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_GTEQ );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex two char op test ... " );
}

static void _TestJsLexStringNullTest(  )
{
	JsLex lex ("\"\"");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_STRING );
	UNIT_ASSERT( "Lex ;", lex.Lexum().Length() == 0 );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex string null test ... " );
}

static void _TestJsLexStringTest()
{
	JsLex lex ("\"hi mom\"");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_STRING );
	UNIT_ASSERT( "Lex ;", lex.Lexum().Equals("hi mom") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex string test ... " );
}

static void _TestJsLexCharTest1()
{
	JsLex lex ("\n'\\n'");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_STRING );
	UNIT_ASSERT( "lex", lex.Lexum().CharAt(0) == '\n' );
	UNIT_ASSERT( "lex line", lex.LineNumber() == 2 );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex char test ... " );
}

static void _TestJsLexCharTest2(  )
{
	JsLex lex ("\n'\\\\'");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_STRING );
	UNIT_ASSERT( "lex", lex.Lexum().CharAt(0) == '\\' );
	UNIT_ASSERT( "lex line", lex.LineNumber() == 2 );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex char 2 test ... " );
}

static void _TestJsLexIdTest(  )
{
	JsLex lex ("_mybob my_bob mybob_ my1bob _123");
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("_mybob") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("my_bob") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("mybob_") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("my1bob") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("_123") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex ID test ... " );
}

static void _TestJsLexNumTest(  )
{
	JsLex lex( "1 1.0 .1 0x10 1.2e5 1.2e-5" );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_INT );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("1") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_REAL );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("1.0") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_REAL );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals(".1") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_HEX );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("0x10") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_REAL );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("1.2e5") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_REAL );
	UNIT_ASSERT( "lex id", lex.Lexum().Equals("1.2e-5") );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex num test ... " );
}

static void _TestJsLexSmallProgTest(  )
{
	JsLex lex ("class test {\n\tpublic method()\n{var x = 1; return x;}\n \t}\n");
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_FUNCTION );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_LBRACE );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_PUBLIC );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_LPAR );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_RPAR );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_LBRACE );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_VAR );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_ASSIGN );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_INT );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_SEMI );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_RETURN );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_ID );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_SEMI );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_RBRACE );
	UNIT_ASSERT( "lex prog 1", lex.Next() == JsLex::T_RBRACE );
	UNIT_ASSERT( "Lex ;", lex.Next() == JsLex::T_JEOF );

	Log::SWriteOkFail( "JS lex small prog test ... " );
}

void _TestJsLex()
{
	_TestJsLexSemi();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexSingles();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexKeywords();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexTwoChar();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexStringNullTest();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexStringTest();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexCharTest1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexCharTest2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexIdTest();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexNumTest();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsLexSmallProgTest();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
