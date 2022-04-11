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

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <math.h>

#include <spl/math/Random.h>

using namespace spl;

#define RAND_MAX_COMBO (4294967295.0/2)

Random::Random()
{
	combo_x = 3;
	combo_y = 1;
	combo_z = 1;
	combo_v = 0;

	uint32 seed;

	time_t _t;
	time (&_t);
	seed = ((long)_t) & (((long)_t) << 16) & ((long)_t) & (((long)_t) << 16);
    combo_x = seed * 8 + 3;
    combo_y = seed * 2 + 1;
    combo_z = seed | 1;
    combo_v = 0;
}

Random::Random(int seed)
{
    combo_x = seed * 8 + 3;
    combo_y = seed * 2 + 1;
    combo_z = seed | 1;
    combo_v = 0;
}

int Random::Next()
{
    combo_v = combo_x * combo_y;
    combo_x = combo_y;
    combo_y = combo_v;
    combo_z = (combo_z & 65535L) * 30903 + (combo_z >> 16);
	int rnd = combo_y + combo_z;
	if ( rnd < 0 )
	{
		return -1 * rnd;
	}
	return rnd;
}

int Random::Next(int max)
{
	return Next() % max;
}

int Random::Next(int min, int max)
{
	return (int)(NextDouble() * (max - min) + min);
}

double Random::NextDouble()
{
	double val = fabs((double)Next()/(double)RAND_MAX_COMBO);
	ASSERT( val <= 1.0 );
	return val;
}

void Random::NextBytes(byte *data, int len)
{
	for ( int x = 0; x < len; x++ )
	{
		int next = Next(256);
		ASSERT (next < 256);
		data[x] = (byte)next;
	}
}

int Random::NextInt()
{
    combo_v = combo_x * combo_y;
    combo_x = combo_y;
    combo_y = combo_v;
    combo_z = (combo_z & 65535L) * 30903 + (combo_z >> 16);
	return combo_y + combo_z;
}

int Random::SNextInt()
{
	Random rnd;
	return rnd.NextInt();
}
