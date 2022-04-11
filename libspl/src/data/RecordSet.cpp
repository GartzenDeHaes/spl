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
#include <spl/data/RecordSet.h>
#include <spl/data/ColumnTypes.h>

using namespace spl;

IColumn::IColumn(const String& name, const int maxlen)
: m_name(name), m_maxlen(maxlen), m_currow(-1)
{
}

IColumn::~IColumn()
{
}

#if defined(DEBUG)
void IColumn::CheckMem() const
{
	m_name.CheckMem();
}

void IColumn::ValidateMem() const
{
	m_name.ValidateMem();
}
#endif

RecordSet::RecordSet()
: m_columnIdx(), m_columns()
{
}

RecordSet::RecordSet(const RecordSet& rs)
: m_columnIdx(), m_columns()
{
	*this = rs;
}

RecordSet::~RecordSet()
{
	Clear();
}

RecordSet& RecordSet::operator =(const RecordSet& rs)
{
	int count = rs.m_columns.Count();
	for ( int x = 0; x < count; x++ )
	{
		IColumn *col = rs.m_columns.ElementAt(x)->Clone();
		m_columns.Add(col);
		m_columnIdx.Set(col->Name(), col);
	}

	return *this;
}

void RecordSet::Clear()
{
	m_columnIdx.Clear();

	int count = m_columns.Count();
	for ( int x = 0; x < count; x++ )
	{
		delete m_columns.ElementAt(x);
	}
	m_columns.Clear();
}

void RecordSet::BeginIter()
{
	int count = m_columns.Count();
#ifdef DEBUG
	int rowcount = (count > 0) ? m_columns.ElementAt(0)->Count() : 0;
#endif
	for ( int x = 0; x < count; x++ )
	{
		m_columns.ElementAt(x)->BeginInter();
		ASSERT(rowcount == m_columns.ElementAt(x)->Count());
	}
}

bool RecordSet::Next()
{
	int count = m_columns.Count();
	for ( int x = 0; x < count; x++ )
	{
		if ( ! m_columns.ElementAt(x)->Next() )
		{
			return false;
		}
	}
	return true;
}

void RecordSet::SetCurrentRow(const int row)
{
	int count = m_columns.Count();
	for ( int x = 0; x < count; x++ )
	{
		m_columns.ElementAt(x)->SeekRow(row);
	}
}

void RecordSet::SetColumns( RecordSet& rs )
{
	ASSERT( rs.ColumnCount() == 0 );
	int count = m_columns.Count();
	for ( int x = 0; x < count; x++ )
	{
		IColumn *col = m_columns.ElementAt(x);
		rs.DefineColumn( col->Name().GetChars(), col->Type(), col->MaxLength() );
	}
}

void RecordSet::DefineColumn(const String& name, int type, int fieldmaxlen)
{
	IColumn *col;
	switch ( type )
	{
	case DbSqlType::SQL_TYPE_INT8:
		col = new Int8Column(name);
		break;
	case DbSqlType::SQL_TYPE_INT16:
		col = new Int16Column(name);
		break;
	case DbSqlType::SQL_TYPE_INT32:
		col = new Int32Column(name);
		break;
	case DbSqlType::SQL_TYPE_INT64:
		col = new Int64Column(name);
		break;
	case DbSqlType::SQL_TYPE_DECIMAL:
		col = new DecimalColumn(name, fieldmaxlen);
		break;
	case DbSqlType::SQL_TYPE_FLOAT32:
		col = new Float32Column(name);
		break;
	case DbSqlType::SQL_TYPE_FLOAT64:
		col = new Float64Column(name);
		break;
	case DbSqlType::SQL_TYPE_FLAG:
		col = new BitColumn(name);
		break;
	case DbSqlType::SQL_TYPE_TIMESTAMP:
		col = new TimeStampColumn(name);
		break;
	case DbSqlType::SQL_TYPE_DATE:
		col = new DateColumn(name);
		break;
	case DbSqlType::SQL_TYPE_DATETIME:
		col = new DateTimeColumn(name);
		break;
	case DbSqlType::SQL_TYPE_CHAR:
		col = new CharColumn(name, fieldmaxlen);
		break;
	case DbSqlType::SQL_TYPE_VARCHAR:
		col = new VarCharColumn(name, fieldmaxlen);
		break;
	case DbSqlType::SQL_TYPE_BLOB:
		throw new NotImplementedException();
		break;
	default:
		throw new InvalidArgumentException("Unknown SQL type");
	}
	m_columns.Add(col);
	m_columnIdx.Set(name, col);
}

#if defined(DEBUG)
void RecordSet::CheckMem() const
{
	m_columnIdx.CheckMem();
	m_columns.CheckMem();

	int count = m_columns.Count();
	for ( int x = 0; x < count; x++ )
	{
		IColumn *col = m_columns.ElementAt(x);
		DEBUG_NOTE_MEM( col );
		col->CheckMem();
	}
}

void RecordSet::ValidateMem() const
{
	m_columnIdx.ValidateMem();
	m_columns.ValidateMem();

	int count = m_columns.Count();
	for ( int x = 0; x < count; x++ )
	{
		IColumn *col = m_columns.ElementAt(x);
		ASSERT_PTR( col );
		col->ValidateMem();
	}
}
#endif
