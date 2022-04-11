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
#include <spl/data/SqlLiteCommand.h>

#include "src/sqllite/sqlite3.h"

using namespace spl;

SqlLiteCommand::SqlLiteCommand()
: m_db(NULL), m_stmt(NULL)
{
}

SqlLiteCommand::SqlLiteCommand(const SqlLiteCommand& cmd)
: Command(cmd), m_db(cmd.m_db), m_stmt(NULL)
{
}

SqlLiteCommand::SqlLiteCommand(void *db, const String& cmdtxt)
: Command(cmdtxt), m_db(db), m_stmt(NULL)
{
}

SqlLiteCommand::~SqlLiteCommand()
{
	if (NULL != m_stmt)
	{
		sqlite3_finalize((sqlite3_stmt *)m_stmt);
		m_stmt = NULL;
	}
}

SqlLiteCommand& SqlLiteCommand::operator =(const SqlLiteCommand& cmd)
{
	Command::operator =(cmd);
	m_db = cmd.m_db;
	Clear();
	return *this;
}

void SqlLiteCommand::Prepare()
{
	if (NULL == m_stmt)
	{
		int rc = sqlite3_prepare_v2((sqlite3 *)m_db, m_cmdtxt.GetChars(), m_cmdtxt.Length(), (sqlite3_stmt **)&m_stmt, NULL);
		if (SQLITE_OK != rc)
		{
			throw new SqlException(sqlite3_errmsg((sqlite3 *)m_db));
		}
	}
}

void SqlLiteCommand::Clear()
{
	Command::Clear();
	if (NULL != m_stmt)
	{
		sqlite3_finalize((sqlite3_stmt *)m_stmt);
		m_stmt = NULL;
	}
}

