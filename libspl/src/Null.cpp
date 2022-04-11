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
#include <spl/Null.h>

using namespace spl;

Null Null::m_instance;

Null::Null()
{
}

Null::~Null()
{
}

bool Null::Equals( const IComparable *a ) const
{
	return NULL ==a || a->MajicNumber() == MajicNumber();
}

int Null::Compare( const IComparable *a ) const
{
	return (NULL == a || a->MajicNumber() == MajicNumber()) ? 0 : -1;
}

bool Null::Equals( const IComparable& a ) const
{
	return a.MajicNumber() == MajicNumber();
}

int Null::Compare( const IComparable& a ) const
{
	return a.MajicNumber() == MajicNumber() ? 0 : -1;
}

int32 Null::MajicNumber() const
{
	return NULL_MAJIC;
}

uint32 Null::HashCode() const
{
	return 0;
}
