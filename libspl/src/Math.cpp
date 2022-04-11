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

#include <spl/math/Math.h>

using namespace spl;

uint32 Math::combo_x = 3;
uint32 Math::combo_y = 1;
uint32 Math::combo_z = 1;
uint32 Math::combo_v = 0;
bool Math::gInited = false;

#define RAND_MAX_COMBO (4294967295.0/2)

IHashable::~IHashable()
{
}

IComparable::~IComparable()
{
}

IMemoryValidate::~IMemoryValidate()
{
}

void Math::InitRandom()
{
	uint32 seed;

	time_t _t;
	time (&_t);
	seed = ((long)_t) & (((long)_t) << 16) & ((long)_t) & (((long)_t) << 16);
    combo_x = seed * 8 + 3;
    combo_y = seed * 2 + 1;
    combo_z = seed | 1;
    combo_v = 0;
    
    gInited = true;
}

double Math::Random()
{
	double d = fabs((double)(RandomInt())/(double)RAND_MAX_COMBO);
	Debug::Assert(d < 1.0);
	return d;
}

int Math::RandomInt()
{
	if ( ! gInited )
	{
		InitRandom();
	}
    combo_v = combo_x * combo_y;
    combo_x = combo_y;
    combo_y = combo_v;
    combo_z = (combo_z & 65535L) * 30903 + (combo_z >> 16);
	return (int)(combo_y + combo_z);
}

// Internal method to test if a positive number is prime.
// Not an efficient algorithm.
bool Math::IsPrime( const int n ) 
{
	if( n == 2 || n == 3 )
	{
		return true;
	}
	if( n == 1 || n % 2 == 0 )
	{
		return false;
	}
	for( int i = 3; i * i <= n; i += 2 )
	{
		if( n % i == 0 )
		{
			return false;
		}
	}
	return true;
}

// Internal method to return a prime number at least as large as n.
// Assumes n > 0.
int Math::NextPrime( const int n )
{
	int prime = n;
	if( prime % 2 == 0 )
	{
		prime++;
	}
	for( ; !IsPrime( prime ); prime += 2 )
	{
	}
	return prime;
}

uint32 Math::Hash( const float32 i )
{
	double fractpart, intpart;
	fractpart = modf (i, &intpart);
	
	return ( ((uint32)intpart ) ^ (uint32)(fractpart * 100000) );
}

uint32 Math::Hash( const float64 i )
{
	double fractpart, intpart;
	fractpart = modf (i, &intpart);
	
	return ( ((uint32)intpart ) ^ (uint32)(fractpart * 100000) );
}

uint64 Math::HashLong(const char *cp)
{
	uint64 h = 0;
	int count = (int)strlen(cp);
	int x;
	
	for (x = 0; x < count; x++)
	{
		h = (h << 6) ^ (h >> 58) ^ cp[x];
	}

	return h;
}

uint32 Math::Hash( const char *str )
{
	uint64 h = HashLong(str);

	return (uint32)((h & 0xFFFFFFFF) ^ (h >> 32));
}

uint32 Math::Hash( const IHashable& i)
{
	return i.HashCode();
}

uint32 Math::Hash( const IHashable *i)
{
	return i->HashCode();
}

float32 FastMath::InvSqrt(float32 x)
{
	/* A very fast function to calculate the approximate inverse square root of a
	 * floating point value and a helper function that uses it for getting the
	 * normal squareroot. For an explanation of the inverse squareroot function
	 * read:
	 * http://www.math.purdue.edu/~clomont/Math/Papers/2003/InvSqrt.pdf
	 *
	 * Unfortunately the original creator of this function seems to be unknown.
	 */
	union { int i; float32 x; } tmp;
	float32 xhalf = 0.5f * x;
	tmp.x = x;
	tmp.i = 0x5f375a86 - (tmp.i >> 1);
	x = tmp.x;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

float64 FastMath::m_sinus[4096];
float64 FastMath::m_cosinus[4096];
bool FastMath::m_trig = false;
float64 FastMath::m_rad2scale = 4096.0 / Math::PI() / 2.0;
float64 FastMath::m_pad = 256 * Math::PI();

int FastMath::m_fastRandoms[32];
int FastMath::m_fastRndPointer = 0;
bool FastMath::m_fastRndInit = false;

void FastMath::BuildTrig()
{
	for (int i = 0; i < 4096; i++)
	{
		m_sinus[i] = Math::Sin((float64) (i / m_rad2scale));
		m_cosinus[i] = Math::Cos((float64) (i / m_rad2scale));
	}
	m_trig = true;
}

void FastMath::CropBuffer(int *buffer, int size, int min, int max)
{
	for (int i = size - 1; i >= 0; i--) 
	{
		buffer[i] = Crop(buffer[i], min, max);
	}
}

int FastMath::FastRandom(int bits)
{
	if ( bits < 1 ) 
		return 0;
	m_fastRndPointer = (m_fastRndPointer+1) & 31;
	if (!m_fastRndInit)
	{
		for (int i=0;i<32;i++) 
		{
			m_fastRandoms[i] = (int)Random(0,0xFFFFFF);
		}
		m_fastRndInit = true;
	}
	return m_fastRandoms[m_fastRndPointer]&(1<<(bits-1));
}
