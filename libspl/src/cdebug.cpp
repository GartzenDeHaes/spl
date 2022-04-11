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
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#include <spl/Debug.h>
#include <spl/Exception.h>
#include <spl/io/log/Log.h>
#include <spl/String.h>

using namespace spl;

#if defined(DEBUG)

void *operator new(size_t size)
{
	return malloc( (int)size );
}

void operator delete(void *vp)
{
	free( vp );
}

void *operator new[](size_t size)
{
	void *mem = _debugMalloc( (int)size, __FILE__, __LINE__, true );
	return mem;
}

void operator delete[](void *vp)
{
	free( vp );
}

void debugAssertCPP(const int cond, const char *file, const int line)
{
	if ( !cond )
	{
		char buf[1024];
		sprintf(buf, "Assertion failed in %s on line %d", file, line);
		throw new AssertionFailedException(buf);
	}
}

void _unitAssert( const char *msg, const int cond, const char *filename, const int lineno )
{
	if ( cond == 0 )
	{
		char buf[256];
		sprintf(buf, "ASSERTION FAILED in %s, on line %d\n%s\n", filename, lineno, msg);

		Log::SWriteError( buf );
#ifdef __TANDEM
		exit(20);
#endif
	}
}


void _unitTest( const char *msg, const int cond)
{
	Log::SWriteInfo(String::Format("%s ..... %s\n", msg, ((cond != 0) ? "ok" : "FAIL")));
}

#endif
