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
#include <spl/data/DataColumn.h>
#include <spl/DateTime.h>
#include <spl/Date.h>
#include <spl/Decimal.h>

using namespace spl;

DataColumn::DataColumn()
{
}

DataColumn::DataColumn(const DataColumn& col)
{
}

DataColumn::DataColumn(const String& name, int index, int colType, int dataLen)
: m_name(name), m_index(index), m_colType(colType), m_dataLen(dataLen)
{
}

DataColumn::~DataColumn()
{
}

DataColumn& DataColumn::operator =(const DataColumn& col)
{
	m_name = col.m_name;
	m_index = col.m_index;
	m_colType = col.m_colType;
	m_dataLen = col.m_dataLen;

	return *this;
}

/*void DataColumn::InitCell(Variant& v) const
{
	switch (m_colType)
	{
	case DbSqlType::SQL_TYPE_UNASSIGNED:
		break;
	case DbSqlType::SQL_TYPE_INT8:
		v = (byte)0;
		break;
	case DbSqlType::SQL_TYPE_INT16:
		v = (int16)0;
		break;
	case DbSqlType::SQL_TYPE_INT32:
		v = (int32)0;
		break;
	case DbSqlType::SQL_TYPE_INT64:
		v = (int64)0;
		break;
	case DbSqlType::SQL_TYPE_DECIMAL:
		v = Decimal();
		break;
	case DbSqlType::SQL_TYPE_FLOAT32:
		v = (float32)0;
		break;
	case DbSqlType::SQL_TYPE_FLOAT64:
		v = (float64)0;
		break;
	case DbSqlType::SQL_TYPE_FLAG:
		v = (byte)0;
		break;
	case DbSqlType::SQL_TYPE_TIMESTAMP:
		v = DateTime(0, 0, 0);
		break;
	case DbSqlType::SQL_TYPE_DATE:
		v = Date(0, 0, 0);
		break;
	case DbSqlType::SQL_TYPE_DATETIME:
		v = DateTime(0, 0, 0);
		break;
	case DbSqlType::SQL_TYPE_CHAR:
		v = String();
		break;
	case DbSqlType::SQL_TYPE_VARCHAR:
		v = String();
		break;
	case DbSqlType::SQL_TYPE_BLOB:
		throw new InvalidArgumentException("Binary not supported");
	}
}*/

DataColumnPtr DataColumn::Clone() const
{
	return DataColumnPtr(new DataColumn(*this));
}

#if defined(DEBUG)
void DataColumn::CheckMem() const
{
	m_name.CheckMem();
}

void DataColumn::ValidateMem() const
{
	m_name.ValidateMem();
}
#endif
