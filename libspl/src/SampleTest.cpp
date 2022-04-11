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
#include <spl/math/SampleTest.h>

using namespace spl;

static void _Sort(int N, double *ar)
{
	for (int x = 0; x < N; x++)
	{
		for(int y = x; y < N; y++)
		{
			if( ar[x] > ar[y] )
			{
				double d = ar[x];
				ar[x] = ar[y];
				ar[y] = d;
			}
		}
	}
}

struct _SPair
{
	double val;
	bool one;
};

static void _ArrayPairInsert(struct _SPair *a, const int alen, double d, bool one)
{
/*
' insert n value into an n dimensional array
'
' called by:  RankSQV
*/
	for(int x = 0; x < alen; x++)
	{
		if (d >= a[x].val)
		{
			//'
			//' insert the value
			//'
			for (int y = alen-1; y > x; y--)
			{
				a[y].val = a[y - 1].val;
				a[y].one = a[y - 1].one;
			}
			a[x].val = d;
			a[x].one = one;
			return;
		}
	}
	throw new Exception("shouldn't get here");
}

double _AvgRank(struct _SPair *pairs, const int pairslen, int x)
{
	double target = pairs[x].val;

	for(int i = 0; i < pairslen; i++)
	{
		if (pairs[i].val == target)
		{
			// average the ranks of the duplicates
			int N = 1;
			double sum = i + 1;
			while( i < pairslen )
			{
				if( pairs[i].val != pairs[i + 1].val )
				{
					break;
				}
				N = N + 1;
				i = i + 1;
				sum = sum + i + 1;
			}
			return sum / N;
		}
	}

	throw new Exception("Shouldn't get here");
}

static double _SortSumR1(Sample *a, Sample *b)
{
	int x;

	double r = 0;

	int N = a->N() + b->N();
	struct _SPair *pairs = new _SPair[N];
	if ( NULL == pairs )
	{
		throw OutOfMemoryException();
	}

	for (x = 0; x < N; x++)
	{
		pairs[x].val = -1;
	}

	for (x = 0; x < a->N(); x++)
	{
		_ArrayPairInsert( pairs, N, a->Item(x), true );
	}

	for (x = 0; x < b->N(); x++)
	{
		_ArrayPairInsert( pairs, N, b->Item(x), false );
	}

	for (x = 0; x < N; x++)
	{
		if (pairs[x].one)
		{
			r = r + _AvgRank(pairs, N, x);
		}
	}

	delete[] pairs;

	return r;
}

