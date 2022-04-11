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
#include <spl/xml/xpath/private/XPathParser.h>

#include <spl/Debug.h>

#if defined(DEBUG) || defined(_DEBUG)

#include <stdio.h>
#include <stdlib.h>
#include <spl/io/log/Log.h>
#include <spl/xml/xpath/private/XPathParser.h>

using namespace spl;

static void _TestXPathParse1()
{
	XPathParser parser;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestPath1 1.0");

	Array<XPathOperatorPtr> prog = parser.Parse("/");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	prog.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestPath1 1.1");

	prog = parser.Parse("/node1/node2");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	prog.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestPath1 1.2");

	prog = parser.Parse("node2");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	prog.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestPath1 1.3");

	prog = parser.Parse("../node2");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	prog.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestPath1 1.4");

	prog = parser.Parse("//");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	prog.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestPath1 1.5");

	prog = parser.Parse("//nodex/node2");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	prog.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestPath1 1.6");

	Log::SWriteOkFail( "XPath path parse" );
}

static void _TestXPathParseSyntaxError()
{
	XPathParser parser;

	try
	{
		Array<XPathOperatorPtr> prog = parser.Parse("///");
		UNIT_ASSERT("/// should have thrown an error", false);
	}
	catch (Exception *ex)
	{
		delete ex;
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseSyntaxError 1.0");

	try
	{
		Array<XPathOperatorPtr> prog = parser.Parse("^");
		UNIT_ASSERT("^ should have thrown an error", false);
	}
	catch (Exception *ex)
	{
		delete ex;
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseSyntaxError 1.0");
	
	try
	{
		Array<XPathOperatorPtr> prog = parser.Parse("/bookstore/");
		UNIT_ASSERT("/bookstore/ should have thrown an error", false);
	}
	catch (Exception *ex)
	{
		delete ex;
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseSyntaxError 1.0");
	
	try
	{
		Array<XPathOperatorPtr> prog = parser.Parse("/bookstore//");
		UNIT_ASSERT("/bookstore// should have thrown an error", false);
	}
	catch (Exception *ex)
	{
		delete ex;
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseSyntaxError 1.0");

	Log::SWriteOkFail( "XPath parse syntax errors" );
}

static void _TestXPathParseW3cSchoolExamples()
{
	XPathParser parser;

	parser.Parse("/bookstore/book[1]/title");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.0");

	// The following example selects the text from all the price nodes:
	parser.Parse("/bookstore/book/price/text()");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.1");

	// Select price nodes with price>35
	parser.Parse("/bookstore/book[price>35]/price");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.2");

	// Select title nodes with price>35
	parser.Parse("/bookstore/book[price>35]/title");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.3");

	// Selects all book nodes that are children of the current node
	parser.Parse("child::book");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.4");

	// Selects all children of the current node
	parser.Parse("child::*");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.5");

	// Selects all text child nodes of the current node
	parser.Parse("hild::text()");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.6");

	// Selects all book ancestors of the current node - and the current as well if it is a book node
	parser.Parse("ancestor-or-self::book");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.7");

	// Selects all price grandchildren of the current node
	parser.Parse("child::*/child::price");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples 1.8");

	Log::SWriteOkFail( "XPath w3c school exmaples parse" );
}

static void _TestXPathParsePathAttibutes()
{
	XPathParser parser;

	parser.Parse("/@");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("XPath attributes");
	
	parser.Parse("//@");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("XPath attributes");
	
	parser.Parse("/bookstore@");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("XPath attributes");

	parser.Parse("/bookstore/book@lang");
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	parser.CheckMem();
	UNIT_ASSERT_MEM_NOTED("XPath attributes");

	Log::SWriteOkFail( "XPath attributes" );
}

void _TestXPathParse(  )
{
	_TestXPathParse1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParse");

	_TestXPathParseSyntaxError();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseSyntaxError");

	_TestXPathParseW3cSchoolExamples();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParseW3cSchoolExamples");
	
	_TestXPathParsePathAttibutes();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestXPathParsePathAttibutes");
}

#endif
