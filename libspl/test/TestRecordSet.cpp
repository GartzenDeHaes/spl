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
#include <spl/Debug.h>
#include <spl/io/log/Log.h>
#include <spl/data/RecordSet.h>

using namespace spl;

#ifdef DEBUG

static void _TestRecordSet1()
{
	RecordSet *rs = new RecordSet();
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 1.1");

	delete rs;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 1.2");

	Log::SWriteOkFail( "RecordSet 1" );
}

static void _TestRecordSet2()
{
	RecordSet *rs = new RecordSet();
	rs->ValidateMem();

	rs->DefineColumn("c1", DbSqlType::SQL_TYPE_INT32, 4);
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 2.1");

	delete rs;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 2.2");

	rs = new RecordSet();
	rs->DefineColumn("c1", DbSqlType::SQL_TYPE_DATETIME, 8);
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 2.4");

	delete rs;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 2.5");

	Log::SWriteOkFail( "RecordSet 2" );
}

static void _TestRecordSet2a()
{
	RecordSet *rs = new RecordSet();
	rs->DefineColumn("c1", DbSqlType::SQL_TYPE_INT32, 4);
	rs->DefineColumn("c2", DbSqlType::SQL_TYPE_VARCHAR, 32);
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 2.1");

	delete rs;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 2.2");

	Log::SWriteOkFail( "RecordSet 2" );
}

static void _TestRecordSet3()
{
	RecordSet *rs = new RecordSet();
	rs->DefineColumn("c1", DbSqlType::SQL_TYPE_INT32, 4);
	rs->DefineColumn("c2", DbSqlType::SQL_TYPE_VARCHAR, 32);
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 3.1");

	rs->GetColumn("c1")->Append( (int32)42 );
	rs->GetColumn("c2")->Append( String("row 1") );
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 3.2");

	rs->BeginIter();
	UNIT_ASSERT( "Should be a record", rs->Next() );
	UNIT_ASSERT( "in32 column data", rs->GetColumn("c1")->GetInt32() == 42 );
	UNIT_ASSERT( "varchar column data", rs->GetColumn("c2")->GetVarchar()->Equals("row 1") );
	UNIT_ASSERT( "Should not be another record", !rs->Next() );
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 3.3");

	delete rs;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 3.4");

	Log::SWriteOkFail( "RecordSet 3" );
}