static double _Rankits(int N, int rank)
{
	static double table[][20] = {
	//		1		2		3		4		5		6		7		8		9					10		11		12		13		14		15		16		17		18		19		20
	{/*1*/	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,				-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*2*/	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,				-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*3*/	-0.846,	0,		0.846,	-777,	-777,	-777,	-777,	-777,	-777,				-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*4*/	-1.029,	-0.297,	0.297,	1.029,	-777,	-777,	-777,	-777,	-777,				-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*5*/	-1.163,	-0.495,	0,		0.495,	1.163,	-777,	-777,	-777,	-777,				-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*6*/	-1.267,	-0.642,	-0.202,	0.202,	0.642,	1.267,	-777,	-777,	-777,				-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*7*/	-1.352,	-0.757,	-0.353,	0,		0.353,	0.757,	1.352,	-777,	-777,		/*7*/	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*8*/	-1.424,	-0.852,	-0.473,	-0.153,	0.153,	0.473,	0.852,	1.424,	-777,		/*8*/	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*9*/	-1.485,	-0.932,	-0.572,	-0.275,	0,		0.275,	0.572,	0.932,	1.485,		/*9*/	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*10*/	-1.539,	-1.001,	-0.656,	-0.376,	-0.123,	0.123,	0.376,	0.656,	1.001,		/*10*/	1.539,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*11*/	-1.586,	-1.062,	-0.729,	-0.462,	-0.225,	0,		0.225,	0.462,	0.729,		/*11*/	1.062,	1.586,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*12*/	-1.629,	-1.116,	-0.793,	-0.537,	-0.312,	-0.103,	0.103,	0.312,	0.537,		/*12*/	0.793,	1.116,	1.629,	-777,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*13*/	-1.668,	-1.164,	-0.85,	-0.603,	-0.388,	-0.19,	0,		0.19,	0.388,		/*13*/	0.603,	0.85,	1.164,	1.668,	-777,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*14*/	-1.703,	-1.208,	-0.901,	-0.662,	-0.456,	-0.267,	-0.088,	0.088,	0.267,		/*14*/	0.456,	0.662,	0.901,	1.208,	1.703,	-777,	-777,	-777,	-777,	-777,	-777},
	{/*15*/	-1.736,	-1.248,	-0.948,	-0.715,	-0.516,	-0.335,	-0.165,	0,		0.165,		/*15*/	0.335,	0.516,	0.715,	0.948,	1.248,	1.736,	-777,	-777,	-777,	-777,	-777},
	{/*16*/	-1.766,	-1.285,	-0.99,	-0.763,	-0.57,	-0.396,	-0.234,	-0.077,	0.077,		/*16*/	0.234,	0.396,	0.57,	0.763,	0.99,	1.285,	1.766,	-777,	-777,	-777,	-777},
	{/*17*/	-1.794,	-1.319,	-1.029,	-0.807,	-0.619,	-0.451,	-0.295,	-0.146,	0,			/*17*/	0.146,	0.295,	0.451,	0.619,	0.807,	1.029,	1.319,	1.794,	-777,	-777,	-777},
	{/*18*/	-1.82,	-1.35,	-1.066,	-0.848,	-0.665,	-0.502,	-0.351,	-0.208,	-0.069,		/*18*/	0.069,	0.208,	0.351,	0.502,	0.665,	0.848,	1.066,	1.35,	1.82,	-777,	-777},
	{/*19*/	-1.844,	-1.38,	-1.099,	-0.886,	-0.707,	-0.548,	-0.402,	-0.264,	-0.131,		/*19*/	0,		-0.131,	0.264,	0.402,	0.548,	0.707,	0.886,	1.099,	1.38,	1.844,	-777},
	{/*20*/	-1.867,	-1.408,	-1.131,	-0.921,	-0.745,	-0.59,	-0.448,	-0.315,	-0.187,		/*20*/	-0.062,	0.062,	0.187,	0.315,	0.448,	0.59,	0.745,	0.921,	1.131,	1.408,	1.867}
	//		1		2		3		4		5		6		7		8		9					10		11		12		13		14		15		16		17		18		19		20
	};
//    Dim table(3 To 20, 1 To 20) As Double
//
//    'N , RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8, RANK9, RANK10, RANK11, RANK12, RANK13, RANK14, RANK15, RANK16, RANK17, RANK18, RANK19, RANK20
//    '3,
//    table(3, 1) = -0.846: table(3, 2) = 0#: table(3, 3) = 0.846
//    table(4, 1) = -1.029: table(4, 2) = -0.297: table(4, 3) = 0.297: table(4, 4) = 1.029
//    table(5, 1) = -1.163: table(5, 2) = -0.495: table(5, 3) = 0#: table(5, 4) = 0.495: table(5, 5) = 1.163
//    table(6, 1) = -1.267: table(6, 2) = -0.642: table(6, 3) = -0.202: table(6, 4) = 0.202: table(6, 5) = 0.642: table(6, 6) = 1.267
//    table(7, 1) = -1.352: table(7, 2) = -0.757: table(7, 3) = -0.353: table(7, 4) = 0#: table(7, 5) = 0.353: table(7, 6) = 0.757: table(7, 7) = 1.352
//    table(8, 1) = -1.424: table(8, 2) = -0.852: table(8, 3) = -0.473: table(8, 4) = -0.153: table(8, 5) = 0.153: table(8, 6) = 0.473: table(8, 7) = 0.852: table(8, 8) = 1.424
//    table(9, 1) = -1.485: table(9, 2) = -0.932: table(9, 3) = -0.572: table(9, 4) = -0.275: table(9, 5) = 0#: table(9, 6) = 0.275: table(9, 7) = 0.572: table(9, 8) = 0.932: table(9, 9) = 1.485
//    table(10, 1) = -1.539: table(10, 2) = -1.001: table(10, 3) = -0.656: table(10, 4) = -0.376: table(10, 5) = -0.123: table(10, 6) = 0.123: table(10, 7) = 0.376: table(10, 8) = 0.656: table(10, 9) = 1.001: table(10, 10) = 1.539
//    table(11, 1) = -1.586: table(11, 2) = -1.062: table(11, 3) = -0.729: table(11, 4) = -0.462: table(11, 5) = -0.225: table(11, 6) = 0#: table(11, 7) = 0.225: table(11, 8) = 0.462: table(11, 9) = 0.729: table(11, 10) = 1.062: table(11, 11) = 1.586
//    table(12, 1) = -1.629: table(12, 2) = -1.116: table(12, 3) = -0.793: table(12, 4) = -0.537: table(12, 5) = -0.312: table(12, 6) = -0.103: table(12, 7) = 0.103: table(12, 8) = 0.312: table(12, 9) = 0.537: table(12, 10) = 0.793: table(12, 11) = 1.116: table(12, 12) = 1.629
//    table(13, 1) = -1.668: table(13, 2) = -1.164: table(13, 3) = -0.85: table(13, 4) = -0.603: table(13, 5) = -0.388: table(13, 6) = -0.19: table(13, 7) = 0#: table(13, 8) = 0.19: table(13, 9) = 0.388: table(13, 10) = 0.603: table(13, 11) = 0.85: table(13, 12) = 1.164: table(13, 13) = 1.668
//    table(14, 1) = -1.703: table(14, 2) = -1.208: table(14, 3) = -0.901: table(14, 4) = -0.662: table(14, 5) = -0.456: table(14, 6) = -0.267: table(14, 7) = -0.088: table(14, 8) = 0.088: table(14, 9) = 0.267: table(14, 10) = 0.456: table(14, 11) = 0.662: table(14, 12) = 0.901: table(14, 13) = 1.208: table(14, 14) = 1.703
//    table(15, 1) = -1.736: table(15, 2) = -1.248: table(15, 3) = -0.948: table(15, 4) = -0.715: table(15, 5) = -0.516: table(15, 6) = -0.335: table(15, 7) = -0.165: table(15, 8) = 0#: table(15, 9) = 0.165: table(15, 10) = 0.335: table(15, 11) = 0.516: table(15, 12) = 0.715: table(15, 13) = 0.948: table(15, 14) = 1.248: table(15, 15) = 1.736
//    table(16, 1) = -1.766: table(16, 2) = -1.285: table(16, 3) = -0.99: table(16, 4) = -0.763: table(16, 5) = -0.57: table(16, 6) = -0.396: table(16, 7) = -0.234: table(16, 8) = -0.077: table(16, 9) = 0.077: table(16, 10) = 0.234: table(16, 11) = 0.396: table(16, 12) = 0.57: table(16, 13) = 0.763: table(16, 14) = 0.99: table(16, 15) = 1.285: table(16, 16) = 1.766
//    table(17, 1) = -1.794: table(17, 2) = -1.319: table(17, 3) = -1.029: table(17, 4) = -0.807: table(17, 5) = -0.619: table(17, 6) = -0.451: table(17, 7) = -0.295: table(17, 8) = -0.146: table(17, 9) = 0#: table(17, 10) = 0.146: table(17, 11) = 0.295: table(17, 12) = 0.451: table(17, 13) = 0.619: table(17, 14) = 0.807: table(17, 15) = 1.029: table(17, 16) = 1.319: table(17, 17) = 1.794
//    table(18, 1) = -1.82: table(18, 2) = -1.35: table(18, 3) = -1.066: table(18, 4) = -0.848: table(18, 5) = -0.665: table(18, 6) = -0.502: table(18, 7) = -0.351: table(18, 8) = -0.208: table(18, 9) = -0.069: table(18, 10) = 0.069: table(18, 11) = 0.208: table(18, 12) = 0.351: table(18, 13) = 0.502: table(18, 14) = 0.665: table(18, 15) = 0.848: table(18, 16) = 1.066: table(18, 17) = 1.35: table(18, 18) = 1.82
//    table(19, 1) = -1.844: table(19, 2) = -1.38: table(19, 3) = -1.099: table(19, 4) = -0.886: table(19, 5) = -0.707: table(19, 6) = -0.548: table(19, 7) = -0.402: table(19, 8) = -0.264: table(19, 9) = -0.131: table(19, 10) = 0#: table(19, 11) = 0.131: table(19, 12) = 0.264: table(19, 13) = 0.402: table(19, 14) = 0.548: table(19, 15) = 0.707: table(19, 16) = 0.886: table(19, 17) = 1.099: table(19, 18) = 1.38: table(19, 19) = 1.844
//    '20,
//    table(20, 1) = -1.867: table(20, 2) = -1.408: table(20, 3) = -1.131: table(20, 4) = -0.921: table(20, 5) = -0.745: table(20, 6) = -0.59: table(20, 7) = -0.448: table(20, 8) = -0.315: table(20, 9) = -0.187: table(20, 10) = -0.062: table(20, 11) = 0.062: table(20, 12) = 0.187: table(20, 13) = 0.315: table(20, 14) = 0.448: table(20, 15) = 0.59: table(20, 16) = 0.745: table(20, 17) = 0.921: table(20, 18) = 1.131: table(20, 19) = 1.408: table(20, 20) = 1.867
//
	if ( N < 1 || N > 20 )
	{
		throw new InvalidArgumentException("Rankit N must be in the range [1,20]");
	}
	if ( rank < 1 || rank > 20 )
	{
		throw new InvalidArgumentException("Rankit rank  must be in the range [1,20]");
	}
	double prob = table[ rank-1 ][ N-1 ];
	ASSERT( prob != -777 && prob < 2 && prob > -2 );
	return prob;
}

static double _AvgRankits(struct _SPair *pairs, int N, int x)
{
	ASSERT_MEM( pairs, N * sizeof(struct _SPair) );
	double sum = 0;
	int ncount = 0;
	ASSERT_PTR(&pairs[x]);
	double target = pairs[x].val;

	for (int i = 0; i < N; i++)
	{
		if (pairs[i].val == target)
		{
			/* average the ranks of the duplicates*/
			ncount = 1;
			sum = _Rankits(N, i + 1);
			while (i < N && pairs[i].val == pairs[i + 1].val)
			{
				ncount = ncount + 1;
				i = i + 1;
				sum = sum + _Rankits(N, i + 1);
			}
			return sum / ncount;
		}
	}
	throw new Exception("Internal error in _AvgRankits");
}

