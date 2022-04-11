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
#include <spl/data/SqliteTransaction.h>
#include <spl/String.h>
#include "src/sqllite/sqlite3.h"

using namespace spl;

void _ExecuteSqlLite(void *db, const String& sql)
{
	char *zErrMsg = NULL;
	int rc = sqlite3_exec((sqlite3 *)db, sql.GetChars(), NULL, NULL, &zErrMsg);
	if (SQLITE_OK != rc)
	{
		SqlException *ex = new SqlException(zErrMsg);
		sqlite3_free(zErrMsg);
		throw ex;
	}
}

SqliteTransaction::SqliteTransaction(void *db)
: m_db(db)
{
	_ExecuteSqlLite((sqlite3 *)m_db, "BEGIN TRANSACTION;");
}

SqliteTransaction::~SqliteTransaction()
{
	if (NULL != m_db)
	{
		Rollback();
	}
}

void SqliteTransaction::Commit()
{
	_ExecuteSqlLite((sqlite3 *)m_db, "COMMIT TRANSACTION;");
	m_db = NULL;
}

void SqliteTransaction::Rollback()
{
	_ExecuteSqlLite((sqlite3 *)m_db, "ROLLBACK TRANSACTION;");
	m_db = NULL;
}