static void _TestRecordSet4()
{
	DateTime dtm1(1995, 7, 1);
	DateTime dtm2(1984, 9, 11);

	RecordSet *rs = new RecordSet();
	rs->DefineColumn("c1", DbSqlType::SQL_TYPE_INT8, 1);
	rs->DefineColumn("c2", DbSqlType::SQL_TYPE_INT16, 2);
	rs->DefineColumn("c3", DbSqlType::SQL_TYPE_INT32, 4);
	rs->DefineColumn("c4", DbSqlType::SQL_TYPE_INT64, 8);
	//rs->DefineColumn("c5", DbSqlType::SQL_TYPE_DECIMAL, 12);
	rs->DefineColumn("c6", DbSqlType::SQL_TYPE_FLOAT32, 4);
	rs->DefineColumn("c7", DbSqlType::SQL_TYPE_FLOAT64, 8);
	rs->DefineColumn("c8", DbSqlType::SQL_TYPE_FLAG, 4);
	rs->DefineColumn("c9", DbSqlType::SQL_TYPE_TIMESTAMP, 8);
	rs->ValidateMem();
	//rs->DefineColumn("c10", DbSqlType::SQL_TYPE_DATE, 4);
	rs->DefineColumn("c11", DbSqlType::SQL_TYPE_DATETIME, 8);
	rs->DefineColumn("c12", DbSqlType::SQL_TYPE_CHAR, 22);
	rs->DefineColumn("c13", DbSqlType::SQL_TYPE_VARCHAR, 22);
	//rs->DefineColumn("c14", DbSqlType::SQL_TYPE_BLOB, 1024);

	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 4.1");

	rs->GetColumn("c1")->Append( (int8)1 );
	rs->GetColumn("c2")->Append( (int16)2 );
	rs->GetColumn("c3")->Append( (int32)3 );
	rs->GetColumn("c4")->Append( (int64)4 );
	rs->GetColumn("c6")->Append( (float32)5 );
	rs->GetColumn("c7")->Append( (float64)6 );
	rs->GetColumn("c8")->Append( true );
	rs->GetColumn("c9")->Append( dtm1 );
	rs->GetColumn("c11")->Append( dtm1 );
	rs->GetColumn("c12")->Append( String("char") );
	rs->GetColumn("c13")->Append( String("varchar") );

	rs->GetColumn("c1")->Append( (int8)7 );
	rs->GetColumn("c2")->Append( (int16)8 );
	rs->GetColumn("c3")->Append( (int32)9 );
	rs->GetColumn("c4")->Append( (int64)10 );
	rs->GetColumn("c6")->Append( (float32)11 );
	rs->GetColumn("c7")->Append( (float64)12 );
	rs->GetColumn("c8")->Append( false );
	rs->GetColumn("c9")->Append( dtm2 );
	rs->GetColumn("c11")->Append( dtm2 );
	rs->GetColumn("c12")->Append( String("char2") );
	rs->GetColumn("c13")->Append( String("varchar2") );

	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 4.2");

	rs->BeginIter();
	UNIT_ASSERT( "Should be a record", rs->Next() );
	UNIT_ASSERT( "byte", rs->GetColumn("c1")->GetByte() == 1 );
	UNIT_ASSERT( "int16", rs->GetColumn("c2")->GetInt16() == 2 );
	UNIT_ASSERT( "int32", rs->GetColumn("c3")->GetInt32() == 3 );
	UNIT_ASSERT( "int64", rs->GetColumn("c4")->GetInt64() == 4 );
	UNIT_ASSERT( "float32", rs->GetColumn("c6")->GetFloat32() == 5 );
	UNIT_ASSERT( "float64", rs->GetColumn("c7")->GetFloat64() == 6 );
	UNIT_ASSERT( "bool", rs->GetColumn("c8")->GetBit() == true );
	UNIT_ASSERT( "timestamp", rs->GetColumn("c9")->GetDateTime().Equals(dtm1) );
	UNIT_ASSERT( "datetime", rs->GetColumn("c11")->GetDateTime().Equals(dtm1) );
	UNIT_ASSERT( "char", rs->GetColumn("c12")->GetChar()->Equals( "char" ) );
	UNIT_ASSERT( "varchar", rs->GetColumn("c13")->GetChar()->Equals( "varchar" ) );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 4.3");

	UNIT_ASSERT( "Should be a record", rs->Next() );
	UNIT_ASSERT( "byte", rs->GetColumn("c1")->GetByte() == 7 );
	UNIT_ASSERT( "int16", rs->GetColumn("c2")->GetInt16() == 8 );
	UNIT_ASSERT( "int32", rs->GetColumn("c3")->GetInt32() == 9 );
	UNIT_ASSERT( "int64", rs->GetColumn("c4")->GetInt64() == 10 );
	UNIT_ASSERT( "float32", rs->GetColumn("c6")->GetFloat32() == 11 );
	UNIT_ASSERT( "float64", rs->GetColumn("c7")->GetFloat64() == 12 );
	UNIT_ASSERT( "bool", rs->GetColumn("c8")->GetBit() == false );
	UNIT_ASSERT( "timestamp", rs->GetColumn("c9")->GetDateTime().Equals(dtm2) );
	UNIT_ASSERT( "datetime", rs->GetColumn("c11")->GetDateTime().Equals(dtm2) );
	UNIT_ASSERT( "char", rs->GetColumn("c12")->GetChar()->Equals( "char2" ) );
	UNIT_ASSERT( "varchar", rs->GetColumn("c13")->GetChar()->Equals( "varchar2" ) );
	
	UNIT_ASSERT( "Should not be another record", !rs->Next() );
	rs->ValidateMem();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( rs );
	rs->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 4.4");

	delete rs;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("RecordSet 4.5");

	Log::SWriteOkFail( "RecordSet 4" );
}

void TestRecordSet()
{
	_TestRecordSet1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRecordSet2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRecordSet2a();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRecordSet3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRecordSet4();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
