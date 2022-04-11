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
#ifndef _bitmask_h
#define _bitmask_h

#include <spl/Debug.h>
#include <spl/types.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/WeakReference.h>

namespace spl
{
	/**
	 * @defgroup math Math
	 * @{
	 */

    class BitMask;
    typedef RefCountPtr<BitMask> BitMaskPtr;
    typedef WeakReference<BitMask, BitMaskPtr> BitMaskRef;

	class BitMask : public IMemoryValidate
	{
	protected:
		int m_rows, m_cols;
		int m_size;
		byte *m_bits;
		bool m_intrinsic;

		void GetByteBit(int row, int col, int *bytenum, int *bytebit);
		void CalcSize(int row, int col);

	public:
		BitMask(int rows, int cols);
		BitMask(int rows, int cols, byte *bits);
		BitMask(const BitMask& bits);
		virtual ~BitMask();

		BitMask& operator =(const BitMask& bits);

		void Set(int row, int col);
		bool IsSet(int row, int col);
		void Clear(int row, int col);
		void Clear();

		inline int SizeInBytes() { return m_size; }

	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

	/** @} */
}
#endif
