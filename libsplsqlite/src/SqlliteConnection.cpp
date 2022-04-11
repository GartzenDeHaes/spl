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
#include <spl/Exception.h>
#include <spl/io/File.h>
#include <spl/Int32.h>
#include <spl/data/SqliteCommand.h>
#include <spl/data/SqliteConnection.h>
#include <spl/data/SqliteTransaction.h>

using namespace spl;

#include "src/sqllite/sqlite3.h"

SqliteConnection::SqliteConnection(const String& databaseFilename)
: Connection("", databaseFilename, "", ""), m_db(NULL)
{
	ChangeDatabase(databaseFilename);
}

SqliteConnection::~SqliteConnection()
{
	Close();
}

/// @brief Open the connection, ready for use after Open.
void SqliteConnection::Open()
{
	if (NULL == m_db)
	{
		int sres = sqlite3_open(m_database.GetChars(), (sqlite3 **)&m_db);
		if (0 != sres)
		{
			throw new SqlException(String("Can't open database: ") + sqlite3_errmsg((sqlite3 *)m_db));
		}
	}
}

/// @brief Close the connection, free resources.
void SqliteConnection::Close()
{
	if (NULL != m_db)
	{
		sqlite3_close((sqlite3 *)m_db);
		m_db = NULL;
	}
}

/// \brief Change to a different database
void SqliteConnection::ChangeDatabase(const String& databaseFilename)
{
	m_database = databaseFilename;
	if (NULL != m_db)
	{
		Close();
		Open();
	}
}

TransactionPtr SqliteConnection::BeginTransaction()
{
	return SqlLiteTransactionPtr (new SqlLiteTransaction(m_db));
}

CommandPtr SqliteConnection::CreateCommand()
{
	return SqlLiteCommandPtr(new SqlLiteCommand(m_db, ""));
}

CommandPtr SqliteConnection::CreateCommand(const String& cmdText)
{
	return SqlLiteCommandPtr(new SqlLiteCommand(m_db, cmdText));
}

int SqliteConnection::ExecuteNonQuery(const String& sql)
{
	SqlLiteCommand cmd(m_db, sql);
	return cmd.ExecuteNonQuery();
}

RecordSetPtr SqliteConnection::ExecuteQuery(const String& sql)
{
	SqlLiteCommand cmd(m_db, sql);
	return cmd.ExecuteQuery();
}

void SqliteConnection::RegisterThread()
{
}

void SqliteConnection::RegisterThreadExit()
{
}

void SqliteConnection::AllowOutOfOrderCommandParameters(bool allow)
{
}

#if defined(DEBUG)
void SqliteConnection::CheckMem() const
{
	Connection::CheckMem();
}

void SqliteConnection::ValidateMem() const
{
	Connection::ValidateMem();
}
#endif

