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
#ifndef _sampletest_h
#define _sampletest_h

#include <spl/Debug.h>
#include <spl/math/Sample.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	/** @brief A statical two sample test.
	 *	@ref Sample
	 */
	class SampleTest
	{
	protected:
		Sample m_s1;
		Sample m_s2;
		
		static double Betacf(double a, double b, double x);

	public:
		SampleTest(const Sample& s1, const Sample& s2);
		SampleTest(const List<double>& s1, const List<double>& s2);
		SampleTest(const Vector<double>& s1, const Vector<double>& s2);
		SampleTest(const Array<double>& s1, const Array<double>& s2);
		SampleTest(const SampleTest& t);
		virtual ~SampleTest();
		
		SampleTest& operator =(const SampleTest& t);

		inline Sample& DataSet1() { return m_s1; }
		inline Sample& DataSet2() { return m_s2; }

		double StudentsT(double *prob);
		double FTest(double *fstat);
		double ApproximateTTest(double *prob, double *df);
		double OneSampleTTest();
		double UStat();
		double UStatAlternate();

		double WTestSmall();
		double Levene(double *prob);
		double LeveneAlternate(double *prob);

		void Rankit(Sample **x1, Sample **x2);
		
		inline double StandardError() const
		{
			return Math::Sqrt((m_s1.StDevSample() * m_s1.StDevSample()) / m_s1.N() + (m_s2.StDevSample() * m_s2.StDevSample()) / m_s2.N());
		}

		static double Betai(double a, double b, double x);
		static double Gammaln(double xx);
		static double Gamma(double d);
		static double FDistr(double x, double v, double w);
		
		static double TCriticalValue(int N, double alpha);
		static double TCriticalValueAlternate(int N, double alpha);
		static double WTestCriticalValue(int N, double alpha);
		static double UCriticalPoint(int N1, int N2, double alpha);

	#ifdef DEBUG
		virtual void ValidateMem () const;
		virtual void CheckMem () const;
	#endif
	};

	/** @} */
}
#endif
