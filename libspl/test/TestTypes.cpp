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
#include <spl/types.h>

#ifdef DEBUG

using namespace spl;

void testtypes(  )
{
	UNIT_TEST( "int8", sizeof(int8) == 1 );
	UNIT_TEST( "int16", sizeof(int16) == 2 );
	UNIT_TEST( "int32", sizeof(int32) == 4 );
	UNIT_TEST( "int64", sizeof(int64) == 8 );
	UNIT_TEST( "uint8", sizeof(uint8) == 1 );
	UNIT_TEST( "byte", sizeof(byte) == 1 );
	UNIT_TEST( "uint16", sizeof(uint16) == 2 );
	UNIT_TEST( "uint32", sizeof(uint32) == 4 );
	UNIT_TEST( "uint64", sizeof(uint64) == 8 );
	UNIT_TEST( "bool true", true == (1==1) );
	UNIT_TEST( "bool false", false == (1==0) );
}

#endif /* DEBUG */


