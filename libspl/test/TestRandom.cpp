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
#include <spl/math/Random.h>
#include <spl/math/Sample.h>
#include <spl/math/SampleTest.h>

using namespace spl;

#ifdef DEBUG

static void _TestRandomInner(Random *rnd, bool rndCheckMem)
{
	int count = 6000;
	Array<double> data(count);
	
	for ( int x = 0; x < count; x++ )
	{
		data[x] = rnd->NextDouble();
	}
	
	Sample stats(data);
	
	UNIT_ASSERT( "Random count", stats.N() == count );
	UNIT_ASSERT( "Random mean", stats.Mean() > .46 && stats.Mean() < .53 );
	UNIT_ASSERT( "Random STD", stats.StDevSample() > 0 );
	UNIT_ASSERT( "Random Max", stats.Max() < 1 );
	UNIT_ASSERT( "Random range", stats.Min() != stats.Max() );
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	data.CheckMem();
	stats.CheckMem();
	if ( rndCheckMem )
	{
		DEBUG_NOTE_MEM( rnd );
	}
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Random");	
}

static void _TestRandom1()
{
	Random rnd;
	_TestRandomInner(&rnd, false);
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Random test 1.1");

	Log::SWriteOkFail( "Random test 1" );
}

static void _TestRandom2()
{
	const int SIZE = 4000;
	Random rnd;

	int x;
	Array<double> d1(SIZE);
	Array<double> d2(SIZE);

	for ( x = 0; x < SIZE; x++ )
	{
		d1[x] = rnd.NextDouble();
		ASSERT(d1[x] >= 0);
	}
	for ( x = 0; x < SIZE; x++ )
	{
		d2[x] = rnd.NextDouble();
		ASSERT(d1[2] >= 0);
	}

	SampleTest test(d1, d2);

	double med = test.DataSet1().Median();
	UNIT_ASSERT("Median off", med > 0.47 && med < 0.53);
	med = test.DataSet2().Median();
	UNIT_ASSERT("Median off", med > 0.47 && med < 0.53);

	med = test.DataSet1().Mean();
	UNIT_ASSERT("Mean off", med > 0.47 && med < 0.53);
	med = test.DataSet2().Mean();
	UNIT_ASSERT("Mean off", med > 0.47 && med < 0.53);

	double prop;
	double t = test.Levene(&prop);
	UNIT_ASSERT("Variances should be homogenous", t < .2);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	test.CheckMem();
	d1.CheckMem();
	d2.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Random test 2.1");

	Log::SWriteOkFail( "Random test 2" );
}

void _TestRandom()
{
	_TestRandom1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRandom2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