SampleTest::SampleTest(const Sample& s1, const Sample& s2)
: m_s1(s1), m_s2(s2)
{
}

SampleTest::SampleTest(const List<double>& s1, const List<double>& s2)
: m_s1(s1), m_s2(s2)
{
}

SampleTest::SampleTest(const Vector<double>& s1, const Vector<double>& s2)
: m_s1(s1), m_s2(s2)
{
}

SampleTest::SampleTest(const Array<double>& s1, const Array<double>& s2)
: m_s1(s1), m_s2(s2)
{
}

SampleTest::SampleTest(const SampleTest& t)
: m_s1(t.m_s1), m_s2(t.m_s2)
{
}

SampleTest::~SampleTest()
{
}

SampleTest& SampleTest::operator =(const SampleTest& t)
{
	m_s1 = t.m_s1;
	m_s2 = t.m_s2;

	return *this;
}

/** @brief Student's t-test fo signifcantly different means
  *
  */
double SampleTest::StudentsT(double *prob)
{
	double var1 = m_s1.AverageVariance();
	double var2 = m_s2.AverageVariance();

	int df = m_s1.N() + m_s2.N() - 2;

	double svar = ((m_s1.N() - 1) * var1 + (m_s2.N() - 1) * var2) / df;

	double t = (m_s1.Mean() - m_s2.Mean()) / Math::Sqrt(svar * (1.0 / m_s1.N() + 1.0 / m_s2.N()));
	if ( NULL != prob )
	{
		*prob = Betai(0.5 * df, 0.5, df / (df + t * t));
	}

	return t;
}

/** @brief returns the incomplete beta funciton Ix(a, b)
  *
  */
double SampleTest::Betai(double a, double b, double x)
{
    double bt;
    
    if( x < 0 || x > 1 )
	{
		throw new InvalidArgumentException("x out of range in betai");
	}
    
	if( x == 0 || x == 1 )
	{
		bt = 0;
	}
	else
	{
		bt = Math::Exp(Gammaln(a + b) - Gammaln(a) - Gammaln(b) + a * Math::Log(x) + b * Math::Log(1 - x));
	}

	if( x < (a + 1) / (a + b + 2) )
	{
		return bt * Betacf(a, b, x) / a;
	}
	else
	{
		return 1 - bt * Betacf(b, a, 1 - x) / b;
	}
}

/** @brief returns the value ln(tao(?)(xx)) for xx > 0
  *
  */
double SampleTest::Gammaln(double xx)
{
	double cof[6];

	cof[0] = 76.1800917294715;
	cof[1] = -86.5053203294168;
	cof[2] = 24.0140982408309;
	cof[3] = -1.23173957245015;
	cof[4] = 1.20865097386618E-03;
	cof[5] = -5.395239384953E-06;

	double y = xx;
	double x = xx;
	double tmp = x + 5.5;
	tmp = tmp - (x + 0.5) * Math::Log(tmp);
	double ser = 1.00000000019001;

	for (int j = 0; j < 6; j++)
	{
		y = y + 1;
		ser = ser + cof[j] / y;
	}
	return -tmp + Math::Log(2.506628274631 * ser / x);
}


double SampleTest::Gamma(double d)
{
	return Math::Exp(Gammaln(d));
}

/** @brief evaluate continued fration for incomplete beta funciton by modified lentz's method (5.2)
  *
  */
double SampleTest::Betacf(double a, double b, double x)
{
	const double FPMIN = 1E-30;
	const double EPS = 0.0000003;
	const int MAXIT = 200;

	double qab = a + b;
	double qap = a + 1;
	double qam = a - 1;
	double c = 1;
	double d = 1 - qab * x / qap;
	if( Math::Abs(d) < FPMIN )
	{
		d = FPMIN;
	}

	d = 1 / d;
	double h = d;
	
	/* 100 is the maximum iterations */
	int m;
	for (m = 1; m <= MAXIT; m++ )
	{
		int m2 = 2 * m;
		double aa = m * (b - m) * x / ((qam + m2) * (a + m2));
		d = 1 + aa * d;
		if (Math::Abs(d) < FPMIN)
		{
			d = FPMIN;
		}
		c = 1 + aa / c;
		if( Math::Abs(c) < FPMIN )
		{
			c = FPMIN;
		}
		d = 1 / d;
		h = h * d * c;
		aa = -(a + m) * (qab + m) * x / ((a + m2) * (qap + m2));
		d = 1 + aa * d;
		if (Math::Abs(d) < FPMIN)
		{
			d = FPMIN;
		}
		c = 1 + aa / c;
		if (c < FPMIN)
		{
			c = FPMIN;
		}
		d = 1 / d;
		double del = d * c;
		h = h * del;
		if (Math::Abs(del - 1) < EPS)
		{
			/* finished */
			break;
		}
	}
	if (m == MAXIT)
	{
		throw new InvalidArgumentException("betacf a or b too big, or MAXIt too small");
	}
	return h;
}

/** @brief Small values of prob indicate that the two sets have significantly different variances.
  *
  * Returns the probability
  */
double SampleTest::FTest(double *fstat)
{
	double df1;
	double df2;

	double avgVar1 = 0;
	double avgVar2 = 0;
	double ep = 0;
	double s;

	int j;

	ASSERT(NULL != fstat);

	for( j = 0; j < m_s1.N(); j++ )
	{
		s = m_s1.Item(j) - m_s1.Mean();
		ep = ep + s;
		avgVar1 = avgVar1 + s * s;
	}
	avgVar1 = (avgVar1 - ep * ep / m_s1.N()) / (m_s1.Mean() - 1);

	for( j = 0; j < m_s2.N(); j++ )
	{
		s = m_s2.Item(j) - m_s2.Mean();
		ep = ep + s;
		avgVar2 = avgVar2 + s * s;
	}
	avgVar2 = (avgVar2 - ep * ep / m_s2.N()) / (m_s2.N() - 1);

	if (avgVar1 > avgVar2)
	{
		if (avgVar2 == 0)
		{
			*fstat = 0;
		}
		else
		{
			*fstat = avgVar1 / avgVar2;
		}
		df1 = m_s1.N(); - 1;
		df2 = m_s2.N() - 1;
	}
	else
	{
		if (avgVar1 == 0)
		{
			*fstat = 0;
		}
		else
		{
			*fstat = avgVar2 / avgVar1;
		}
		df1 = m_s2.N(); - 1;
		df2 = m_s1.N() - 1;
	}

	double prob = 2 * Betai(0.5 * df2, 0.5 * df1, df2 / (df2 + df1 * *fstat));
	if (prob > 1)
	{
		prob = 2 - prob;
	}
	ASSERT(prob <= 1);

	return prob;
}

