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
#ifndef _math_h
#define _math_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <spl/Decimal.h>
#include <spl/Exception.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	#ifdef __TANDEM
	/* Stupid tandem doesn't support standard float functions. */
	#define finite(d) (!isnan(d))
	#define fabsf fabs
	#define acosf acos
	#define asinf asin
	#define atanf atan
	#define atan2f atan2
	#define ceilf ceil
	#define cosf cos
	#define coshf cosh
	#define expf exp
	#define floorf floor
	#define fmodf fmod
	#define logf log
	#define log10f log10
	#define powf pow
	#define sinf sin
	#define sinhf sinh
	#define sqrtf sqrt
	#define tanf tan
	#define tanhf tanh
	#endif

	/**
	 *	@brief Static class for math functions.
	 *	Besides cross platform issues, this also address version issues in MSVC.
	 */
	class Math
	{
	private:
		static uint32 combo_x;
		static uint32 combo_y;
		static uint32 combo_z;
		static uint32 combo_v;
		static bool gInited;
		
	public:
		inline static double E() { return 2.71828182845904523536; }
		inline static double PI() { return 3.14159265358979323846; }
		inline static double LOG2E() { return 1.44269504088896340736; }
		inline static double LOG10E() { return 0.434294481903251827651; }
		inline static double LN2() { return 0.693147180559945309417; }
		inline static double LN10() { return 2.30258509299404568402; }
		inline static double PI_2() { return 1.57079632679489661923; }
		inline static double PI_4() { return 0.785398163397448309616; }
		inline static double SQRT2() { return 1.41421356237309504880; }
		inline static double SQRT1_2() { return 0.707106781186547524401; }

		inline static bool IsINF(double d) 
		{
	#ifdef _WINDOWS 
			return _finite(d) == 0;
	#else
			return finite(d) == 0;
	#endif 
		}

		inline static bool IsNaN(double d) 
		{ 
	#ifdef _WINDOWS
			return _isnan(d) != 0; 
	#else
			return isnan(d) != 0; 
	#endif
		}

		inline static int Abs(int8 i) { return abs(i); }
		inline static int16 Abs(int16 i) { return (int16)abs(i); }
		inline static int32 Abs(int32 i) { return (int32)abs(i); }
		inline static int64 Abs(int64 i) 
		{ 
	#ifdef _WINDOWS
	#if _MSC_VER > 1200
			return _abs64(i);
	#else
			return i < 0 ? -i : i;
	#endif
	#else
			return labs(i); 
	#endif
		}
		inline static float32 Abs(float32 f) { return (float32)fabsf(f); }
		inline static float64 Abs(float64 f) { return fabs(f); }
		inline static Decimal Abs(Decimal d) { return d.Abs(); }
		inline static double Acos(double f) { return acos(f); }
		inline static float Acos(float f) { return (float32)acosf(f); }
		inline static double Asin(double f) { return asin(f); }
		inline static float Asin(float f) { return (float32)asinf(f); }
		inline static double Atan(double f) { return atan(f); }
		inline static float Atan(float f) { return (float32)atanf(f); }
		inline static double Atan2(double x, double y) { return atan2(x, y); }
		inline static float Atan2(float x, float y) { return (float32)atan2f(x, y); }
		inline static double Ceiling(double f) { return ceil(f); }
		inline static float Ceiling(float f) { return (float32)ceilf(f); }
		inline static double Cos(double f) { return cos(f); }
		inline static float Cos(float f) { return (float32)cosf(f); }
		inline static double Cosh(double f) { return cosh(f); }
		inline static float Cosh(float f) { return (float32)coshf(f); }
		inline static double Exp(double d) { return exp(d); }
		inline static float Exp(float d) { return (float32)expf(d); }
		inline static double Floor(double f) { return floor(f); }
		inline static float Floor(float f) { return (float32)floorf(f); }
		inline static double Remainder(double x, double y) { return fmod(x, y); }
		inline static float Remainder(float x, float y) { return (float32)fmodf(x, y); }
		inline static double Log(double d) { return log(d); }
		inline static float Log(float d) { return (float32)logf(d); }
		inline static double Log(double d, double base) { return log(d)/log(base); }
		inline static float Log(float d, float base) { return (float32)(logf(d)/logf(base)); }
		inline static double Log10(double d) { return log10(d); }
		inline static float Log10(float d) { return (float32)log10f(d); }
		inline static int8 Max(int8 a, int8 b) { return (a>b) ? a : b; }
		inline static byte Max(byte a, byte b) { return (a>b) ? a : b; }
		inline static int16 Max(int16 a, int16 b) { return (a>b) ? a : b; }
		inline static uint16 Max(uint16 a, uint16 b) { return (a>b) ? a : b; }
		inline static int32 Max(int32 a, int32 b) { return (a>b) ? a : b; }
		inline static uint32 Max(uint32 a, uint32 b) { return (a>b) ? a : b; }
		inline static int64 Max(int64 a, int64 b) { return (a>b) ? a : b; }
	#ifndef __TANDEM
		inline static uint64 Max(uint64 a, uint64 b) { return (a>b) ? a : b; }
	#endif
		inline static float32 Max(float32 a, float32 b) { return (a>b) ? a : b; }
		inline static double Max(double a, double b) { return (a>b) ? a : b; }
		inline static Decimal Max(Decimal a, Decimal b) { return (a>b) ? a : b; }
		inline static int8 Min(int8 a, int8 b) { return (a<b) ? a : b; }
		inline static byte Min(byte a, byte b) { return (a<b) ? a : b; }
		inline static int16 Min(int16 a, int16 b) { return (a<b) ? a : b; }
		inline static uint16 Min(uint16 a, uint16 b) { return (a<b) ? a : b; }
		inline static int32 Min(int32 a, int32 b) { return (a<b) ? a : b; }
		inline static uint32 Min(uint32 a, uint32 b) { return (a<b) ? a : b; }
		inline static int64 Min(int64 a, int64 b) { return (a<b) ? a : b; }
	#ifndef __TANDEM
		inline static uint64 Min(uint64 a, uint64 b) { return (a<b) ? a : b; }
	#endif
		inline static float32 Min(float32 a, float32 b) { return (a<b) ? a : b; }
		inline static double Min(double a, double b) { return (a<b) ? a : b; }
		inline static Decimal Min(Decimal a, Decimal b) { return (a<b) ? a : b; }
		inline static double Pow(double d, double e) { return pow(d, e); }
		inline static float Pow(float d, float e) { return (float32)powf(d, e); }
		inline static double Round(double d) { return (double)((int64)d); }
		inline static float Round(float d) { return (float)((int64)d); }
		inline static double Round(double d, int dec) { double r = pow(10.0, dec); return floor(d*r)/r; }
		inline static float Round(float d, int dec) { double r = pow(10.0, dec); return (float)(floor(d*r)/r); }
		inline static Decimal Round(Decimal d) { return d.Round(); }
		inline static int Sign(int8 i) { return (i) ? ((i>0)?1:-1) : 0; }
		inline static int Sign(int16 i) { return (i) ? ((i>0)?1:-1) : 0; }
		inline static int Sign(int32 i) { return (i) ? ((i>0)?1:-1) : 0; }
		inline static int Sign(int64 i) { return (i) ? ((i>0)?1:-1) : 0; }
		inline static int Sign(float32 f) { return (f) ? ((f>0)?1:-1) : 0; }
		inline static int Sign(double d) { return (d) ? ((d>0)?1:-1) : 0; }
		inline static int Sign(Decimal d) { return (d.RawData()) ? ((d.RawData()>0)?1:-1) : 0; }
		inline static double Sin(double d) { return sin(d); }
		inline static float Sin(float d) { return (float32)sinf(d); }
		inline static double Sinh(double d) { return sinh(d); }
		inline static float Sinh(float d) { return (float32)sinhf(d); }
		inline static double Sqrt(double d) { return sqrt(d); }
		inline static float Sqrt(float d) { return (float32)sqrtf(d); }
		inline static double Tan(double d) { return tan(d); }
		inline static float Tan(float d) { return (float32)tanf(d); }
		inline static double Tanh(double d) { return tanh(d); }
		inline static float Tanh(float d) { return (float32)tanhf(d); }

		static void InitRandom();
		static double Random();
		static int RandomInt();
		inline static int RandomRange(int max) { return (int)(Random() * max); }
		
		static bool IsPrime( const int n );
		static int NextPrime( const int n );
		
		inline static int32 Hash( const int64 i ) { return (int32)((i >> 32) ^ (i && 0XFFFFFFFF)); }
		inline static int32 Hash( const int32 i ) { return i; }
		inline static int32 Hash( const int16 i ) { return i | (i << 16); }
		inline static int32 Hash( const int8 i ) { return i | (i << 8) | (i << 16) | (i << 24); }
		inline static int32 Hash( const uint64 i ) { return (int32)((i >> 32) ^ (i && 0XFFFFFFFF)); }
		inline static int32 Hash( const uint32 i ) { return (int32)i; }
		inline static int32 Hash( const uint16 i ) { return i | (i << 16); }
		inline static int32 Hash( const uint8 i ) { return i | (i << 8) | (i << 16) | (i << 24); }
		static uint32 Hash( const float32 i );
		static uint32 Hash( const float64 i );
		static uint32 Hash( const char *cp );
		static uint64 HashLong( const char *cp );
		static uint32 Hash( const IHashable& i );
		static uint32 Hash( const IHashable *i );
	};

	/**
	  * @brief Static class for math function with increased speed, but reduced accuracy.
	  */
	class FastMath
	{
	protected:
		static float64 m_sinus[4096];
		static float64 m_cosinus[4096];
		static bool m_trig;
		static float64 m_pi;
		static float64 m_rad2scale;
		static float64 m_pad;

		static int m_fastRandoms[32];
		static int m_fastRndPointer;
		static bool m_fastRndInit;

		static float32 InvSqrt(float32 x);
		static void BuildTrig();

	public:
		inline static float32 Sqrt(float32 x)
		{
			return 1.0f / InvSqrt(x);
		}

		inline static float64 Sqrt(float64 x)
		{
			return Math::Sqrt(x);
		}

		static inline float64 Deg2rad(float64 deg)
		{
			return deg * 0.0174532925194f;
		}

		static inline float64 Rad2deg(float64 rad)
		{
			return rad * 57.295779514719f;
		}

		static inline float64 Sin(float64 angle)
		{
			if(!m_trig) BuildTrig();
			return m_sinus[(int)((angle+m_pad)*m_rad2scale)&0xFFF];
		}

		static inline float64 Cos(float64 angle)
		{
			if(!m_trig) BuildTrig();
			return m_cosinus[(int)((angle+m_pad)*m_rad2scale)&0xFFF];
		}

		static inline float64 Pythagoras(float64 a, float64 b)
		{
			return Sqrt(a*a+b*b);
		}

		static inline int Pythagoras(int a, int b)
		{
			return (int)Sqrt((float64)(a*a+b*b));
		}

		// R A N G E  T O O L S

		static inline int Crop(int num, int min, int max)
		{
			return (num<min) ? min : (num>max) ? max : num;
		}

		static inline float64 Crop(float64 num, float64 min, float64 max)
		{
			return (num<min) ? min : (num>max) ? max : num;
		}

		static inline bool InRange(int num, int min, int max)
		{
			return ((num>=min) && (num<max));
		}

		// B U F F E R   O P E R A T I O N S

		static inline void ClearBuffer(int *buffer, int size, int value)
		{
			memset(buffer, value, sizeof(int)*size);
		}

		static void CropBuffer(int *buffer, int size, int min, int max);

		static inline void CopyBuffer(int *source, int size, int *target)
		{
			memcpy(target, source, size*sizeof(int));
		}

		// R A N D O M  N U M B E R S

		static inline float64 Random()
		{
			return (float64)(Math::Random());
		}

		static inline float64 Random(float64 min, float64 max)
		{
			return (float64)(Math::Random()*(max-min)+min);
		}

		static inline float64 RandomWithDelta(float64 averidge, float64 delta)
		{
			return averidge + Random() * delta;
		}

		static int FastRandom(int bits);

		static inline int FastRndBit()
		{
			return FastRandom(1);
		}

		static inline float64 Interpolate(float64 a, float64 b, float64 d)
		{
			float64 f = (1-Cos(d*m_pi))*0.5f;
			return a+f*(b-a);
		}
		
		inline bool IsPrime( const int n ) { return Math::IsPrime(n); }
		inline int NextPrime( const int n ) {	return Math::NextPrime(n); }
	};

	/** @} */
}
#endif
