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
#include <spl/UInt32.h>
#include <spl/Undefined.h>

using namespace spl;

Undefined Undefined::m_instance;

Undefined::Undefined()
{
}

Undefined::~Undefined()
{
}

bool Undefined::Equals( const IComparable *a ) const
{
	return a->MajicNumber() == MajicNumber();
}

int Undefined::Compare( const IComparable *a ) const
{
	return a->MajicNumber() == MajicNumber() ? 0 : -1;
}

bool Undefined::Equals( const IComparable& a ) const
{
	return a.MajicNumber() == MajicNumber();
}

int Undefined::Compare( const IComparable& a ) const
{
	return a.MajicNumber() == MajicNumber() ? 0 : -1;
}

int32 Undefined::MajicNumber() const
{
	return UNDEFINED_MAJIC;
}

uint32 Undefined::HashCode() const
{
	return UInt32::MaxValue();
}