double SampleTest::ApproximateTTest(double *prob, double *df)
{    
	ASSERT(NULL != prob && NULL != df);

	double var1 = m_s1.Variance();
	double var2 = m_s2.Variance();

	double t = (m_s1.Mean() - m_s2.Mean()) / Math::Sqrt(var1 / m_s1.N() + var2 / m_s2.N());
	*df = Math::Pow((var1 / m_s1.N() + var2 / m_s2.N()), 2) / (Math::Pow((var1 / m_s1.N()), 2) / (m_s1.N() - 1) + Math::Pow((var2 / m_s2.N()), 2) / (m_s2.N() - 1));
	*prob = Betai(0.5 * *df, 0.5 * *df, *df / (*df + t * t));

	return t;
}

double SampleTest::OneSampleTTest()
{
    if( m_s1.Variance() == 0 )
	{
		/** @BUG I think this is wrong */
		return 4;
	}
    else
	{
		return (m_s1.Mean() - m_s2.Mean()) / (Math::Sqrt(m_s1.Variance() / m_s1.N()));
	}
}

//'{This program performs the Brown-Forsythe & Levene Tests for homogeneity of
//'variances for the two group case.
//'Levene and Brown-Forsythe tests for homogeneity of variances (HOV)
//'A important assumption in the t-test for mean differences is that the variances
//'in the two groups are equal (homogeneous). Two powerful and commonly used tests
//'of this assumption are the Levene test and the Brown-Forsythe modification of
//'this test. However, it is important to realize that (1) the homogeneity of
//'variances assumption is usually not as crucial as other assumptions for the
//'t-test for mean differences, in particular in the case of equal n, and (2) that
//'the tests described below are not necessarily very robust themselves (e.g.,
//'Glass and Hopkins, 1996, p. 436, call these tests "fatally flawed" see also the
//'description of these tests below). If you are concerned about a violation of the
//'HOV assumption, it is always advisable to repeat the key analyses using
//'nonparametric methods.
//'Levene 's test (homogeneity of variances): For each dependent variable, a t-test
//'for mean differences is performed on the absolute deviations of values from the
//'respective group means. If the Levene test is statistically significant, then
//'the hypothesis of homogeneous variances should be rejected.
//'Brown & Forsythe's test (homogeneity of variances): Recently, some authors
//'(e.g., Glass and Hopkins, 1996) have called into question the power of the
//'Levene test for unequal variances. Specifically, the absolute deviation (from
//'the group means) scores can be expected to be highly skewed thus, the normality
//'assumption for t-test for mean differences of those absolute deviation scores is
//'usually violated. This poses a particular problem when there is unequal n in the
//'two groups that are to be compared. A more robust test that is very similar to
//'the Levene test has been proposed by Brown and Forsythe (1974). Instead of
//'performing the t-test for mean differences on the deviations from the mean, one
//'can perform the analysis on the deviations from the group medians. Olejnik and
//'Algina (1987) have shown that this test will give quite accurate error rates
//'even when the underlying distributions for the raw scores deviate significantly
//'from the normal distribution. However, as Glass and Hopkins (1996, p. 436) have
//'pointed out, both the Levene test as well as the Brown-Forsythe modification
//'suffer from what those authors call a "fatal flaw," namely, that both tests
//'themselves rely on the homogeneity of variances assumption (of the absolute
//'deviations from the means or medians) and hence, it is not clear how robust
//'these tests are themselves in the presence of significant variance heterogeneity
//'and unequal n.
//'Program written, modified, or edited at StatSoft, Inc.}

/** @brief large values of prob indiccate that the two sets have significantly different variances
  */
double SampleTest::Levene(double *prob)
{
	if( m_s1.N() == 0 || m_s2.N() == 0 )
	{
		throw new Exception("No data in samples");
	}
	
	double *z1 = new double[m_s1.N()];
	double *z2 = new double[m_s2.N()];
	
	double med1 = m_s1.Median();
	double med2 = m_s2.Median();

	int j;
	double sum1 = 0;
	double zmean1 = 0;
	double zmean2 = 0;

	for( j = 0; j < m_s1.N(); j++)
	{
		z1[j] = Math::Abs(m_s1.Item(j) - m_s1.Mean());
		sum1 = sum1 + m_s1.Item(j);
	}
	zmean1 = sum1 / m_s1.N();

	sum1 = 0;
	for( j = 0; j < m_s2.N(); j++)
	{
		z2[j] = Math::Abs(m_s2.Item(j));
		sum1 = sum1 + m_s2.Item(j);
	}
	zmean2 = sum1 / m_s2.N();
	sum1 = 0;

	double sumsqr1 = 0;
	double sumsqr2 = 0;
	double sum2 = 0;

	for( j = 0; j < m_s1.N(); j++ )
	{
		sum1 = sum1 + z1[j];
		sumsqr1 = sumsqr1 + z1[j] * z1[j];
	}

	for( j = 0; j < m_s2.N(); j++)
	{
		sum2 = sum2 + z2[j];
		sumsqr2 = sumsqr2 + z2[j] * z2[j];
	}

	double sumsqr = 0;
	double SST = 0;
	double SSTRT = 0;
	double SSE = 0;
	double f = 0;

	sumsqr = (sum1 + sum2) * (sum1 + sum2);
	SST = sumsqr1 + sumsqr2 - sumsqr / (m_s1.N() + m_s2.N());
	SSTRT = (sum1 * sum1) / m_s1.N() + (sum2 * sum2) / m_s2.N() - sumsqr / (m_s1.N() + m_s2.N());
	SSE = SST - SSTRT;
	//degrees of freedom = Count + B.Count - 2
	f = SSTRT / (SSE / m_s1.N() + m_s2.N() - 2);
	*prob = 1 - FDistr(f, 1, m_s1.N() + m_s2.N() - 2);

	delete[] z1;
	delete[] z2;
	
	return f;
}

double SampleTest::LeveneAlternate(double *tprob)
{
/*
'  This is the alternate x2 < x1 way.
'
    ' 1. the absolute value of the difference between each replicate and the
    '    mean for its repective treatment is calculated.
    
    ' 2.  A 2-tailed t-test is performed between the test treatment absolute
    '     differences and the reference treatment absolute differences.
*/
	List<double> vals;    
    int i;
    
    for (i = 0; i < m_s1.N(); i++ )
	{
		vals.Add( Math::Abs(m_s1.Item(i) - m_s1.Mean()) );
	}
    Sample ds1(vals);
    
	vals.Clear();
    for (i = 0; i < m_s2.N(); i++ )
	{
		vals.Add( Math::Abs(m_s2.Item(i) - m_s2.Mean()) );
	}
    Sample ds2(vals);
    
	SampleTest tst(ds1, ds2);

	double t = tst.StudentsT(tprob);
	return Math::Abs(t);
}

double SampleTest::FDistr(double x, double v, double w)
{
	return (Gamma((v + w) / 2)) / (Gamma(v / 2) * Gamma(w / 2)) * Math::Pow((v / w), (v / 2)) * Math::Pow(x, (v / 2)) * Math::Pow((1 + (v / w) * x), (-(v + w) / 2));
}

/** @brief Shapiro-Wilk W Test of Normality for Small Samples 
 *   THIS IS THE SHAPIRO-WILK TEST FOR SMALL SAMPLES
 *   THE SAMPLE SIZE MUST BE LESS THAN OR EQUAL TO 50
 *   TEST13 SW(N)  =\002,f10.4
 */
