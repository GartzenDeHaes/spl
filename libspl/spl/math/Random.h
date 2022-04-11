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
#ifndef _Random_h
#define _Random_h

#include <spl/Debug.h>
#include <spl/types.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	/** @brief This algorithm was snagged off usenet in the 80's.
	 *	@todo Identify the algorithm used, or at least analyze its randomness.
	 *
	 */
	class Random
	{
	protected:
		uint32 combo_x;
		uint32 combo_y;
		uint32 combo_z;
		uint32 combo_v;

	public:
		Random();
		Random(int seed);
			
		inline Random(const Random& rnd) 
		:	combo_x(rnd.combo_x), 
			combo_y(rnd.combo_y), 
			combo_z(rnd.combo_v), 
			combo_v(rnd.combo_v)
		{
		}

		inline Random& operator =(const Random& rnd)
		{
			combo_x = rnd.combo_x;
			combo_y = rnd.combo_y;
			combo_z = rnd.combo_z;
			combo_v = rnd.combo_v;

			return *this;
		}
		
		int Next();
		int Next(int max);
		int Next(int min, int max);
		void NextBytes(byte *data, int len);
		int NextInt();
		
		double NextDouble();
		
		static int SNextInt();
	};

	/** @} */
}
#endif
