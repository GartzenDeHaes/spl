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
#include <spl/data/CommandParameter.h>
#include <spl/data/ColumnTypes.h>

using namespace spl;

CommandParameter::CommandParameter(const String& name, int type, int dir, int len)
: m_name(name), m_type(type), m_dir(dir), m_data(), m_len(len)
{
	switch( type )
	{
	case DbSqlType::SQL_TYPE_INT8:
		m_len = 1;
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_INT16:
		m_len = 2;
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_INT32:
		m_len = 4;
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_INT64:
		m_len = 8;
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_DECIMAL:	/* MYSQL: 65 digit fixed point */
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_FLOAT32:
		m_len = 4;
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_FLOAT64:
		m_len = 8;
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_FLAG:
		m_isnumeric = true;
		break;
	case DbSqlType::SQL_TYPE_TIMESTAMP:
		m_len = 8;
		m_isnumeric = false;
		break;
	case DbSqlType::SQL_TYPE_DATE:
		m_isnumeric = false;
		break;
	case DbSqlType::SQL_TYPE_DATETIME:
		m_len = 8;
		m_isnumeric = false;
		break;
	case DbSqlType::SQL_TYPE_CHAR:
		m_isnumeric = false;
		break;
	case DbSqlType::SQL_TYPE_VARCHAR:
		m_isnumeric = false;
		break;
	case DbSqlType::SQL_TYPE_BLOB:
		m_isnumeric = false;
		break;
	}
}

CommandParameter::CommandParameter(const String& name, int8 val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_INT8), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(1)
{
}

CommandParameter::CommandParameter(const String& name, int16 val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_INT16), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(2)
{
}

CommandParameter::CommandParameter(const String& name, int32 val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_INT32), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(4)
{
}

CommandParameter::CommandParameter(const String& name, int64 val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_INT64), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(8)
{
}

CommandParameter::CommandParameter(const String& name, float32 val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_FLOAT32), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(4)
{
}

CommandParameter::CommandParameter(const String& name, float64 val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_FLOAT64), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(8)
{
}

CommandParameter::CommandParameter(const String& name, bool val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_FLAG), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(4)
{
}

CommandParameter::CommandParameter(const String& name, DateTime val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_DATETIME), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val), m_len(8)
{
}

CommandParameter::CommandParameter(const String& name, char *val)
: m_name(name), m_type(DbSqlType::SQL_TYPE_VARCHAR), m_dir(ParameterDirection::PARAM_DIR_IN), m_data(val)
{
	m_len = (int)strlen(val);
}

CommandParameter::CommandParameter(const CommandParameter& prm)
:	m_name(prm.m_name),
	m_type(prm.m_type),
	m_dir(prm.m_dir),
	m_data(prm.m_data),
	m_isnumeric(prm.m_isnumeric),
	m_len(prm.m_len)
{
}

CommandParameter::~CommandParameter()
{
}

CommandParameter& CommandParameter::operator =(const CommandParameter& prm)
{
	m_name = prm.m_name;
	m_type = prm.m_type;
	m_dir = prm.m_dir;
	m_data = prm.m_data;
	m_isnumeric = prm.m_isnumeric;
	m_len = prm.m_len;

	return *this;
}

byte CommandParameter::GetByte()
{
	return m_data.ToByte();
}

int16 CommandParameter::GetInt16()
{
	return m_data.ToInt16();
}

int32 CommandParameter::GetInt32()
{
	return m_data.ToInt32();
}

int64 CommandParameter::GetInt64()
{
	return m_data.ToInt64();
}

Decimal CommandParameter::GetDecimal()
{
	return m_data.ToDecimal();
}

float32 CommandParameter::GetFloat32()
{
	return m_data.ToFloat32();
}

float64 CommandParameter::GetFloat64()
{
	return m_data.ToFloat64();
}

bool CommandParameter::GetBit()
{
	return m_data.ToBool();
}

DateTime CommandParameter::GetTimeStamp()
{
	return m_data.ToDateTime();
}

Date CommandParameter::GetDate()
{
	return m_data.ToDate();
}

DateTime CommandParameter::GetDateTime()
{
	return m_data.ToDateTime();
}

StringPtr CommandParameter::GetChar()
{
	return m_data.ToString();
}

StringPtr CommandParameter::GetVarchar()
{
	return m_data.ToString();
}

Variant& CommandParameter::GetVariant()
{
	return m_data;
}

void CommandParameter::Set(int8 val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(int16 val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(int32 val)
{
	m_data.ValidateMem();
	m_data = Variant(val);
	m_data.ValidateMem();
}

void CommandParameter::Set(int64 val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(Decimal val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(float32 val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(float64 val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(bool val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(DateTime& val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(Date& val)
{
	m_data = Variant(val);
}

void CommandParameter::Set(const String& val)
{
	m_data = Variant(val);
}

#if defined(DEBUG)
void CommandParameter::CheckMem() const
{
	m_name.CheckMem();
	m_data.CheckMem();
}

void CommandParameter::ValidateMem() const
{
	m_name.ValidateMem();
	m_data.ValidateMem();
}
#endif