double SampleTest::WTestSmall()
{
	const double *a = NULL;
	int N = m_s1.N() + m_s2.N();
	if (N > 50)
	{
		throw new Exception("This version of the w-test only handles up to 50 data points");
	}

	static const double a3[] = {0.7071};
	static const double a4[] = { 0.6872, 0.1677, -1 };
	static const double a5[] = { 0.6646, 0.2413, -1 };
	static const double a6[] = { 0.6431, 0.2806, 0.0875, -1 };
	static const double a7[] = { 0.6233, 0.3031, 0.1401, -1 };
	static const double a8[] = { 0.6052, 0.3164, 0.1743, 0.0561, -1 };
	static const double a9[] = { 0.5888, 0.3244, 0.1976, 0.0947, -1 };;
	static const double a10[] = { 0.5739, 0.3291, 0.2141, 0.1224, 0.0399, -1 };
	static const double a11[] = { 0.5601, 0.3315, 0.226, 0.1429, 0.0695, -1 };
	static const double a12[] = { 0.5475, 0.3325, 0.2347, 0.1586, 0.0922, 0.0303, -1 };
	static const double a13[] = { 0.5359, 0.3325, 0.2412, 0.1707, 0.1099, 0.0539, -1 };
	static const double a14[] = { 0.5251,0.3318,0.246,0.1802,0.124,0.0727,0.024,-1 };
	static const double a15[] = { 0.515,0.3306,0.2495,0.1878,0.1353,0.088,0.0433,-1 };
	static const double a16[] = { 0.5056,0.329,0.2521,0.1939,0.1447,0.1005,0.0593,0.0196,-1 };
	static const double a17[] = { 0.4968,0.3273,0.254,0.1988,0.1524,0.1109,0.0725,0.0359,-1 };
	static const double a18[] = { 0.4886,0.3253,0.2553,0.2027,0.1587,0.1197,0.0837,0.0496,0.0163,-1 };
	static const double a19[] = { 0.4808,0.3232,0.2561,0.2059,0.1641,0.1271,0.0932,0.0612,0.0303,-1 };
	static const double a20[] = { 0.4734,0.3211,0.2565,0.2085,0.1686,0.1334,0.1013,0.0711,0.0422,0.014,-1 };
	static const double a21[] = { 0.4643,0.3185,0.2578,0.2119,0.1736,0.1399,0.1092,0.0804,0.053,0.0263,-1 };
	static const double a22[] = { 0.459,0.3156,0.2571,0.2131,0.1764,0.1443,0.115,0.0878,0.0618,0.0368,0.0122,-1 };
	static const double a23[] = { 0.4542,0.3126,0.2563,0.2139,0.1787,0.148,0.1201,0.0941,0.0696,0.0459,0.0228,-1 };
	static const double a24[] = { 0.4493,0.3098,0.2554,0.2145,0.1807,0.1512,0.1245,0.0997,0.0764,0.0539,0.0321,0.0107,-1 };
	static const double a25[] = { 0.445,0.3069,0.2543,0.2148,0.1822,0.1539,0.1283,0.1046,0.0823,0.061,0.0403,0.02,-1 };
	static const double a26[] = { 0.4407,0.3043,0.2533,0.2151,0.1836,0.1563,0.1316,0.1089,0.0876,0.0672,0.0476,0.0284,0.0094,-1 };
	static const double a27[] = { 0.4366,0.3018,0.2522,0.2152,0.1848,0.1584,0.1346,0.1128,0.0923,0.0728,0.054,0.0358,0.0178,-1 };
	static const double a28[] = { 0.4328,0.2992,0.251,0.2151,0.1857,0.1601,0.1372,0.1162,0.0965,0.0778,0.0598,0.0424,0.0253,0.0084,-1 };
	static const double a29[] = { 0.4291,0.2968,0.2499,0.215,0.1864,0.1616,0.1395,0.1192,0.1002,0.0822,0.065,0.0483,0.032,0.0159,-1 };
	static const double a30[] = { 0.4254,0.2944,0.2487,0.2148,0.187,0.163,0.1415,0.1219,
		0.1036,0.0862,0.0697,0.0537,0.0381,0.0227,0.0076,-1 };
	static const double a31[] = { 0.422,0.2921,0.2475,0.2145,0.1874,0.1641,0.1433,0.1243,0.1066,0.0899,
		0.0739,0.0585,0.0435,0.0289,0.0144,-1 };
	static const double a32[] = { 0.4188,0.2898,0.2463,0.2141,0.1878,0.1651,0.1449,0.1265,0.1093,0.0931,
		0.0777,0.0629,0.0485,0.0344,0.0206,0.0068,-1 };
	static const double a33[] = { 0.4156,0.2876,0.2451,0.2137,0.188,0.166,0.1463,0.1284,0.1118,0.0961,0.0812,
		0.0669,0.053,0.0395,0.0262,0.0131,-1 };
	static const double a34[] = { 0.4127,0.2854,0.2439,0.2132,0.1882,0.1667,0.1475,0.1301,0.114,0.0988,0.0844,
		0.0706,0.0572,0.0441,0.0314,0.0187,0.0062,-1 };
	static const double a35[] = { 0.4096,0.2834,0.2427,0.2127,0.1883,0.1673,0.1487,0.1317,0.116,0.1013,0.0873,
		0.0739,0.061,0.0484,0.0361,0.0239,0.0119,-1 };
	static const double a36[] = { 0.4068,0.2813,0.2415,0.2121,0.1883,0.1678,0.1496,0.1331,0.1179,0.1036,0.09,
		0.077,0.0645,0.0523,0.0404,0.0287,0.0172,0.0057,-1 };
	static const double a37[] = { 0.404,0.2794,0.2403,0.2116,0.1883,0.1683,0.1505,0.1344,0.1196,0.1056,0.0924,
		0.0798,0.0677,0.0559,0.0444,0.0331,0.022,0.011,-1 };
	static const double a38[] = { 0.4015,0.2774,0.2391,0.211,0.1881,0.1686,0.1513,0.1356,0.1211,0.1075,0.0947,
		0.0824,0.0706,0.0592,0.0481,0.0372,0.0264,0.0158,0.0053,-1 };
	static const double a39[] = { 0.3989,0.2755,0.238,0.2104,0.188,0.1689,0.152,0.1366,0.1225,0.1092,0.0967,
		0.0848,0.0733,0.0622,0.0515,0.0409,0.0305,0.0203,0.0101,-1 };
	static const double a40[] = { 0.3964,0.2737,0.2368,0.2098,0.1878,0.1691,0.1526,0.1376,0.1237,0.1108,
		0.0986,0.087,0.0759,0.0651,0.0546,0.0444,0.0343,0.0244,0.0146,0.0049,-1 };
	static const double a41[] = { 0.394,0.2719,0.2357,0.2091,0.1876,0.1693,0.1531,0.1384,0.1249,0.1123,
		0.1004,0.0891,0.0782,0.0677,0.0575,0.0476,0.0379,0.0283,0.0188,0.0094,-1 };
	static const double a42[] = { 0.3917,0.2701,0.2345,0.2085,0.1874,0.1694,0.1535,0.1392,0.1259,
		0.1136,0.102,0.0909,0.0804,0.0701,0.0602,0.0506,0.0411,0.0318,0.0227,0.0136,0.0045,-1 };
	static const double a43[] = { 0.3894,0.2684,0.2334,0.2078,0.1871,0.1695,0.1539,0.1398,0.1269,0.1149,
		0.1035,0.0927,0.0824,0.0724,0.0628,0.0534,0.0442,0.0352,0.0263,0.0175,0.0087,-1 };
	static const double a44[] = { 0.3872,0.2667,0.2323,0.2072,0.1868,0.1695,0.1542,0.1405,0.1278,0.116,
		0.1049,0.0943,0.0842,0.0745,0.0651,0.056,0.0471,0.0383,0.0296,0.0211,0.0126,0.0042,-1 };
	static const double a45[] = { 0.385,0.2651,0.2313,0.2065,0.1865,0.1695,0.1545,0.141,0.1286,0.117,0.1062,
		0.0959,0.086,0.0765,0.0673,0.0584,0.0497,0.0412,0.0328,0.0245,0.0163,0.0081,-1 };
	static const double a46[] = { 0.383,0.2635,0.2302,0.2058,0.1862,0.1695,0.1548,0.1415,0.1293,0.118,0.1073,
		0.0972,0.0876,0.0783,0.0694,0.0607,0.0522,0.0439,0.0357,0.0277,0.0197,0.0118,0.0039,-1 };
	static const double a47[] = { 0.3808,0.262,0.2291,0.2052,0.1859,0.1695,0.155,0.142,0.13,0.1189,0.1085,0.0986,
		0.0892,0.0801,0.0713,0.0628,0.0546,0.0465,0.0385,0.0307,0.0229,0.0153,0.0076,-1 };
	static const double a48[] = { 0.3789,0.2604,0.2281,0.2045,0.1855,0.1693,0.1551,0.1423,0.1306,0.1197,0.1095,
		0.0998,0.0906,0.0817,0.0731,0.0648,0.0568,0.0489,0.0411,0.0335,0.0259,0.0185,0.0111,0.0037,-1 };
	static const double a49[] = { 0.377,0.2589,0.2271,0.2038,0.1851,0.1692,0.1553,0.1427,0.1312,0.1205,0.1105,0.101,
		0.0919,0.0832,0.0748,0.0667,0.0588,0.0511,0.0436,0.0361,0.0288,0.0215,0.0143,0.0071,-1 };
	static const double a50[] = { 0.3751,0.2574,0.226,0.2032,0.1847,0.1691,0.1554,0.143,0.1317,0.1212,0.1113,
	0.102,0.0932,0.0846,0.0764,0.0685,0.0608,0.0532,0.0459,0.0386,0.0314,0.0244,0.0174,0.0104,0.0035,-1 };

	switch ( N )
	{
	case 3:
		a = &a3[0];
		break;
	case 4:
		a = &a4[0];
		break;
	case 5:
		a = &a5[0];
		break;
	case 6:
		a = &a6[0];
		break;
	case 7:
		a = &a7[0];
		break;
	case 8:
		a = &a8[0];
		break;
	case 9:
		a = &a9[0];
		break;
	case 10:
		a = &a10[0];
		break;
	case 11:
		a = &a11[0];
		break;
	case 12:
		a = &a12[0];
		break;
	case 13:
		a = &a13[0];
		break;
	case 14:
		a = &a14[0];
		break;
	case 15:
		a = &a15[0];
		break;
	case 16:
		a = &a16[0];
		break;
	case 17:
		a = &a17[0];
		break;
	case 18:
		a = &a18[0];
		break;
	case 19:
		a = &a19[0];
		break;
	case 20:
		a = &a20[0];
		break;
	case 21:
		a = &a21[0];
		break;
	case 22:
		a = &a22[0];
		break;
	case 23:
		a = &a23[0];
		break;
	case 24:
		a = &a24[0];
		break;
	case 25:
		a = &a25[0];
		break;
	case 26:
		a = &a26[0];
		break;
	case 27:
		a = &a27[0];
		break;
	case 28:
		a = &a28[0];
		break;
	case 29:
		a = &a29[0];
		break;
	case 30:
		a = &a30[0];
		break;
	case 31:
		a = &a31[0];
		break;
	case 32:
		a = &a32[0];
		break;
	case 33:
		a = &a33[0];
		break;
	case 34:
		a = &a34[0];
		break;
	case 35:
		a = &a35[0];
		break;
	case 36:
		a = &a36[0];
		break;
	case 37:
		a = &a37[0];
		break;
	case 38:
		a = &a38[0];
		break;
	case 39:
		a = &a39[0];
		break;
	case 40:
		a = &a40[0];
		break;
	case 41:
		a = &a41[0];
		break;
	case 42:
		a = &a42[0];
		break;
	case 43:
		a = &a43[0];
		break;
	case 44:
		a = &a44[0];
		break;
	case 45:
		a = &a45[0];
		break;
	case 46:
		a = &a46[0];
		break;
	case 47:
		a = &a47[0];
		break;
	case 48:
		a = &a48[0];
		break;
	case 49:
		a = &a49[0];
		break;
	case 50:
		a = &a50[0];
		break;
	default:
		throw new Exception("Unsupported value of N in w-test small");
	}

	double sumb = 0;
	double sumx = 0;
	double sumx2 = 0;

	int i;
	int k = N / 2;

	double *x1 = new double[N];

	// put the data from both sets into an array
	for (i = 0; i < m_s1.N(); i++)
	{
		x1[i] = m_s1.Item(i) - m_s1.Mean();
	}

	i = m_s1.N();
	for (int j = 0; j < m_s2.N(); j++)
	{
		x1[i] = m_s2.Item(j) - m_s2.Mean();
		i++;
	}

	_Sort(N, x1);
	ASSERT_MEM( x1, N * sizeof(double) );

	for( i = 0; i < N; i++ )
	{
		sumx = sumx + x1[i];
	}
	sumx = sumx / N;

	for (i = 0; i < N; i++)
	{
		sumx2 = sumx2 + (x1[i] - sumx) * (x1[i] - sumx);
	}

	if (((N - 1) % 2) == 0 )
	{
		for(i = 0; i < k; i++)
		{
			sumb = sumb + a[i] * (x1[N - (i + 1)] - x1[i]);
		}
	}
	else
	{
		for(i = 0; i <= k; i++)
		{
			sumb = sumb + a[i] * (x1[N - (i + 1)] - x1[i]);
		}
	}

	delete [] x1;

	if (sumx2 == 0)
	{
		return 0;
	}
	else
	{
		return sumb * sumb / sumx2;
	}
}

