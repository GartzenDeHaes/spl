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
#include <spl/xml/xpath/private/XPathLex.h>

#ifdef DEBUG

using namespace spl;

const char* const _theNumericTestInput[] =
{
    "9 + 7 - 6 * 42 + 36 div 3 + 100 - 2 * 6",
    "9 + 7 - 6 + 3 + 24 - 32 - 89 + 123 - 12256 + 89957",
    "0 + 0",
    "1 div 2",
    "1 * 0.25",
    "3 * 3 * 3",
    "9 div 3 div 3",
    "15.95 - 13.56",
    0
};

const double _theNumericTestExpectedOutput[] =
{
    -136.0,
    77740.0,
    0,
    0.5,
    0.25,
    27,
    1,
    15.95 - 13.56,
    0   // This is a dummy entry and should be last.
};

const char* const   _theBooleanTestInput[] =
{
    "0 < 2 < 3",
    "0 < 2 > 3",
    "1 < 2 < 3",

    "0 div 0 != 0 div 0",
    "0 div 0 = 0 div 0",
    "1 < 0 div 0",
    "1 <= 0 div 0",
    "2 > 0 div 0",
    "2 >= 0 div 0",

    "1 < 2",
    "1500 < 657",
    "1465787676 < 5 div 0",
    "4657 div 0 < 4563745658",

    "1000 <= 256000",
    "1000 <= 1000",
    "200564 <= 1999",
    "5768594 <= 56 div 0",
    "564783 div 0 <= 6758494857",

    "2015 > 100",
    "56478 > 240000",
    "4657 div 0 > 57683946",
    "573068574 > 5 div 0",

    "46000 >= 1500",
    "56983 >= 56983",
    "460983 >= 1500000",
    "67594876 div 0 >= 576849",

    "1465787676 >= 5 div 0",
    "-1465787676 >= -5 div 0",
    "-1679 < -87 div 0",

    "1 = 2 = 0",
    "1 = 2 != 0",

    0
};

const bool  _theBooleanTestExpectedOutput[] =
{
    true,
    false,
    true,

    true,
    false,
    false,
    false,
    false,
    false,

    true,
    false,
    true,
    false,

    true,
    true,
    false,
    true,
    false,

    true,
    false,
    true,
    false,

    true,
    true,
    false,
    true,

    false,
    true,
    false,

    true,
    false,

    0
};

const char* const   _theStringTestInput[] =
{
    "string(0)",
    "string(1.5)",
    "string(-1.5)",
    "string(0.5)",
    "string(-0.5)",
    "string(9)",
    "string(-9)",
    "string(\"9\")",
    "string(0.25)",

    "concat(\"foo\", \"bar\")",
    "concat(\"foo\", \"bar\", \"sky\")",

    "contains(\"foobar\", \"oba\")",
    "contains(\"LotusXSL4C is great\", \"boag\")",

    "starts-with(\"foobar\", \"foo\")",
    "starts-with(\"LotusXSL4C is great\", \"boag\")",

    "substring-after(\"1999/04/01\", \"/\")",
    "substring-after(\"1999/04/01\", \"19\")",

    "substring-before(\"1999/04/01\", \"/\")",
    "substring-before(\"1999/04/01\", \"01\")",

    "substring(\"12345\", 1.5, 2.6)",
    "substring(\"12345\", 0, 3)",
    "substring(\"12345\", 5, 29)",
    "substring(\"12345\", -1, 2)",
    "substring(\"12345\", -2, -1)",
    "substring(\"12345\", -2)",

    "normalize-space(\"   aa   a  \")",

    "translate(\"---aaa--\", \"abc-\", \"ABC\")",

    "5 = 0",
    "5 = 5",
    0
};

const char* const   _theStringTestExpectedOutput[] =
{
    "0",
    "1.5",
    "-1.5",
    "0.5",
    "-0.5",
    "9",
    "-9",
    "9",
    "0.25",

    "foobar",
    "foobarsky",

    "true",
    "false",

    "true",
    "false",

    "04/01",
    "99/04/01",

    "1999",
    "1999/04/",

    "234",
    "12",
    "5",
    "",
    "",
    "12345",

    "aa a",

    "AAA",

    "false",
    "true",
    0
};

