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

#ifdef DEBUG
#include <spl/io/log/Log.h>
#include <spl/web/Uri.h>

using namespace spl;

static void _TestUri1()
{
	Uri *uri = new Uri();
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(uri);
	uri->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Uri test 1.0");

	uri->Parse("http://localhost:8080/xml?addr=3501%20NE%20105TH%20ST&city=&zip=98125");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(uri);
	uri->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Uri test 1.1");

	UNIT_ASSERT("protocol", uri->Protocol().Equals("http"));
	UNIT_ASSERT("host", uri->Host().Equals("localhost"));
	UNIT_ASSERT("port", uri->Port() == 8080);
	UNIT_ASSERT("path", uri->Path().Equals("/xml"));
	UNIT_ASSERT("param count", uri->Args().Count() == 3);
	UNIT_ASSERT("has addr", uri->HasArg("addr"));
	UNIT_ASSERT("has city", uri->HasArg("city"));
	UNIT_ASSERT("has zip", uri->HasArg("zip"));
	UNIT_ASSERT("addr", uri->GetArg("addr").Equals("3501 NE 105TH ST"));
	UNIT_ASSERT("city", uri->GetArg("city").Equals(""));
	UNIT_ASSERT("zip", uri->GetArg("zip").Equals("98125"));

	delete uri;

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Uri test 1.3");

	uri = new Uri();
	uri->Parse("/");
	UNIT_ASSERT("/", uri->Path().Equals("/"));

	delete uri;

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Uri test 1.4");

	Log::SWriteOkFail( "Uri test 1" );
}

static void _TestUri2()
{
	Uri *uri = new Uri();
	
	uri->Parse("http://localhost/xml.asp");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(uri);
	uri->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Uri test 2.0");

	UNIT_ASSERT("protocol", uri->Protocol().Equals("http"));
	UNIT_ASSERT("host", uri->Host().Equals("localhost"));
	UNIT_ASSERT("port", uri->Port() == 80);
	UNIT_ASSERT("path", uri->Path().Equals("/"));
	UNIT_ASSERT("filename", uri->Filename().Equals("xml.asp"));
	UNIT_ASSERT("fileext", uri->FileExt().Equals("asp"));
	UNIT_ASSERT("param count", uri->Args().Count() == 0);

	delete uri;

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Uri test 2.3");

	Log::SWriteOkFail( "Uri test 2" );
}

void _TestUri()
{
	_TestUri1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestUri2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}


#endif