/** @brief mann-whitney
  */
double SampleTest::UStat()
{    
	int N = m_s1.N() + m_s2.N();       
	double R1 = _SortSumR1(&m_s1, &m_s2);

	return m_s1.N() * m_s2.N() + (m_s1.N() * (m_s1.N() + 1)) / 2 - R1;
}

/** @brief mann-whitney
  */
double SampleTest::UStatAlternate()
{
	int N = m_s1.N() + m_s2.N();       
	double R1 = _SortSumR1(&m_s2, &m_s1);
	return m_s1.N() * m_s2.N() + (m_s2.N() * (m_s2.N() + 1)) / 2 - R1;
}

double SampleTest::WTestCriticalValue(int N, double alpha)
{
	int category;

	if ( alpha <= 0.01 )
	{
		category = 1;
	}
	else if ( alpha <= 0.02 )
	{
		category = 2;
	}
	else if ( alpha <= 0.05 )
	{
		category = 3;
	}
	else if ( alpha <= 0.1 )
	{
		category = 4;
	}
	else
	{
		category = 5;
	}
    
	static double table[][50] = {
    //A0_01, A0_02, A0_05, A0_10, A0_50
	/*3*/{0.753,	0.756,	0.767,	0.789,	0.959},
    /*4*/{0.687,	0.707,	0.748,	0.792,	0.935},
    /*5*/{0.686,	0.715,	0.762,	0.806,	0.927},
    /*6*/{0.713,	0.743,	0.788,	0.826,	0.927},
    /*7*/{0.73,		0.76,	0.803,	0.838,	0.928},
    /*8*/{0.749,	0.778,	0.818,	0.851,	0.932},
    /*9*/{0.764,	0.791,	0.829,	0.859,	0.935},
    /*10*/{0.781,	0.806,	0.842,	0.869,	0.938},
    /*11*/{0.792,	0.817,	0.85,	0.876,	0.94},
    /*12*/{0.805,	0.828,	0.859,	0.883,	0.943},
    /*13*/{0.814,	0.837,	0.866,	0.889,	0.945},
    /*14*/{0.825,	0.846,	0.874,	0.895,	0.947},
    /*15*/{0.835,	0.855,	0.881,	0.901,	0.95},
    /*16*/{0.844,	0.863,	0.887,	0.906,	0.952},
    /*17*/{0.851,	0.869,	0.892,	0.91,	0.954},
    /*18*/{0.858,	0.874,	0.897,	0.914,	0.956},
    /*19*/{0.863,	0.879,	0.901,	0.917,	0.957},
    /*20*/{0.868,	0.884,	0.905,	0.92,	0.959},
    /*21*/{0.873,	0.888,	0.908,	0.923,	0.96},
	      {0.878,	0.892,	0.911,	0.926,	0.961},
          {0.881,	0.895,	0.914,	0.928,	0.962},
          {0.884,	0.898,	0.916,	0.93,	0.963},
          {0.888,	0.901,	0.918,	0.931,	0.964},
	/*26*/
          {0.891,	0.904,	0.92,	0.933,	0.965},
          {0.894,	0.906,	0.923,	0.935,	0.965},
          {0.896,	0.908,	0.924,	0.936,	0.966},
          {0.898,	0.91,	0.926,	0.937,	0.966},
          {0.9,		0.912,	0.927,	0.939,	0.967},
	/*31*/
          {0.902,	0.914,	0.929,	0.94,	0.967},
          {0.904,	0.915,	0.93,	0.941,	0.968},
          {0.906,	0.917,	0.931,	0.942,	0.968},
          {0.908,	0.919,	0.933,	0.943,	0.969},
          {0.91,	0.92,	0.934,	0.944,	0.969},
	/*36*/
          {0.912,	0.922,	0.935,	0.945,	0.97},
          {0.914,	0.924,	0.936,	0.946,	0.97},
          {0.916,	0.925,	0.938,	0.947,	0.971},
          {0.917,	0.927,	0.939,	0.948,	0.971},
          {0.919,	0.928,	0.94,	0.949,	0.972},
	/*41*/
          {0.92,	0.929,	0.941,	0.95,	0.972},
          {0.922,	0.93,	0.942,	0.951,	0.972},
          {0.923,	0.932,	0.943,	0.951,	0.973},
          {0.924,	0.933,	0.944,	0.952,	0.973},
          {0.926,	0.934,	0.945,	0.953,	0.973},
	/*46*/
          {0.927,	0.935,	0.945,	0.953,	0.974},
          {0.928,	0.936,	0.946,	0.954,	0.974},
          {0.929,	0.937,	0.947,	0.954,	0.974},
          {0.929,	0.937,	0.947,	0.955,	0.974},
          {0.93,	0.938,	0.947,	0.955,	0.974}
	};
	
	return table[category][N-1];
}

