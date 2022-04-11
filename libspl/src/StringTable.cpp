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
#include <spl/math/Math.h>
#include <spl/collection/StringTable.h>

using namespace spl;

StringTable::StringTable()
: m_stringIdx(), m_strings(), m_lists()
{
}

StringTable::StringTable(const StringTable& st)
: m_stringIdx(), m_strings(), m_lists()
{
	*this = st;
}


StringTable::~StringTable()
{
	Clear();
}

StringTable& StringTable::operator =(const StringTable& st)
{
	Clear();
	for ( int x = 0; x < st.m_strings.Count(); x++ )
	{
		GetPtr(st.m_strings.ElementAt(x));
	}

	return *this;
}

void StringTable::Clear()
{
	int x;

	m_stringIdx.Clear();

	for ( x = 0; x < m_strings.Count(); x++ )
	{
		delete m_strings.ElementAt(x);
	}
	for ( x = 0; x < m_lists.Count(); x++ )
	{
		delete m_lists.ElementAt(x);
	}
	m_strings.Clear();
	m_lists.Clear();
}

int StringTable::PositionOf(const String& str)
{
	GetPtr(str);

	for ( int x = 0; x < m_strings.Count(); x++ )
	{
		if (m_strings.ElementAt(x)->Equals(str))
		{
			return x;
		}
	}
	throw new Exception("Internal error at StringTable::PositionOf");
}

String *StringTable::GetPtr(const char *cp)
{
	Vector<String *> *lst;
	int hash = Math::Hash(cp);
	if ( !m_stringIdx.ContainsKey(hash) )
	{
		lst = new Vector<String *>();
		m_lists.Add(lst);
		m_stringIdx.Set(hash, lst);
	}
	else
	{
		lst = m_stringIdx.Get(hash);
	}
	ASSERT_MEM(lst, sizeof(Vector<String *>));

	for (int x = 0; x < lst->Count(); x++)
	{
		if (lst->ElementAt(x)->Equals(cp))
		{
			return lst->ElementAt(x);
		}
	}
	String *str = new String(cp);
	m_strings.Add(str);
	lst->Add(str);
	return str;
}

#ifdef DEBUG
void StringTable::ValidateMem() const
{
	m_stringIdx.ValidateMem();
	m_strings.ValidateMem();
	m_lists.ValidateMem();
}

void StringTable::CheckMem() const
{
	m_stringIdx.CheckMem();
	m_strings.CheckMem();
	m_lists.CheckMem();
}
#endif

