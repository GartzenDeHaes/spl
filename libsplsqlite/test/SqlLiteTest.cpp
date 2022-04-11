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
#include <spl/data/SqlLiteConnection.h>

#ifdef DEBUG

#include <spl/io/File.h>
#include <spl/io/log/Log.h>

using namespace spl;

static void _TestSqlLite1()
{
	const char *filename = "test.sqlite";

	if (File::Exists(filename))
	{
		File::Delete(filename);
	}

	{
		SqlLiteConnection con(filename);
		con.Open();

		DEBUG_CLEAR_MEM_CHECK_POINTS();
		con.CheckMem();
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("SQLite3 1.1");

		con.ExecuteNonQuery("CREATE TABLE data (PK INT PRIMARY KEY, NAME VARCHAR(255));");

		TransactionPtr trans = con.BeginTransaction();
		con.ExecuteNonQuery("INSERT INTO data (PK, NAME) VALUES (1, 'bob');");
		con.ExecuteNonQuery("INSERT INTO data (PK, NAME) VALUES (2, 'bill');");
		trans->Rollback();

		RecordSetPtr rs = con.ExecuteQuery("SELECT * FROM data;");
		UNIT_ASSERT("rollback failed", rs->RowCount() == 0);

		trans = con.BeginTransaction();
		con.ExecuteNonQuery("INSERT INTO data (PK, NAME) VALUES (1, 'bob');");
		con.ExecuteNonQuery("INSERT INTO data (PK, NAME) VALUES (2, 'bill');");
		trans->Commit();

		rs = con.ExecuteQuery("SELECT * FROM data;");
		UNIT_ASSERT("inserts failed", rs->RowCount() == 2);
		UNIT_ASSERT("1,1 = 1", rs->GetColumn(0)->GetInt32(0) == 1);
		UNIT_ASSERT("2,1 = 2", rs->GetColumn(0)->GetInt32(1) == 2);
		UNIT_ASSERT("1,2 = 'bob'", rs->GetColumn(1)->GetVarchar(0)->Equals("bob"));
		UNIT_ASSERT("2,2 = 'bill'", rs->GetColumn(1)->GetVarchar(1)->Equals("bill"));

		CommandPtr cmd = con.CreateCommand("SELECT * FROM data WHERE PK = @pk;");
		cmd->CreateParameter("@pk", DbSqlType::SQL_TYPE_INT32, ParameterDirection::PARAM_DIR_IN, 4)->Set(1);
		
		rs = cmd->ExecuteQuery();
		UNIT_ASSERT("command parameter failed", rs->RowCount() == 1);
		UNIT_ASSERT("1,1 = 1", rs->GetColumn(0)->GetInt32(0) == 1);
		UNIT_ASSERT("1,2 = 'bob'", rs->GetColumn(1)->GetVarchar(0)->Equals("bob"));

		cmd.Release();
		con.Close();
	}

	File::Delete(filename);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("SQLite3 1.2");
	Log::SWriteOkFail( "SQLite3 test 1" );
}

void _TestSqlLite()
{
	_TestSqlLite1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
