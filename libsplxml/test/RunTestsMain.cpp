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
#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/io/log/Log.h>
#include <spl/UnitTest.h>
#include <spl/net/Socket.h>

#ifdef DEBUG

using namespace spl;

int main(int argc, char **argv)
{
	try
	{
		TestHarness::RunTests();
		Log::SWriteEndOfRunTotal();
		
		ASSERT_MEM_FREE();
		Socket::ShutdownService();
		DEBUG_TEAR_DOWN(true);                  
		return (Log::SMessageCount() > 0) ? 100 : 0;
	}
	catch ( Exception *ex )
	{
		Log::SWrite( ex );
		delete ex;
	}
	catch ( OutOfMemoryException mex )
	{
		Log::SWrite( mex );
	}
	return 20;
}

#endif
