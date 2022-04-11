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
#include <spl/UnitTest.h>
#include <spl/Int32.h>
#include <spl/io/log/Log.h>
#include <spl/collection/RedBlackTree.h>
#include <spl/math/Math.h>

using namespace spl;

#ifdef DEBUG

void _RbTest1()
{
	RedBlackTree<int, int> rbt;
	//rbt.Insert(1, 1);
	//UNIT_ASSERT("RbTest1 lookup present", rbt.Find(1) == 1);
	//UNIT_ASSERT("Contains key", rbt.ContainsKey(1));
	//DEBUG_CLEAR_MEM_CHECK_POINTS();
	//rbt.CheckMem();
	//UNIT_ASSERT_MEM_NOTED("redblack tree one node");
	//rbt.Remove(1);
	//UNIT_ASSERT("RbTest1 remove removed", rbt.Find(1) == 0);

	//DEBUG_CLEAR_MEM_CHECK_POINTS();
	//rbt.CheckMem();
	//UNIT_ASSERT_MEM_NOTED("redblack tree one node");

	Log::SWriteOkFail( "redblack tree one node" );
}

void _RbTest2()
{
/*	int x;
	Log::WriteCheck( "redblack tree 1 - 100 insert/remove" );
	RedBlackTree<int, Int32> rbt;
	
	for ( x = 0; x < 100; x++ )
	{
		rbt.Insert(x, x);
		UNIT_ASSERT("lookup", rbt.Find(x) == x);
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	rbt.CheckMem();
	UNIT_ASSERT_MEM_NOTED("redblack tree 1 - 100 insert/remove");
	for ( x = 0; x < 100; x++ )
	{
		UNIT_ASSERT("lookup", rbt.Find(x) == x);
		UNIT_ASSERT("Contains key", rbt.ContainsKey(x));
	}
	for ( x = 0; x < 100; x++ )
	{
		rbt.Remove(x);
	}
	UNIT_ASSERT("Should be empty", rbt.Count() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	rbt.CheckMem();
	UNIT_ASSERT_MEM_NOTED("redblack tree 1 - 100 insert/remove");

	Log::WriteOkFail( );*/
}

void _RbTest3()
{
/*	int x;
	Log::WriteCheck( "redblack tree 100 - 0 insert/remove" );
	RedBlackTree<int, Int32> rbt;
	
	for ( x = 100; x >= 0; x-- )
	{
		rbt.Insert(x, x);
	}
	for ( x = 100; x >= 0; x-- )
	{
		UNIT_ASSERT("lookup", rbt.Find(x) == x);
		UNIT_ASSERT("Contains key", rbt.ContainsKey(x));
	}
	for ( x = 100; x >= 0; x-- )
	{
		rbt.Remove(x);
	}
	UNIT_ASSERT("Should be empty", rbt.Count() == 0);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	rbt.CheckMem();
	UNIT_ASSERT_MEM_NOTED("redblack tree 100 - 0 insert/remove");

	Log::WriteOkFail(  );*/
}

void _RbTest4()
{
/*	int x;
	Log::WriteCheck( "redblack tree random" );
	Math::InitRandom();
	RedBlackTree<int, Int32> rbt;

	int count = 0;
	for ( x = 0; x < 500; x++ )
	{
		int k = (int)Math::RandomRange(300);
		if ( rbt.ContainsKey(k) )
		{
			UNIT_ASSERT("Key value should be", rbt.Find(k) == k);
		}
		else
		{
			UNIT_ASSERT("Key value should not be", rbt.Find(k) == 0);
			rbt.Insert(k, k);
			count++;
		}
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	rbt.CheckMem();
	UNIT_ASSERT_MEM_NOTED("redblack tree random");
	for ( x = 0; x < 500; x++ )
	{
		int k = (int)Math::RandomRange(300);
		rbt.Remove(k);
	}
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	rbt.CheckMem();
	UNIT_ASSERT_MEM_NOTED("redblack tree random");

	Log::WriteOkFail(  );*/
}

void RedblacktreeTestHarness()
{
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("redblack tree");
	_RbTest1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("redblack tree");
	_RbTest2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("redblack tree");
	_RbTest3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("redblack tree");
	_RbTest4();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("redblack tree");
}

#endif
