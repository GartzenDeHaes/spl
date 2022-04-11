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
#include <stdlib.h>

#include <spl/Debug.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifdef DEBUG

static void simplemalloctest(  )
{
	char *cp;

	DEBUG_CHECK_POINT_HEAP();

	cp = (char *)malloc( 2 * sizeof(char) );
	UNIT_ASSERT_MEM("debugmalloctest", cp, 2*sizeof(char));

	cp[1] = '\0';
	cp[0] = 'A';
	DEBUG_CLEAR_MEM_CHECK_POINTS();

	UNIT_ASSERT_PTR("debugmalloctest", cp);
	UNIT_ASSERT_MEM("debugmalloctest", cp, 2*sizeof(char));
	DEBUG_NOTE_MEM_ALLOCATION( cp );
	UNIT_ASSERT_MEM_NOTED("debugmalloctest");

	free( cp );

	UNIT_ASSERT_CHECK_POINT("debugmalloctest");
	Log::SWriteOkFail( "debug simple malloc" );
}

static void debugmalloctest(  )
{
	void *allocs[1000];
	int x;

	DEBUG_CHECK_POINT_HEAP();

	for ( x = 0; x < 1000; x++ )
	{
		allocs[x] = NULL;
	}
	for ( x = 0; x < 1000; x++ )
	{
		allocs[x] = malloc( 100 );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();

	for ( x = 0; x < 1000; x++ )
	{
		UNIT_ASSERT_PTR("debugmalloctest", allocs[x]);
		UNIT_ASSERT_MEM("debugmalloctest", allocs[x], 100);
		DEBUG_NOTE_MEM_ALLOCATION( allocs[x] );
	}
	UNIT_ASSERT_MEM_NOTED("debugmalloctest");

	for ( x = 0; x < 1000; x++ )
	{
		free(allocs[x]);
	}
	UNIT_ASSERT_CHECK_POINT("debugmalloctest");
	Log::SWriteOkFail( "debug malloc" );
}

static void pointercheck(  )
{
	char *cp;

	cp = (char *)malloc(10);
	UNIT_ASSERT( "pointercheck", _debugCheckPtr(cp) );
	UNIT_ASSERT( "pointercheck", _debugCheckPtr(&cp[4]) );
	UNIT_ASSERT( "pointercheck", ! _debugCheckPtr(&cp[10]) );
	free(cp);

	Log::SWriteOkFail( "debug pointer check" );
}

static void memblockcheck(  )
{
	char *cp;
	
	cp = (char *)malloc(10);
	UNIT_ASSERT( "", _debugCheckBlock(cp, 10) );
	free(cp);

	Log::SWriteOkFail( "debug mem block check" );
}

void _testdebug(  )
{
	simplemalloctest(  );
	debugmalloctest(  );
	pointercheck(  );
	memblockcheck(  );
	UNIT_ASSERT_MEM_NOTED("testdebug");
}

#endif /* DEBUG */