static void _TestXPathLexNumeric()
{
	XPathLex lex;

	// "9 + 7 - 6 * 42 + 36 div 3 + 100 - 2 * 6",
	lex.Tokenize(_theNumericTestInput[0]);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	lex.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathLexNumeric");

	UNIT_ASSERT("9", lex.CurrentLexum() == "9");
	UNIT_ASSERT("9", lex.CurrentToken() == XPathLex::XP_INT);
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("+", lex.Match(XPathLex::XP_INT) == XPathLex::XP_PLUS);
	UNIT_ASSERT("+", lex.CurrentLexum() == "+");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("7", lex.Match(XPathLex::XP_PLUS) == XPathLex::XP_INT);
	UNIT_ASSERT("7", lex.CurrentLexum() == "7");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("-", lex.Match(XPathLex::XP_INT) == XPathLex::XP_MIN);
	UNIT_ASSERT("-", lex.CurrentLexum() == "-");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("6", lex.Match(XPathLex::XP_MIN) == XPathLex::XP_INT);
	UNIT_ASSERT("6", lex.CurrentLexum() == "6");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("*", lex.Match(XPathLex::XP_INT) == XPathLex::XP_STAR);
	UNIT_ASSERT("*", lex.CurrentLexum() == "*");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("42", lex.Match(XPathLex::XP_STAR) == XPathLex::XP_INT);
	UNIT_ASSERT("42", lex.CurrentLexum() == "42");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("+", lex.Match(XPathLex::XP_INT) == XPathLex::XP_PLUS);
	UNIT_ASSERT("+", lex.CurrentLexum() == "+");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("36", lex.Match(XPathLex::XP_PLUS) == XPathLex::XP_INT);
	UNIT_ASSERT("36", lex.CurrentLexum() == "36");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("div", lex.Match(XPathLex::XP_INT) == XPathLex::XP_LITERAL);
	UNIT_ASSERT("div", lex.CurrentLexum() == "div");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("3", lex.Match(XPathLex::XP_LITERAL) == XPathLex::XP_INT);
	UNIT_ASSERT("3", lex.CurrentLexum() == "3");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("+", lex.Match(XPathLex::XP_INT) == XPathLex::XP_PLUS);
	UNIT_ASSERT("+", lex.CurrentLexum() == "+");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("100", lex.Match(XPathLex::XP_PLUS) == XPathLex::XP_INT);
	UNIT_ASSERT("100", lex.CurrentLexum() == "100");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("-", lex.Match(XPathLex::XP_INT) == XPathLex::XP_MIN);
	UNIT_ASSERT("-", lex.CurrentLexum() == "-");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("2", lex.Match(XPathLex::XP_MIN) == XPathLex::XP_INT);
	UNIT_ASSERT("2", lex.CurrentLexum() == "2");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("*", lex.Match(XPathLex::XP_INT) == XPathLex::XP_STAR);
	UNIT_ASSERT("*", lex.CurrentLexum() == "*");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("6", lex.Match(XPathLex::XP_STAR) == XPathLex::XP_INT);
	UNIT_ASSERT("6", lex.CurrentLexum() == "6");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("EOF", lex.Match(XPathLex::XP_INT) == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF", lex.CurrentToken() == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF LEX", lex.CurrentLexum() == "");
	UNIT_ASSERT("HasMoreTokens", !lex.HasMoreTokens());

	Log::SWriteOkFail( "XPathLex numerics 1" );
}

static void _TestXPathLexNumeric2()
{
	XPathLex lex;

	// "15.95 - 13.56",
	lex.Tokenize(_theNumericTestInput[7]);
	UNIT_ASSERT("15.95", lex.CurrentLexum() == "15.95");
	UNIT_ASSERT("15.95", lex.CurrentToken() == XPathLex::XP_FLOAT);
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("-", lex.Match(XPathLex::XP_FLOAT) == XPathLex::XP_MIN);
	UNIT_ASSERT("-", lex.CurrentLexum() == "-");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("13.56", lex.Match(XPathLex::XP_MIN) == XPathLex::XP_FLOAT);
	UNIT_ASSERT("13.56", lex.CurrentLexum() == "13.56");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("EOF", lex.Match(XPathLex::XP_FLOAT) == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF", lex.CurrentToken() == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF LEX", lex.CurrentLexum() == "");
	UNIT_ASSERT("HasMoreTokens", !lex.HasMoreTokens());

	Log::SWriteOkFail( "XPathLex numerics 2" );
}

