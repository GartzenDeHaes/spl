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
#if defined(_WIN32) || defined(_WIN64)
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#include <spl/io/Directory.h>
#include <sys/types.h>

#include <math.h>
#ifdef HAVE_FLOAT_H
#include <float.h>
#endif
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#include <spl/Debug.h>
#include <spl/math/Math.h>
#include <spl/math/Sample.h>
#include <spl/math/SampleTest.h>

using namespace spl;

void Sample::Recalc()
{
	double sum = 0;
	int x;

	for (x = 0; x < m_data.Count(); x++)
	{
		double val = m_data[x];
		if (val < m_min)
		{
			m_min = val;
		}
		if (val > m_max)
		{
			m_max = val;
		}
		sum += val;
	}
	m_mean = sum / m_data.Count();

	sum = 0;
	double mean2 = m_mean * m_mean;
	for (x = 0; x < m_data.Count(); x++)
	{
		double val = m_data[x];
		sum += (val * val) - mean2;
		
        m_variance = m_variance + ((val - m_mean) * (val - m_mean));
        m_averageDeviation = m_averageDeviation + Math::Abs(val - m_mean);
	}
	m_ssd = Math::Sqrt(sum / m_data.Count());
	m_variance /= m_data.Count();
	m_averageDeviation /= m_data.Count();	
}

Sample::Sample(const List<double>& list)
: m_min(DBL_MAX), m_max(DBL_MIN), m_mean(0), m_ssd(0), m_data(list.ToArray())
{
	Recalc();
}

Sample::Sample(const Vector<double>& list)
: m_min(DBL_MAX), m_max(DBL_MIN), m_mean(0), m_ssd(0), m_data(list.ToArray())
{
	Recalc();
}

Sample::Sample(const Array<double>& list)
: m_min(DBL_MAX), m_max(DBL_MIN), m_mean(0), m_ssd(0), m_data(list)
{
	Recalc();
}

Sample::Sample (const Sample& s)
: m_min(s.m_min), m_max(s.m_max), m_mean(s.m_mean), m_ssd(s.m_ssd), m_variance(s.m_variance), m_averageDeviation(s.m_averageDeviation), m_data(s.m_data)
{
}

Sample::~Sample()
{
}

Sample& Sample::operator =(const Sample& s)
{
	m_min = s.m_min;
	m_max = s.m_max;
	m_mean = s.m_mean;
	m_ssd = s.m_ssd;
	m_variance = s.m_variance;
	m_averageDeviation = s.m_averageDeviation;
	m_data = s.m_data;

	return *this;
}

double Sample::AverageVariance() const
{
	double ep = 0;
	double s;
	double var = 0;

	for (int j = 0; j < m_data.Count(); j++)
	{
		s = m_data[j] - m_mean;
		ep = ep + s;
		var = var + s * s;
	}

	return (var - ep * ep / m_data.Count()) / (m_data.Count() - 1);
}

double Sample::Skew() const
{
	double sum = 0;
	for ( int x = 0; x < m_data.Count(); x++ )
	{
        sum += ((m_data[x] - m_mean) / m_ssd) * ((m_data[x] - m_mean) / m_ssd) * ((m_data[x] - m_mean) / m_ssd);
	}
	return sum / m_data.Count();
}

double Sample::Kurtosis() const
{
	double sum = 0;
	for ( int x = 0; x < m_data.Count(); x++ )
	{
        sum = sum + ((m_data[x] - m_mean) / m_ssd) * ((m_data[x] - m_mean) / m_ssd) * ((m_data[x] - m_mean) / m_ssd) * ((m_data[x] - m_mean) / m_ssd);
	}
    return sum / m_data.Count() - 3;
}

double Sample::Median() const
{
	Array<double> da(m_data);
	da.SortAsc();
	
#ifdef DEBUG
	ASSERT_MEM(da, sizeof(da));
	
	for ( int i = 0; i < m_data.Count(); i++ )
	{
		bool found = false;
		ASSERT(i == 0 || da[i] <= da[i-1]);
			
		for ( int j = 0; j < m_data.Count(); j++ )
		{
			if ( m_data[j] == da[i] )
			{
				found = true;
				break;
			}
		}
		ASSERT(found);
	}
#endif

	return da[m_data.Count() / 2];
}

double Sample::ErrorFunction(double x) const
{
	if (x < 0)
	{
		return 1 + Gammp(0.5, x * x);
	}
	else
	{
		return Gammq(0.5, x * x);
	}
}

double Sample::Gammp(const double a, const double x)
{
	if (x < 0 || a <= 0)
	{
		throw new InvalidArgumentException("gammp bad, bad args");
	}

	if (x < (a + 1))
	{
		return Gser(a, x);
	}
	else
	{
		return 1 - Gcf(a, x);
	}
}

double Sample::Gammq(const double a, const double x)
{
	if (x < 0 || a <= 0)
	{
		throw new InvalidArgumentException("gammp bad, bad args");
	}

	if( x < (a + 1) )
	{
		return 1 - Gser(a, x);
	}
	else
	{
		return Gcf(a, x);
	}
}

double Sample::Gcf(const double a, const double x)
{
	int i;
	double gln = SampleTest::Gammaln(a);
	double b = x + 1 - a;
	double c = 1 / 1E-30;
	double d = 1 / b;
	double h = d;

	for(i = 0; i < 200; i++)
	{
		double an = -i * (1 - a);
		b = b + 2;
		d = an * d + b;
		if ( Math::Abs(d) < 1E-30 )
		{
			 d = 1E-30;
		}
		c = b + an / c;
		if (Math::Abs(c) < 1E-30)
		{
			c = 1E-30;
		}
		d = 1 / d;
		double del = d * c;
		h = h * del;
		if (Math::Abs(del) < 0.0000003)
		{
			break;
		}
	}
	if( i >= 200 )
	{
		throw new Exception("gcf failed");
	}
	return Math::Exp(-x + a * Math::Log(x) - gln) * h;
}

double Sample::Gser(const double a, const double x)
{
	double gln = SampleTest::Gammaln(a);

	if( x <= 0 )
	{
		if (x < 0)
		{
			throw new InvalidArgumentException("gser x less than 0 in gser");
		}
		return 0;
	}

	double ap = a;
	double sum = 1 / a;
	double del = sum;

	for (int N = 1; N <= 200; N++)
	{
		ap = ap + 1;
		del = del * x / ap;
		sum = sum + del;
		if (Math::Abs(del) < Math::Abs(sum) * 0.0000003 )
		{
			return sum * Math::Exp(-x + a * Math::Log(x) - gln);
		}
	}

	throw new Exception( "gser a too large or ITMaX to small" );
}

Sample Sample::TransformASin() const
{
	List<double> vals;

	for( int x = 0; x < m_data.Count(); x++ )
	{
		vals.Add(Math::Asin(Math::Sqrt(m_data[x])));
	}
	return Sample(vals);
}

Sample Sample::TransformSqrt375() const
{    
	List<double> vals;

	for( int x = 0; x < m_data.Count(); x++ )
	{
		vals.Add(Math::Sqrt(m_data[x] + 0.375));
	}
	return Sample(vals);
}

Sample Sample::TransformLog10() const
{
	List<double> vals;

	for( int x = 0; x < m_data.Count(); x++ )
	{
		vals.Add(Math::Log(m_data[x]) / Math::Log(10.0) + 1);
	}
	return Sample(vals);
}

#ifdef DEBUG
void Sample::ValidateMem() const
{
	m_data.ValidateMem();
}

void Sample::CheckMem() const
{
	m_data.CheckMem();
}
#endif