void SqlLiteCommand::BindParameters()
{
	Prepare();

	int rc = sqlite3_clear_bindings((sqlite3_stmt*)m_stmt);
	if(SQLITE_OK != rc)
	{
		throw new SqlException(sqlite3_errmsg((sqlite3 *)m_db));
	}

	rc = sqlite3_reset((sqlite3_stmt *)m_stmt);
	if(SQLITE_OK != rc)
	{
		throw new SqlException(sqlite3_errmsg((sqlite3 *)m_db));
	}

	for (int x = 0; x < ParameterCount(); x++)
	{
		CommandParameterPtr prm = GetParameter(x);

		switch(prm->Type())
		{
		case DbSqlType::SQL_TYPE_UNASSIGNED:
			rc = sqlite3_bind_null((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()));
			break;
		case DbSqlType::SQL_TYPE_INT8:
			rc = sqlite3_bind_int((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetInt32());
			break;
		case DbSqlType::SQL_TYPE_INT16:
			rc = sqlite3_bind_int((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetInt32());
			break;
		case DbSqlType::SQL_TYPE_INT32:
			rc = sqlite3_bind_int((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetInt32());
			break;
		case DbSqlType::SQL_TYPE_INT64:
			rc = sqlite3_bind_int64((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetInt64());
			break;
		case DbSqlType::SQL_TYPE_DECIMAL:
			rc = sqlite3_bind_double((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetFloat64());
			break;
		case DbSqlType::SQL_TYPE_FLOAT32:
			rc = sqlite3_bind_double((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), (double)prm->GetFloat32());
			break;
		case DbSqlType::SQL_TYPE_FLOAT64:
			rc = sqlite3_bind_double((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetFloat64());
			break;
		case DbSqlType::SQL_TYPE_FLAG:
			rc = sqlite3_bind_int((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetInt32());
			break;
		case DbSqlType::SQL_TYPE_TIMESTAMP:
			rc = sqlite3_bind_text((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetDateTime().ToString()->GetChars(), -1, NULL);
			break;
		case DbSqlType::SQL_TYPE_DATE:
			rc = sqlite3_bind_int((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetDate().ToRevInt());
			break;
		case DbSqlType::SQL_TYPE_DATETIME:
			rc = sqlite3_bind_text((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetDateTime().ToString()->GetChars(), -1, NULL);
			break;
		case DbSqlType::SQL_TYPE_CHAR:
			rc = sqlite3_bind_text((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetVarchar()->GetChars(), -1, NULL);
			break;
		case DbSqlType::SQL_TYPE_VARCHAR:
			rc = sqlite3_bind_text((sqlite3_stmt *)m_stmt, sqlite3_bind_parameter_index((sqlite3_stmt *)m_stmt, prm->Name().GetChars()), prm->GetVarchar()->GetChars(), -1, NULL);
			break;
		case DbSqlType::SQL_TYPE_BLOB:
			throw new NotImplementedException("BLOB not yet supported");
			//break;
		}

		if(SQLITE_OK != rc)
		{
			throw new SqlException(sqlite3_errmsg((sqlite3 *)m_db));
		}
	}
}

int SqlLiteCommand::ExecuteNonQuery()
{
	BindParameters();
	int rc;

	if (SQLITE_DONE != (rc = sqlite3_step((sqlite3_stmt *)m_stmt)))
	{
		SqlException *ex = new SqlException(sqlite3_errmsg((sqlite3 *)m_db));
		sqlite3_finalize((sqlite3_stmt *)m_stmt);
		throw ex;
	}
	
	return 1;
}

RecordSetPtr SqlLiteCommand::ExecuteQuery()
{
	BindParameters();

	RecordSetPtr rs(new RecordSet());

	int numColumns = sqlite3_column_count((sqlite3_stmt *)m_stmt);

	int rc;
	while (SQLITE_ROW == (rc = sqlite3_step((sqlite3_stmt *)m_stmt)))
	{
		if (rs->ColumnCount() == 0)
		{
			for ( int x = 0; x < numColumns; x++ )
			{
				int ctyp = sqlite3_column_type((sqlite3_stmt *)m_stmt, x);
				switch (ctyp)
				{
				case SQLITE_INTEGER:
					rs->DefineColumn(sqlite3_column_name((sqlite3_stmt *)m_stmt, x), DbSqlType::SQL_TYPE_INT64, 8);
					break;
				case SQLITE_FLOAT:
					rs->DefineColumn(sqlite3_column_name((sqlite3_stmt *)m_stmt, x), DbSqlType::SQL_TYPE_FLOAT64, 8);
					break;
				case SQLITE_BLOB:
					throw new SqlException("BLOB not supported yet");
					//rs->DefineColumn(sqlite3_column_name(stmt, x), DbSqlType::SQL_TYPE_BLOB, 0);
					//break;
				case SQLITE_NULL:
					// TODO: this is wrong, can't burst the record set if column in first row is null
					rs->DefineColumn(sqlite3_column_name((sqlite3_stmt *)m_stmt, x), DbSqlType::SQL_TYPE_VARCHAR, sqlite3_column_bytes((sqlite3_stmt *)m_stmt, x));
					break;
				case SQLITE_TEXT:
					rs->DefineColumn(sqlite3_column_name((sqlite3_stmt *)m_stmt, x), DbSqlType::SQL_TYPE_VARCHAR, sqlite3_column_bytes((sqlite3_stmt *)m_stmt, x));
					break;
				default:
					throw new SqlException("Unknown column type of " + Int32::ToString(ctyp));
				}
			}
		}

		for ( int x = 0; x < numColumns; x++ )
		{
			int ctyp = sqlite3_column_type((sqlite3_stmt *)m_stmt, x);
			switch (ctyp)
			{
			case SQLITE_INTEGER:
				rs->GetColumn(x)->Append(sqlite3_column_int64((sqlite3_stmt *)m_stmt, x));
				break;
			case SQLITE_FLOAT:
				rs->GetColumn(x)->Append(sqlite3_column_double((sqlite3_stmt *)m_stmt, x));
				break;
			case SQLITE_BLOB:
				break;
			case SQLITE_NULL:
				rs->GetColumn(x)->AppendNull();
				break;
			case SQLITE_TEXT:
				rs->GetColumn(x)->Append(String((const char *)sqlite3_column_text((sqlite3_stmt *)m_stmt, x)));
				break;
			}
		}		
	}

	if (SQLITE_DONE != rc)
	{
		throw new SqlException(sqlite3_errmsg((sqlite3 *)m_db));
	}

	return rs;
}
