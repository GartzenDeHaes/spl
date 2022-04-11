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
#include <spl/DelegateDispatch.h>

using namespace spl;

DelegateDispatch::DelegateDispatch()
: m_delegates(2)
{
}

DelegateDispatch::~DelegateDispatch()
{
}

void DelegateDispatch::Call()
{
	for (int x = 0; x < m_delegates.Count(); x++ )
	{
		m_delegates.ElementAtRef(x)->Call();
	}
}

void DelegateDispatch::Clear()
{
	for (int x = 0; x < m_delegates.Count(); x++ )
	{
		m_delegates.ElementAtRef(x)->Clear();
	}
	m_delegates.Clear();
}

#if defined(DEBUG)
void DelegateDispatch::CheckMem() const
{
	m_delegates.CheckMem();
}

void DelegateDispatch::ValidateMem() const
{
	m_delegates.ValidateMem();
}
#endif
