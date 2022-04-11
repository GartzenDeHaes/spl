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
#ifndef _statistics_h
#define _statistics_h

#include <spl/collection/Array.h>
#include <spl/Exception.h>
#include <spl/math/Math.h>
#include <spl/Memory.h>
#include <spl/collection/List.h>
#include <spl/RefCountPtr.h>
#include <spl/collection/Vector.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	/** @brief Statistical data sample.
	 *	@ref SampleTest
	 */
	class Sample : public IMemoryValidate
	{
	private:

		Array<double> m_data;
		double m_min;
		double m_max;
		double m_mean;
		double m_ssd;
		double m_variance;
		double m_averageDeviation;

		void Recalc();
		static double Gammp(const double a, const double x);
		static double Gammq(const double a, const double x);
		static double Gcf(const double a, const double x);
		static double Gser(const double a, const double x);

	public:

		Sample(const List<double>& list);
		Sample(const Vector<double>& list);
		Sample(const Array<double>& list);
		Sample (const Sample& s);
		virtual ~Sample();

		Sample& operator =(const Sample& s);

		inline double Min() const
		{
			return m_min;
		}

		inline double Max() const
		{
			return m_max;
		}

		inline double Mean() const
		{
			return m_mean;
		}

		inline int N() const
		{
			return m_data.Count();
		}

		inline double StDevSample() const
		{
			return m_ssd;
		}
		
		inline double Variance() const
		{
			return m_variance;
		}
		
		inline double AverageDeviation() const
		{
			return m_averageDeviation;
		}

		inline double Item(int idx) const
		{
			if ( idx >= m_data.Count() || idx < 0 )
			{
				throw IndexOutOfBoundsException();
			}
			return m_data[idx];
		}
		
		double AverageVariance() const;
		double Median() const;
		double Skew() const;
		double Kurtosis() const;
		double ErrorFunction(double x) const;

		Sample TransformASin() const;
		Sample TransformSqrt375() const;
		Sample TransformLog10() const;

	#ifdef DEBUG
		virtual void ValidateMem () const;
		virtual void CheckMem () const;
	#endif
	};

	/** @} */
}
#endif
