/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Foobar is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SPL.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <spl/Exception.h>
#include <spl/math/BitMask.h>
#include <memory.h>

using namespace spl;

void BitMask::CalcSize(int rows, int cols)
{
	m_size = (rows * cols)/8 + 8;
}

BitMask::BitMask(int rows, int cols)
: m_rows(rows), m_cols(cols), m_intrinsic(false)
{
	CalcSize(m_rows, m_cols);

	if ( NULL == (m_bits = (byte *)malloc( m_size )) )
	{
		throw OutOfMemoryException();
	}
	Clear();
}

BitMask::BitMask(const BitMask& bits)
: m_size(bits.m_size), m_rows(bits.m_rows), m_cols(bits.m_cols), m_intrinsic(false)
{
	if ( NULL == (m_bits = (byte *)malloc( m_size )) )
	{
		throw OutOfMemoryException();
	}
	Clear();

	memcpy(m_bits, bits.m_bits, m_size);
}

BitMask::BitMask(int rows, int cols, byte *bits)
: m_bits(bits), m_rows(rows), m_cols(cols), m_intrinsic(true)
{
	CalcSize(m_rows, m_cols);

	ValidateMem();

	Clear();
}

BitMask::~BitMask()
{
	if ( ! m_intrinsic )
	{
		free( m_bits );
	}
}

BitMask& BitMask::operator =(const BitMask& bits)
{
	free( m_bits );

	m_size = bits.m_size;
	m_rows = bits.m_rows;
	m_cols = bits.m_cols;

	if ( NULL == (m_bits = (byte *)malloc( m_size )) )
	{
		throw OutOfMemoryException();
	}
	Clear();

	memcpy(m_bits, bits.m_bits, m_size);

	return *this;
}

void BitMask::GetByteBit(int row, int col, int *bytenum, int *bytebit)
{
	if ( row >= m_rows || row < 0 || col >= m_cols || col < 0 )
	{
		throw new IndexOutOfBoundsException();
	}

	int bitnum = row * m_rows + col;
	*bytenum = bitnum / 8;
	*bytebit = bitnum % 8;

	if (*bytenum > m_size)
	{
		throw new IndexOutOfBoundsException();
	}
}

void BitMask::Set(int row, int col)
{
	int bytenum, bytebit;
	GetByteBit(row, col, &bytenum, &bytebit);
	m_bits[bytenum] |= 1<<bytebit;
}

bool BitMask::IsSet(int row, int col)
{
	int bytenum, bytebit;
	GetByteBit(row, col, &bytenum, &bytebit);
	return (m_bits[bytenum] & (1<<bytebit)) != 0;
}

void BitMask::Clear(int row, int col)
{
	int bytenum, bytebit;
	GetByteBit(row, col, &bytenum, &bytebit);
	m_bits[bytenum] ^= 1<<bytebit;
}

void BitMask::Clear()
{
	memset(m_bits, 0, m_size);
}

#ifdef DEBUG
void BitMask::ValidateMem() const
{
	ASSERT_MEM( m_bits, m_rows*m_cols );
}

void BitMask::CheckMem() const
{
	DEBUG_NOTE_MEM_ALLOCATION(m_bits);
}
#endif