double SampleTest::TCriticalValueAlternate(int N, double alpha)
{
	int alphacat;

	if ( N < 1 || N > 50 )
	{
		throw new InvalidArgumentException("N for critical value must in the range [1,50]");
	}

	// get a table column for the alpha level
	if (alpha <= 0.025)
	{
		alphacat = 3;
	}
	else if( alpha <= 0.05 )
	{
		alphacat = 2;
	}
	else if (alpha <= 0.1)
	{
		alphacat = 1;
	}
	else
	{
		alphacat = 0;
	}
	
	static double crtab[][50] = {
		//A0_20		A0_10	A0_05	A_025
    /*1*/{1.376,	3,		6.31,	12.796},
    /*2*/{1.061,	2,		2.92,	4.303},
    /*3*/{0.978,	2,		2.353,	3.182},
    /*4*/{0.941,	2,		2.132,	2.776},
    /*5*/{0.92,		1,		2.015,	2.571},
    /*6*/{0.906,	1,		1.943,	2.447},
    /*7*/{0.896,	1,		1.895,	2.365},
    /*8*/{0.889,	1,		1.86,	2.306},
    /*9*/{0.883,	1,		1.833,	2.262},
    /*10*/{0.879,	1,		1.812,	2.228},
    /*11*/{0.876,	1,		1.796,	2.201},
    /*12*/{0.873,	1,		1.782,	2.179},
    /*13*/{0.87,	1,		1.771,	2.16},
    /*14*/{0.868,	1,		1.761,	2.145},
    /*15*/{0.866,	1,		1.753,	2.131},
    /*16*/{0.865,	1,		1.746,	2.12},
    /*17*/{0.863,	1,		1.74,	2.11},
    /*18*/{0.862,	1,		1.734,	2.101},
    /*19*/{0.861,	1,		1.729,	2.093},
    /*20*/{0.86,	1,		1.725,	2.086},
    /*21*/{0.859,	1,		1.721,	2.08},
    /*22*/{0.858,	1,		1.717,	2.074},
    /*23*/{0.858,	1,		1.714,	2.069},
    /*24*/{0.857,	1,		1.711,	2.064},
    /*25*/{0.856,	1,		1.708,	2.06},
    /*26*/{0.856,	1,		1.71,	2.06},
    /*27*/{0.855,	1,		1.7,	2.05},
    /*28*/{0.855,	1,		1.7,	2.05},
    /*29*/{0.854,	1,		1.7,	2.04},
    /*30*/{0.854,	1,		1.7,	2.04},
	
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	/*40*/{0.851,	1,		1.68,	2.02},
		
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	/*50*/{0.848,	1,		1.67,	2}
	};	
    
    return crtab[alphacat][N-1];
}

/** @brief Do a table lookup of t critical values.
  */