static void _TestXPathLexOperators()
{
	XPathLex lex;

	lex.Tokenize("-12 < .12 <= -1.0 == 1 != 1 > 2 >= 3");

	UNIT_ASSERT("-12", lex.CurrentLexum() == "-12");
	UNIT_ASSERT("15.95", lex.CurrentToken() == XPathLex::XP_INT);
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("<", lex.Match(XPathLex::XP_INT) == XPathLex::XP_LT);
	UNIT_ASSERT("<", lex.CurrentLexum() == "<");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT(".12", lex.Match(XPathLex::XP_LT) == XPathLex::XP_FLOAT);
	UNIT_ASSERT(".12", lex.CurrentLexum() == ".12");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("<=", lex.Match(XPathLex::XP_FLOAT) == XPathLex::XP_LTEQ);
	UNIT_ASSERT("<=", lex.CurrentLexum() == "<=");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("-1.0", lex.Match(XPathLex::XP_LTEQ) == XPathLex::XP_FLOAT);
	UNIT_ASSERT("-1.0", lex.CurrentLexum() == "-1.0");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("==", lex.Match(XPathLex::XP_FLOAT) == XPathLex::XP_EQEQ);
	UNIT_ASSERT("==", lex.CurrentLexum() == "==");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("1", lex.Match(XPathLex::XP_EQEQ) == XPathLex::XP_INT);
	UNIT_ASSERT("1", lex.CurrentLexum() == "1");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("!=", lex.Match(XPathLex::XP_INT) == XPathLex::XP_NEQ);
	UNIT_ASSERT("!=", lex.CurrentLexum() == "!=");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("1", lex.Match(XPathLex::XP_NEQ) == XPathLex::XP_INT);
	UNIT_ASSERT("1", lex.CurrentLexum() == "1");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT(">", lex.Match(XPathLex::XP_INT) == XPathLex::XP_GT);
	UNIT_ASSERT(">", lex.CurrentLexum() == ">");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("2", lex.Match(XPathLex::XP_GT) == XPathLex::XP_INT);
	UNIT_ASSERT("2", lex.CurrentLexum() == "2");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT(">=", lex.Match(XPathLex::XP_INT) == XPathLex::XP_GTEQ);
	UNIT_ASSERT(">=", lex.CurrentLexum() == ">=");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("3", lex.Match(XPathLex::XP_GTEQ) == XPathLex::XP_INT);
	UNIT_ASSERT("3", lex.CurrentLexum() == "3");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("EOF", lex.Match(XPathLex::XP_INT) == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF", lex.CurrentToken() == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF LEX", lex.CurrentLexum() == "");
	UNIT_ASSERT("HasMoreTokens", !lex.HasMoreTokens());

	Log::SWriteOkFail( "XPathLex operators" );
}

static void _TestXPathLexExpr()
{
	XPathLex lex;

	lex.Tokenize("//title[@lang='eng']");

	UNIT_ASSERT("//", lex.CurrentLexum() == "//");
	UNIT_ASSERT("15.95", lex.CurrentToken() == XPathLex::XP_SLASHSLASH);
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("title", lex.Match(XPathLex::XP_SLASHSLASH) == XPathLex::XP_LITERAL);
	UNIT_ASSERT("title", lex.CurrentLexum() == "title");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("[", lex.Match(XPathLex::XP_LITERAL) == XPathLex::XP_LBRAC);
	UNIT_ASSERT("[", lex.CurrentLexum() == "[");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("@", lex.Match(XPathLex::XP_LBRAC) == XPathLex::XP_AT);
	UNIT_ASSERT("@", lex.CurrentLexum() == "@");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("lang", lex.Match(XPathLex::XP_AT) == XPathLex::XP_LITERAL);
	UNIT_ASSERT("lang", lex.CurrentLexum() == "lang");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("=", lex.Match(XPathLex::XP_LITERAL) == XPathLex::XP_EQ);
	UNIT_ASSERT("=", lex.CurrentLexum() == "=");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("eng", lex.Match(XPathLex::XP_EQ) == XPathLex::XP_STRING);
	UNIT_ASSERT("eng", lex.CurrentLexum() == "eng");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("]", lex.Match(XPathLex::XP_STRING) == XPathLex::XP_RBRAC);
	UNIT_ASSERT("]", lex.CurrentLexum() == "]");
	UNIT_ASSERT("HasMoreTokens", lex.HasMoreTokens());

	UNIT_ASSERT("EOF", lex.Match(XPathLex::XP_RBRAC) == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF", lex.CurrentToken() == XPathLex::XP_EOF);
	UNIT_ASSERT("EOF LEX", lex.CurrentLexum() == "");
	UNIT_ASSERT("HasMoreTokens", !lex.HasMoreTokens());

	Log::SWriteOkFail( "XPathLex expressions" );
}

void _TestXPathLex()
{
	_TestXPathLexNumeric();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestXPathLexNumeric2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestXPathLexOperators();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestXPathLexExpr();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif



