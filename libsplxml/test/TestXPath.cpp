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
#include <spl/xml/XmlElement.h>
#include <spl/xml/XmlDocument.h>
#include <spl/xml/xpath/XPath.h>

using namespace spl;

#ifdef DEBUG

static const char *_xpathTestXml =
	"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
	"\n"
	"<bookstore>\n"
	"\n"
	"<book category=\"COOKING\">\n"
	"  <title lang=\"en\">Everyday Italian</title>\n"
	"  <author>Giada De Laurentiis</author>\n"
	"  <year>2005</year>\n"
	"  <price>30.00</price>\n"
	"</book>\n"
	"\n"
	"<book category=\"CHILDREN\">\n"
	"  <title lang=\"en\">Harry Potter</title>\n"
	"  <author>J K. Rowling</author>\n"
	"  <year>2005</year>\n"
	"  <price>29.99</price>\n"
	"</book>\n"
	"\n"
	"<book category=\"WEB\">\n"
	"  <title lang=\"en\">XQuery Kick Start</title>\n"
	"  <author>James McGovern</author>\n"
	"  <author>Per Bothner</author>\n"
	"  <author>Kurt Cagle</author>\n"
	"  <author>James Linn</author>\n"
	"  <author>Vaidyanathan Nagarajan</author>\n"
	"  <year>2003</year>\n"
	"  <price>49.99</price>\n"
	"</book>\n"
	"\n"
	"<book category=\"WEB\">\n"
	"  <title lang=\"en\">Learning XML</title>\n"
	"  <author>Erik T. Ray</author>\n"
	"  <year>2003</year>\n"
	"  <price>39.95</price>\n"
	"</book>\n"
	"\n"
	"</bookstore>";

static void _TestXPathRoot()
{
	XmlDocumentPtr doc = XmlDocument::ParseXml(_xpathTestXml);

	XPath xpath("/");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath / 1");
	
	XmlNodePtr root = xpath.SelectSingleNode(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("/ == docuemnt", root->IsDocument());
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	root.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath / 2");

	Log::SWriteOkFail( "XPath /" );
}

static void _TestXPathRootChild()
{
	XmlDocumentPtr doc = XmlDocument::ParseXml(_xpathTestXml);

	XPath xpath("/bookstore");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore 1");
	
	XmlNodePtr root = xpath.SelectSingleNode(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("/bookstore == bookstore", root->Name() == "bookstore");
	
	xpath = XPath("bookstore");
	root = xpath.SelectSingleNode(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("/bookstore == bookstore", root->Name() == "bookstore");
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	root.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore 2");	

	Log::SWriteOkFail( "XPath /bookstore" );
}

static void _TestXPathRootPath()
{
	XmlDocumentPtr doc = XmlDocument::ParseXml(_xpathTestXml);

	XPath xpath("/bookstore/book");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore/book 1");
	
	XmlNodeListPtr root = xpath.SelectNodes(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("Should be 4 nodes", root->Count() == 4);
	int x;
	for (x = 0; x < root->Count(); x++)
	{
		UNIT_ASSERT("tag name", root->Item(x)->Name() == "book");
	}
	
	xpath = XPath("/bookstore/*");
	root = xpath.SelectNodes(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("Should be 4 nodes", root->Count() == 4);
	for (x = 0; x < root->Count(); x++)
	{
		UNIT_ASSERT("tag name", root->Item(x)->Name() == "book");
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	root.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore/book 2");	

	Log::SWriteOkFail( "XPath /bookstore/book" );
}

static void _TestXPathRootPathSearch()
{
	XmlDocumentPtr doc = XmlDocument::ParseXml(_xpathTestXml);

	XPath xpath("/bookstore//price");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore//price 1");
	
	XmlNodeListPtr root = xpath.SelectNodes(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("Should be 4 nodes", root->Count() == 4);
	for (int x = 0; x < root->Count(); x++)
	{
		UNIT_ASSERT("tag name", root->Item(x)->Name() == "price");
	}
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	root.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore//price 2");	

	Log::SWriteOkFail( "XPath /bookstore//price" );
}

static void _TestXPathAttribPath()
{
	XmlDocumentPtr doc = XmlDocument::ParseXml(_xpathTestXml);

	XPath xpath("/bookstore/book/title@lang");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore/book 1");
	
	XmlNodeListPtr root = xpath.SelectNodes(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("Should be 4 nodes", root->Count() == 4);
	int x;
	for (x = 0; x < root->Count(); x++)
	{
		UNIT_ASSERT("tag name", root->Item(x)->Name() == "lang");
	}
	
	xpath = XPath("/bookstore/@");
	root = xpath.SelectNodes(doc);
	UNIT_ASSERT("Shouldn't be null", root.IsNotNull());
	UNIT_ASSERT("Should be 4 nodes", root->Count() == 4);
	for (x = 0; x < root->Count(); x++)
	{
		UNIT_ASSERT("tag name", root->Item(x)->Name() == "category");
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	xpath.CheckMem();
	doc.CheckMem();
	root.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Xpath /bookstore/book 2");	

	Log::SWriteOkFail( "XPath /bookstore/book/title@lang" );
}

static void _TestXPathPredicate()
{
	XmlDocumentPtr doc = XmlDocument::ParseXml(_xpathTestXml);
	XmlNodeListPtr nodes = doc->SelectNodes("/bookstore/book[@category = 'WEB']");

	UNIT_ASSERT("Shouldn't be null", nodes.IsNotNull());
	UNIT_ASSERT("Should be 2 nodes", nodes->Count() == 2);

	for (int x = 0; x < nodes->Count(); x++)
	{
		UNIT_ASSERT("tag name", nodes->Item(x)->Name() == "book");
		XmlElementPtr xn = nodes->Item(x)->ToElement();
		XmlAttributePtr an = xn->Attribute("category");
		UNIT_ASSERT("tag name", an->Name() == "category");
		UNIT_ASSERT("tag name", *an->Value() == "WEB");
	}
	
	Log::SWriteOkFail( "XPath /bookstore/book[@category = 'WEB']" );
}

void _TestXPath()
{
	_TestXPathRoot();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestXPathRootChild();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestXPathRootPath();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestXPathRootPathSearch();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestXPathAttribPath();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestXPathPredicate();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