double SampleTest::TCriticalValue(int N, double alpha)
{
	int alphacat;

	if ( N < 1 || N > 50 )
	{
		throw new InvalidArgumentException("N for critical value must in the range [1,50]");
	}
	
	// get a table column for the alpha level
	if (alpha <= 0.025)
	{
		alphacat = 3;
	}
	else if( alpha <= 0.05 )
	{
		alphacat = 2;
	}
	else if (alpha <= 0.1)
	{
		alphacat = 1;
	}
	else
	{
		alphacat = 0;
	}
	
	static double crtab[][50] = {
		//A0_20		A0_10	A0_05	A_025
	/*1*/{1.376,	3.08,	6.31,	12.796},
    /*2*/{1.061,	1.89,	2.92,	4.303},
    /*3*/{0.978,	1.64,	2.353,	3.182},
    /*4*/{0.941,	1.53,	2.132,	2.776},
    /*5*/{0.92,		1.48,	2.015,	2.571},
    /*6*/{0.906,	1.44,	1.943,	2.447},
    /*7*/{0.896,	1.42,	1.895,	2.365},
    /*8*/{0.889,	1.4,	1.86,	2.306},
    /*9*/{0.883,	1.38,	1.833,	2.262},
    /*10*/{0.879,	1.37,	1.812,	2.228},
    /*11*/{0.8,		1.36,	1.796,	2.201},
    /*12*/{0.873,	1.36,	1.782,	2.179},
    /*13*/{0.87,	1.35,	1.771,	2.16},
    /*14*/{0.868,	1.34,	1.761,	2.145},
    /*15*/{0.866,	1.34,	1.753,	2.131},
    /*16*/{0.865,	1.34,	1.746,	2.12},
    /*17*/{0.863,	1.33,	1.74,	2.11},
    /*18*/{0.862,	1.33,	1.734,	2.101},
    /*19*/{0.861,	1.33,	1.729,	2.093},
    /*20*/{0.86,	1.32,	1.725,	2.086},
    /*21*/{0.859,	1.32,	1.721,	2.08},
    /*22*/{0.858,	1.32,	1.717,	2.074},
    /*23*/{0.858,	1.32,	1.714,	2.069},
    /*24*/{0.857,	1.32,	1.711,	2.064},
    /*25*/{0.856,	1.32,	1.708,	2.06},
    /*26*/{0.856,	1,		1.71,	2.06},
    /*27*/{0.855,	1,		1.7,	2.05},
    /*28*/{0.855,	1,		1.7,	2.05},
    /*29*/{0.854,	1,		1.7,	2.04},
    /*30*/{0.854,	1,		1.7,	2.04},
	
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	      {0.851,	1,		1.68,	2.02},
	/*40*/{0.851,	1,		1.68,	2.02},
		
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	      {0.848,	1,		1.67,	2},
	/*50*/{0.848,	1,		1.67,	2}
	};
	
    return crtab[alphacat][N-1];
}

double UCriticalPoint(int N1, int N2, double alpha)
{
	static int table1[][10] = {
	/*     1   2   3   4   5   6   7   8   9   0*/
	/*1*/{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*2*/{-1,  4, -1, -1, -1, -1, -1, -1, -1, -1},
	/*3*/{-1,  6,  9, -1, -1, -1, -1, -1, -1, -1},
	/*4*/{-1,  8, 11, 13, 16, -1, -1, -1, -1, -1},
	/*5*/{-1,  9, 13, 16, 20, 23, -1, -1, -1, -1},
	/*6*/{-1, 11, 15, 19, 23, 27, -1, -1, -1, -1},
	/*7*/{-1, 13, 17, 22, 27, 31, 36, -1, -1, -1},
	/*8*/{-1, 14, 19, 25, 30, 35, 40, 45, -1, -1},
	/*9*/{ 9, 16, 22, 27, 33, 39, 45, 50, 56, -1},
	/*10*/{10,17, 24, 30, 37, 43, 49, 56, 62, 68} 
	};
    
	static int table2[][10] = {
	/*     1   2   3   4   5   6   7   8   9   0*/
	/*1*/{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*2*/{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*3*/{-1, -1,  9, -1, -1, -1, -1, -1, -1, -1},
	/*4*/{-1, -1, 12, 15, 18, -1, -1, -1, -1, -1},
	/*5*/{-1, 10, 14, 18, 21, 25, -1, -1, -1, -1},
	/*6*/{-1, 12, 16, 21, 25, 29, -1, -1, -1, -1},
	/*7*/{-1, 14, 19, 24, 29, 34, 38, -1, -1, -1},
	/*8*/{-1, 15, 21, 27, 32, 38, 43, 49, -1, -1},
	/*9*/{-1, 17, 23, 30, 36, 42, 48, 54, 60, -1},
	/*10*/{-1,19, 26, 33, 39, 46, 53, 60, 66, 73}
	};
        
    int d;
    
    if (N1 < N2)
	{
		if ( alpha <= 0.05 )
		{
	        d = table2[N1-1][N2-1];
		}
		else
		{
	        d = table1[N1-1][N2-1];
		}
	}
    else
	{
		if ( alpha <= 0.05 )
		{
	        d = table2[N2-1][N1-1];
		}
		else
		{
	        d = table1[N2-1][N1-1];
		}
	}
    
    if( d == -1 )
	{
        throw new InvalidArgumentException("UCriticalPoint Invalid sample sizes");
	}
	return d;
}

void SampleTest::Rankit(Sample **s1, Sample **s2)
{
	List<double> x1;
	List<double> x2;
	int x;
	int N = m_s1.N() + m_s2.N();

	if (N > 50)
	{
		throw new Exception("Rankit: Samples too large");
	}

	struct _SPair *pairs = new struct _SPair[N];
	if ( NULL == pairs )
	{
		throw OutOfMemoryException();
	}	
	for( x = 0; x < N; x++ )
	{
		pairs[x].val = -1;
		pairs[x].one = false;
	}

	for (x = 0; x < m_s1.N(); x++ )
	{
		_ArrayPairInsert (pairs, N, m_s1.Item(x), true);
	}

	for (x = 0; x < m_s2.N(); x++ )
	{
		_ArrayPairInsert (pairs, N, m_s2.Item(x), true);
	}

	// reverse the array
	for(x = 0; x < N; x++)
	{
		if (x >= N - x)
		{
			break;
		}
		
		double v1 = pairs[x].val;
		bool one1 = pairs[x].one;
		
		pairs[x].val = pairs[(N - 1) - x].val;
		pairs[x].one = pairs[(N - 1) - x].one;
		
		pairs[(N - 1) - x].val = v1;
		pairs[(N - 1) - x].one = one1;
	}
	ASSERT_MEM(pairs, N * sizeof(struct _SPair));

	// create new packs with the rankit scores as elements
	for(x = 0; x < N; x++ )
	{
		if (pairs[x].one)
		{
			// my data
			x1.Add( _AvgRankits(pairs, N, x) );
		}
		else
		{
			x2.Add( _AvgRankits(pairs, N, x) );
		}
	}
	
	delete[] pairs;
	
	if ( NULL == (*s1 = new Sample( x1 )) )
	{
		throw OutOfMemoryException();
	}
	
	if ( NULL == (*s2 = new Sample( x2 )) )
	{
		delete *s1;
		throw OutOfMemoryException();
	}
}

#ifdef DEBUG
void SampleTest::ValidateMem() const
{
	m_s1.ValidateMem();
	m_s2.ValidateMem();
}

void SampleTest::CheckMem() const
{
	m_s1.CheckMem();
	m_s2.CheckMem();
}
#endif
