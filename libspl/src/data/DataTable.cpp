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
#include <spl/data/DataTable.h>

using namespace spl;

DataTable::DataTable()
: m_name("table"), m_rows(100), m_cols(), m_colIdx()
{
}

DataTable::DataTable(const String& name)
: m_name(name), m_rows(100), m_cols(), m_colIdx()
{
}

DataTable::DataTable(const DataTable& dt)
: m_name(), m_rows(), m_cols(), m_colIdx()
{
	*this = dt;
}

DataTable::~DataTable()
{
}

DataTable& DataTable::operator =(const DataTable& dt)
{
	m_name = dt.m_name;
	m_cols = dt.m_cols;
	m_colIdx = dt.m_colIdx;

	for (int x = 0; x < dt.m_rows.Count(); x++ )
	{
		m_rows.Add(DataRowPtr(new  DataRow(dt.m_rows.ElementAt(x))));
	}

	return *this;
}

DataRowPtr DataTable::AddRow()
{
	DataRowPtr row( new DataRow(m_cols) );
	m_rows.Add(row);
	return row;
}

void DataTable::AddRow(DataRowPtr row)
{
	ASSERT(m_cols.Count() == 0 || row->Count() == m_cols.Count());
	m_rows.Add(row);
}

DataColumnPtr DataTable::AddColumn(const String& name)
{
	DataColumnPtr col(new DataColumn(name));
	m_cols.Add(col);

	for (int x = 0; x < m_rows.Count(); x++ )
	{
		m_rows.ElementAt(x)->AddColumn(col);
	}

	return col;
}

void DataTable::AddColumn(DataColumnPtr col)
{
	m_cols.Add(col);
	for (int x = 0; x < m_rows.Count(); x++ )
	{
		m_rows.ElementAt(x)->AddColumn(col);
	}
}

#if defined(DEBUG)
void DataTable::CheckMem() const
{
	m_name.CheckMem();
	m_rows.CheckMem();
	m_cols.CheckMem();
	m_colIdx.CheckMem();
}

void DataTable::ValidateMem() const
{
	m_name.ValidateMem();
	m_rows.ValidateMem();
	m_cols.ValidateMem();
	m_colIdx.ValidateMem();
}
#endif
