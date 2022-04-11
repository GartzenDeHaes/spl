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
#include <spl/data/DataRow.h>

using namespace spl;

DataRow::DataRow()
: m_cols()
{
}

DataRow::DataRow(const DataRow& dr)
: m_cols()
{
	*this = dr;
}

DataRow::DataRow(const Vector<DataColumnPtr>& columns)
: m_cols()
{
	for ( int x = 0; x < m_cols.Count(); x++ )
	{
		m_cols.Add( VariantPtr(new Variant()));
	}
}

DataRow::~DataRow()
{
}

DataRow& DataRow::operator =(const DataRow& dr)
{
	for ( int x = 0; x < m_cols.Count(); x++ )
	{
		m_cols.Add( dr.m_cols.ElementAt(x)->Clone() );
	}

	return *this;
}

void DataRow::AddColumn(const DataColumn& col)
{
	VariantPtr c(new Variant());
	m_cols.Add(c);
}

void DataRow::AddColumn(const DataColumn& col, const Variant& data)
{
	m_cols.Add(data.Clone());
}

#if defined(DEBUG)
void DataRow::CheckMem() const
{
	m_cols.CheckMem();
}

void DataRow::ValidateMem() const
{
	m_cols.ValidateMem();
}
#endif
