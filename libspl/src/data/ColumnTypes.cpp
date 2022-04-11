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
#include <spl/data/ColumnTypes.h>
#include <spl/Double.h>
#include <spl/Int32.h>
#include <spl/Int64.h>

using namespace spl;

#if defined(HAVE_SQLFRONT_H) && defined(HAVE_SQLDB_H)

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TYPE_H
#include <sys/time.h>
#endif

extern "C" {
#include <sqlfront.h>
#include <sqldb.h>
#include <sybdb.h>
};

#endif

void _ReverseBytes(byte *data, int len)
{
	ASSERT((len % 2) == 0);

	for ( int x = 0; x < len/2; x++ )
	{
		byte b = data[x];
		int x2 = (len-x)-1;
		ASSERT(x2 >= 0);
		data[x] = data[x2];
		data[x2] = b;
	}
}

Int8Column::Int8Column(const String& name)
: IColumn(name, 1), m_data()
{
}

Int8Column::~Int8Column() {}

int Int8Column::Count() const { return m_data.Count(); }
int Int8Column::Type() const { return DbSqlType::SQL_TYPE_INT8; }

IColumn *Int8Column::Clone() const 
{
	Int8Column *col = new Int8Column(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void Int8Column::Append( int8 i ) { m_data.Add( i ); }
void Int8Column::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( bool i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( Date i ) { throw new InvalidTypeConversionException(); }
void Int8Column::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void Int8Column::AppendParse( const char *data, const int len) { m_data.Add( (int8)atoi(data) ); }
void Int8Column::Append( void *data, int len )
{
	ASSERT(len != 0);
	// Append the LSB
#if LITTLE_ENDIAN
	Append( ((byte *)data)[len-1] );
#else
	Append( ((byte *)data)[0] );
#endif
}
void Int8Column::Append( IColumn *col, const int row ) { Append(col->GetByte(row)); }
void Int8Column::AppendNull() { Append(0); }

int8 Int8Column::GetByte(const int row) { return m_data.ElementAt(row); }
int16 Int8Column::GetInt16(const int row) { return (int16)m_data.ElementAt(row); }
int32 Int8Column::GetInt32(const int row) { return (int32)m_data.ElementAt(row); }
int64 Int8Column::GetInt64(const int row) { return (int64)m_data.ElementAt(row); }
Decimal Int8Column::GetDecimal(const int row) { return Decimal(m_data.ElementAt(row)); }
float32 Int8Column::GetFloat32(const int row) { return (float32)m_data.ElementAt(row); }
float64 Int8Column::GetFloat64(const int row) { return (float64)m_data.ElementAt(row); }
bool Int8Column::GetBit(const int row) { return 0 != m_data.ElementAt(row); }
DateTime Int8Column::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date Int8Column::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime Int8Column::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Int8Column::GetChar(const int row) { ASSERT(row >= 0); return Int32::ToString(GetInt32(row)); }
StringPtr Int8Column::GetVarchar(const int row) { ASSERT(row >= 0); return Int32::ToString(GetInt32(row)); }
Variant Int8Column::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void Int8Column::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void Int8Column::ValidateMem() const { IColumn::ValidateMem(); IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

Int16Column::Int16Column(const String& name) 
: IColumn(name, 2), m_data()
{ 
}

Int16Column::~Int16Column() { }

int Int16Column::Count() const { return m_data.Count(); }
int Int16Column::Type() const { return DbSqlType::SQL_TYPE_INT16; }

IColumn *Int16Column::Clone() const 
{
	Int16Column *col = new Int16Column(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void Int16Column::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( int16 i ) { m_data.Add( i ); }
void Int16Column::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( bool i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( Date i ) { throw new InvalidTypeConversionException(); }
void Int16Column::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void Int16Column::AppendParse( const char *data, const int len) { m_data.Add( (int16)atoi(data) ); }
void Int16Column::Append( void *data, int len )
{
	ASSERT(len >= 2);
	// Append the LSB's
#if LITTLE_ENDIAN
	Append( *(int16 *)&((byte *)data)[len-2] );
#else
	Append( *(int16 *)&((byte *)data)[1] );
#endif
}
void Int16Column::Append( IColumn *col, const int row ) { Append(col->GetInt16(row)); }
void Int16Column::AppendNull() { Append(0); }

int8 Int16Column::GetByte(const int row) { return (int8)m_data.ElementAt(row); }
int16 Int16Column::GetInt16(const int row) { return m_data.ElementAt(row); }
int32 Int16Column::GetInt32(const int row) { return (int32)m_data.ElementAt(row); }
int64 Int16Column::GetInt64(const int row) { return (int64)m_data.ElementAt(row); }
Decimal Int16Column::GetDecimal(const int row) { return Decimal(m_data.ElementAt(row)); }
float32 Int16Column::GetFloat32(const int row) { return (float32)m_data.ElementAt(row); }
float64 Int16Column::GetFloat64(const int row) { return (float64)m_data.ElementAt(row); }
bool Int16Column::GetBit(const int row) { return 0 != m_data.ElementAt(row); }
DateTime Int16Column::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date Int16Column::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime Int16Column::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Int16Column::GetChar(const int row) { ASSERT(row >= 0); return Int32::ToString(GetInt32(row)); }
StringPtr Int16Column::GetVarchar(const int row) { ASSERT(row >= 0); return Int32::ToString(GetInt32(row)); }
Variant Int16Column::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void Int16Column::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void Int16Column::ValidateMem() const { IColumn::ValidateMem(); IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

Int32Column::Int32Column(const String& name) 
: IColumn(name, 4), m_data()
{ 
}

Int32Column::~Int32Column() { }

int Int32Column::Count() const { return m_data.Count(); }
int Int32Column::Type() const { return DbSqlType::SQL_TYPE_INT32; }

IColumn *Int32Column::Clone() const 
{
	Int32Column *col = new Int32Column(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void Int32Column::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( int32 i ) { m_data.Add( i ); }
void Int32Column::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( bool i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( Date i ) { throw new InvalidTypeConversionException(); }
void Int32Column::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void Int32Column::AppendParse( const char *data, const int len) { m_data.Add( (int32)atoi(data) ); }
void Int32Column::Append( void *data, int len )
{
	ASSERT(len >= 4);
	// Append the LSB's
#if LITTLE_ENDIAN
	Append( *(int32 *)&((byte *)data)[len-4] );
#else
	Append( *(int32 *)&((byte *)data)[3] );
#endif
}
void Int32Column::Append( IColumn *col, const int row ) { Append(col->GetInt32(row)); }
void Int32Column::AppendNull() { Append(0); }

int8 Int32Column::GetByte(const int row) { return (int8)m_data.ElementAt(row); }
int16 Int32Column::GetInt16(const int row) { return (int16)m_data.ElementAt(row); }
int32 Int32Column::GetInt32(const int row) { return (int32)m_data.ElementAt(row); }
int64 Int32Column::GetInt64(const int row) { return (int64)m_data.ElementAt(row); }
Decimal Int32Column::GetDecimal(const int row) { return Decimal(m_data.ElementAt(row)); }
float32 Int32Column::GetFloat32(const int row) { return (float32)m_data.ElementAt(row); }
float64 Int32Column::GetFloat64(const int row) { return (float64)m_data.ElementAt(row); }
bool Int32Column::GetBit(const int row) { return 0 != m_data.ElementAt(row); }
DateTime Int32Column::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date Int32Column::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime Int32Column::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Int32Column::GetChar(const int row) { ASSERT(row >= 0); return Int32::ToString(GetInt32(row)); }
StringPtr Int32Column::GetVarchar(const int row) { ASSERT(row >= 0); return Int32::ToString(GetInt32(row)); }
Variant Int32Column::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void Int32Column::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void Int32Column::ValidateMem() const { IColumn::ValidateMem(); IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

Int64Column::Int64Column(const String& name) 
: IColumn(name, 8), m_data()
{ 
}

Int64Column::~Int64Column() { }

int Int64Column::Count() const { return m_data.Count(); }
int Int64Column::Type() const { return DbSqlType::SQL_TYPE_INT64; }

IColumn *Int64Column::Clone() const 
{
	Int64Column *col = new Int64Column(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void Int64Column::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( int64 i ) { m_data.Add( i ); }
void Int64Column::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( bool i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( Date i ) { throw new InvalidTypeConversionException(); }
void Int64Column::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void Int64Column::AppendParse( const char *data, const int len) { m_data.Add( atol(data) ); }
void Int64Column::Append( void *data, int len )
{
	ASSERT(len >= 8);
	// Append the LSB's
#if LITTLE_ENDIAN
	Append( *(int32 *)&((byte *)data)[len-8] );
#else
	Append( *(int32 *)&((byte *)data)[7] );
#endif
}
void Int64Column::Append( IColumn *col, const int row ) { Append(col->GetInt64(row)); }
void Int64Column::AppendNull() { Append(0); }

int8 Int64Column::GetByte(const int row) { return (int8)m_data.ElementAt(row); }
int16 Int64Column::GetInt16(const int row) { return (int16)m_data.ElementAt(row); }
int32 Int64Column::GetInt32(const int row) { return (int32)m_data.ElementAt(row); }
int64 Int64Column::GetInt64(const int row) { return (int64)m_data.ElementAt(row); }
Decimal Int64Column::GetDecimal(const int row) { return Decimal(m_data.ElementAt(row)); }
float32 Int64Column::GetFloat32(const int row) { return (float32)m_data.ElementAt(row); }
float64 Int64Column::GetFloat64(const int row) { return (float64)m_data.ElementAt(row); }
bool Int64Column::GetBit(const int row) { return 0 != m_data.ElementAt(row); }
DateTime Int64Column::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date Int64Column::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime Int64Column::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Int64Column::GetChar(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Int64Column::GetVarchar(const int row) { throw new InvalidTypeConversionException(); }
Variant Int64Column::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void Int64Column::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void Int64Column::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

Float32Column::Float32Column(const String& name) 
: IColumn(name, 4), m_data()
{ 
}

Float32Column::~Float32Column() { }

int Float32Column::Count() const { return m_data.Count(); }
int Float32Column::Type() const { return DbSqlType::SQL_TYPE_FLOAT32; }

IColumn *Float32Column::Clone() const 
{
	Float32Column *col = new Float32Column(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void Float32Column::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( float32 i ) { m_data.Add( i ); }
void Float32Column::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( bool i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( Date i ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void Float32Column::Append( void *data, int len )
{
	ASSERT(len == 4);
	// Append the LSB's
#if LITTLE_ENDIAN
	Append( *(float32 *)data );
#else
	_ReverseBytes((byte *)data, len);
	Append( *(float32 *)data );
#endif
}
void Float32Column::Append( IColumn *col, const int row ) { Append(col->GetFloat32(row)); }
void Float32Column::AppendNull() { Append(0); }

int8 Float32Column::GetByte(const int row) { return (int8)m_data.ElementAt(row); }
int16 Float32Column::GetInt16(const int row) { return (int16)m_data.ElementAt(row); }
int32 Float32Column::GetInt32(const int row) { return (int32)m_data.ElementAt(row); }
int64 Float32Column::GetInt64(const int row) { return (int64)m_data.ElementAt(row); }
Decimal Float32Column::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 Float32Column::GetFloat32(const int row) { return m_data.ElementAt(row); }
float64 Float32Column::GetFloat64(const int row) { return (float64)m_data.ElementAt(row); }
bool Float32Column::GetBit(const int row) { return 0 != m_data.ElementAt(row); }
DateTime Float32Column::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date Float32Column::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime Float32Column::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Float32Column::GetChar(const int row) { ASSERT(row >= 0); return Double::ToString(GetFloat64(row)); }
StringPtr Float32Column::GetVarchar(const int row) { ASSERT(row >= 0); return Double::ToString(GetFloat64(row)); }
void Float32Column::AppendParse( const char *data, const int len) { m_data.Add( (float32)atof(data) ); }
Variant Float32Column::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void Float32Column::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void Float32Column::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

Float64Column::Float64Column(const String& name) 
: IColumn(name, 8), m_data()
{ 
}

Float64Column::~Float64Column() { }

int Float64Column::Count() const { return m_data.Count(); }
int Float64Column::Type() const { return DbSqlType::SQL_TYPE_FLOAT64; }

IColumn *Float64Column::Clone() const 
{
	Float64Column *col = new Float64Column(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void Float64Column::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( float64 i ) { m_data.Add( i ); }
void Float64Column::Append( bool i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( Date i ) { throw new InvalidTypeConversionException(); }
void Float64Column::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void Float64Column::AppendParse( const char *data, const int len) { m_data.Add( (float64)atof(data) ); }
void Float64Column::Append( void *data, int len )
{
	ASSERT(len == 8);
	// Append the LSB's
#if LITTLE_ENDIAN
	Append( *(float64 *)data );
#else
	_ReverseBytes((byte *)data, len);
	Append( *(float64 *)data );
#endif
}
void Float64Column::Append( IColumn *col, const int row ) { Append(col->GetFloat64(row)); }
void Float64Column::AppendNull() { Append(0); }

int8 Float64Column::GetByte(const int row) { return (int8)m_data.ElementAt(row); }
int16 Float64Column::GetInt16(const int row) { return (int16)m_data.ElementAt(row); }
int32 Float64Column::GetInt32(const int row) { return (int32)m_data.ElementAt(row); }
int64 Float64Column::GetInt64(const int row) { return (int64)m_data.ElementAt(row); }
Decimal Float64Column::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 Float64Column::GetFloat32(const int row) { return (float32)m_data.ElementAt(row); }
float64 Float64Column::GetFloat64(const int row) { return m_data.ElementAt(row); }
bool Float64Column::GetBit(const int row) { return 0 != m_data.ElementAt(row); }
DateTime Float64Column::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date Float64Column::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime Float64Column::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Float64Column::GetChar(const int row) { throw new InvalidTypeConversionException(); }
StringPtr Float64Column::GetVarchar(const int row) { throw new InvalidTypeConversionException(); }
Variant Float64Column::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void Float64Column::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void Float64Column::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

DecimalColumn::DecimalColumn(const String& name, const int maxlen) 
: IColumn(name, maxlen), m_data()
{ 
}

DecimalColumn::~DecimalColumn() { }

int DecimalColumn::Count() const { return m_data.Count(); }
int DecimalColumn::Type() const { return DbSqlType::SQL_TYPE_DECIMAL; }

IColumn *DecimalColumn::Clone() const 
{
	DecimalColumn *col = new DecimalColumn(m_name.GetChars(), m_maxlen);
	col->m_data = m_data;
	return col;
}

void DecimalColumn::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( Decimal i ) { m_data.Add( i.RawData() ); }
void DecimalColumn::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( bool i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( Date i ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void DecimalColumn::AppendParse( const char *data, const int len) { m_data.Add( Decimal::Parse(data) ); }
void DecimalColumn::Append( void *data, int len )
{
	throw NotImplementedException("Decimal not yet supported");
}
void DecimalColumn::Append( IColumn *col, const int row ) { Append(col->GetDecimal(row)); }
void DecimalColumn::AppendNull() { Append(0); }

int8 DecimalColumn::GetByte(const int row) { throw new InvalidTypeConversionException(); }
int16 DecimalColumn::GetInt16(const int row) { throw new InvalidTypeConversionException(); }
int32 DecimalColumn::GetInt32(const int row) { throw new InvalidTypeConversionException(); }
int64 DecimalColumn::GetInt64(const int row) { throw new InvalidTypeConversionException(); }
Decimal DecimalColumn::GetDecimal(const int row) { return Decimal(m_data.ElementAt(row)); }
float32 DecimalColumn::GetFloat32(const int row) { return (float32)Decimal(m_data.ElementAt(row)).ToDouble(); }
float64 DecimalColumn::GetFloat64(const int row) { return (float64)Decimal(m_data.ElementAt(row)).ToDouble(); }
bool DecimalColumn::GetBit(const int row) { return m_data.ElementAt(row) != 0; }
DateTime DecimalColumn::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date DecimalColumn::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime DecimalColumn::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr DecimalColumn::GetChar(const int row) { throw new InvalidTypeConversionException(); }
StringPtr DecimalColumn::GetVarchar(const int row) { throw new InvalidTypeConversionException(); }
Variant DecimalColumn::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void DecimalColumn::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void DecimalColumn::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

BitColumn::BitColumn(const String& name) 
: IColumn(name, sizeof(bool)), m_data()
{ 
}

BitColumn::~BitColumn() { }

int BitColumn::Count() const { return m_data.Count(); }
int BitColumn::Type() const { return DbSqlType::SQL_TYPE_FLAG; }

IColumn *BitColumn::Clone() const 
{
	BitColumn *col = new BitColumn(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void BitColumn::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( bool i ) { m_data.Add( i ); }
void BitColumn::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( Date i ) { throw new InvalidTypeConversionException(); }
void BitColumn::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void BitColumn::AppendParse( const char *data, const int len) { m_data.Add( 0 != atoi(data) ); }
void BitColumn::Append( void *data, int len )
{
	switch (len)
	{
	case 1:
		Append( *(byte *)data != 0 );
		break;
	case 2:
		Append( *(int16 *)data != 0 );
		break;
	case 4:
		Append( *(int32 *)data != 0 );
		break;
	case 8:
		Append( *(int64 *)data != 0 );
		break;
	default:
		throw Exception("Unsupported bit column length");
	}
}
void BitColumn::Append( IColumn *col, const int row ) { Append(col->GetBit(row)); }
void BitColumn::AppendNull() { Append(0); }

int8 BitColumn::GetByte(const int row) { return (int8)m_data.ElementAt(row); }
int16 BitColumn::GetInt16(const int row) { return (int16)m_data.ElementAt(row); }
int32 BitColumn::GetInt32(const int row) { return (int32)m_data.ElementAt(row); }
int64 BitColumn::GetInt64(const int row) { return (int64)m_data.ElementAt(row); }
Decimal BitColumn::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 BitColumn::GetFloat32(const int row) { throw new InvalidTypeConversionException(); }
float64 BitColumn::GetFloat64(const int row) { throw new InvalidTypeConversionException(); }
bool BitColumn::GetBit(const int row) { return m_data.ElementAt(row); }
DateTime BitColumn::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date BitColumn::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime BitColumn::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr BitColumn::GetChar(const int row) { throw new InvalidTypeConversionException(); }
StringPtr BitColumn::GetVarchar(const int row) { throw new InvalidTypeConversionException(); }
Variant BitColumn::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void BitColumn::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void BitColumn::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

TimeStampColumn::TimeStampColumn(const String& name) 
: IColumn(name, sizeof(DateTime)), m_data()
{ 
}

TimeStampColumn::~TimeStampColumn() { }

int TimeStampColumn::Count() const { return m_data.Count(); }
int TimeStampColumn::Type() const { return DbSqlType::SQL_TYPE_DATETIME; }

IColumn *TimeStampColumn::Clone() const 
{
	TimeStampColumn *col = new TimeStampColumn(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void TimeStampColumn::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( bool i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( DateTime i ) { m_data.Add( i ); }
void TimeStampColumn::Append( Date i ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void TimeStampColumn::AppendParse( const char *data, const int len) { m_data.Add( DateTime::Parse(data) ); }
void TimeStampColumn::Append( void *data, int len )
{
#if defined(HAVE_SQLFRONT_H) && defined(HAVE_SQLDB_H)

	ASSERT(len == sizeof(DBDATETIME));
	time_t ret;
	struct tm t;
	DBDATEREC rec;	
	DBDATETIME *dtm = (DBDATETIME *)data;
	dbdatecrack( NULL, &rec, dtm );

	memset( &t, 0, sizeof(t) );
	t.tm_hour = rec.hour;
	t.tm_mday = rec.month;
	t.tm_min = rec.minute;
	t.tm_mon = rec.month - 1;
	t.tm_sec = rec.second;
	//t.tm_wday = rec.datedweek;
	//t.tm_yday = rec.datedyear;
	t.tm_year = rec.year - 1900;

	ret = mktime( &t );
	if ( ret < 0 )
	{
		throw SqlException("Invalid date time");
	}
	Append(DateTime(ret));

#else
	throw NotImplementedException();
#endif
}
void TimeStampColumn::Append( IColumn *col, const int row ) { Append(col->GetDateTime(row)); }
void TimeStampColumn::AppendNull() { Append(0); }

int8 TimeStampColumn::GetByte(const int row) { throw new InvalidTypeConversionException(); }
int16 TimeStampColumn::GetInt16(const int row) { throw new InvalidTypeConversionException(); }
int32 TimeStampColumn::GetInt32(const int row) { throw new InvalidTypeConversionException(); }
int64 TimeStampColumn::GetInt64(const int row) { throw new InvalidTypeConversionException(); }
Decimal TimeStampColumn::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 TimeStampColumn::GetFloat32(const int row) { throw new InvalidTypeConversionException(); }
float64 TimeStampColumn::GetFloat64(const int row) { throw new InvalidTypeConversionException(); }
bool TimeStampColumn::GetBit(const int row) { throw new InvalidTypeConversionException(); }
DateTime TimeStampColumn::GetTimeStamp(const int row) { return m_data.ElementAt(row); }
Date TimeStampColumn::GetDate(const int row) { return m_data.ElementAt(row).DatePart(); }
DateTime TimeStampColumn::GetDateTime(const int row) { return m_data.ElementAt(row); }
StringPtr TimeStampColumn::GetChar(const int row) { throw new InvalidTypeConversionException(); }
StringPtr TimeStampColumn::GetVarchar(const int row) { throw new InvalidTypeConversionException(); }
Variant TimeStampColumn::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void TimeStampColumn::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void TimeStampColumn::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

DateColumn::DateColumn(const String& name) 
: IColumn(name, sizeof(Date)), m_data()
{ 
}

DateColumn::~DateColumn() { }

int DateColumn::Count() const { return m_data.Count(); }
int DateColumn::Type() const { return DbSqlType::SQL_TYPE_DATE; }

IColumn *DateColumn::Clone() const 
{
	DateColumn *col = new DateColumn(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void DateColumn::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( bool i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void DateColumn::Append( Date i ) { m_data.Add( i ); }
void DateColumn::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void DateColumn::AppendParse( const char *data, const int len) { m_data.Add( Date::Parse(data) ); }
void DateColumn::Append( void *data, int len )
{
	throw NotImplementedException("This append is intended for MSSQL, which doesn't support a DATE type");
}
void DateColumn::Append( IColumn *col, const int row ) { Append(col->GetDate(row)); }
void DateColumn::AppendNull() { Append(0); }

int8 DateColumn::GetByte(const int row) { throw new InvalidTypeConversionException(); }
int16 DateColumn::GetInt16(const int row) { throw new InvalidTypeConversionException(); }
int32 DateColumn::GetInt32(const int row) { throw new InvalidTypeConversionException(); }
int64 DateColumn::GetInt64(const int row) { throw new InvalidTypeConversionException(); }
Decimal DateColumn::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 DateColumn::GetFloat32(const int row) { throw new InvalidTypeConversionException(); }
float64 DateColumn::GetFloat64(const int row) { throw new InvalidTypeConversionException(); }
bool DateColumn::GetBit(const int row) { throw new InvalidTypeConversionException(); }
DateTime DateColumn::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date DateColumn::GetDate(const int row) { return m_data.ElementAt(row); }
DateTime DateColumn::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr DateColumn::GetChar(const int row) { return m_data.ElementAt(row).ToString(); }
StringPtr DateColumn::GetVarchar(const int row) { return m_data.ElementAt(row).ToString(); }
Variant DateColumn::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void DateColumn::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void DateColumn::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

DateTimeColumn::DateTimeColumn(const String& name) 
: IColumn(name, sizeof(DateTime)), m_data()
{ 
}

DateTimeColumn::~DateTimeColumn() { }

int DateTimeColumn::Count() const { return m_data.Count(); }
int DateTimeColumn::Type() const { return DbSqlType::SQL_TYPE_DATETIME; }

IColumn *DateTimeColumn::Clone() const 
{
	DateTimeColumn *col = new DateTimeColumn(m_name.GetChars());
	col->m_data = m_data;
	return col;
}

void DateTimeColumn::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( bool i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( DateTime i ) { m_data.Add( i ); }
void DateTimeColumn::Append( Date i ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::Append( const String& str ) { throw new InvalidTypeConversionException(); }
void DateTimeColumn::AppendParse( const char *data, const int len) { m_data.Add( DateTime::Parse(data) ); }
void DateTimeColumn::Append( void *data, int len )
{
#if defined(HAVE_SQLFRONT_H) && defined(HAVE_SQLDB_H)

	ASSERT(len == sizeof(DBDATETIME));
	time_t ret;
	struct tm t;
	DBDATEREC rec;	
	DBDATETIME *dtm = (DBDATETIME *)data;
	dbdatecrack( NULL, &rec, dtm );

	memset( &t, 0, sizeof(t) );
	t.tm_hour = rec.hour;
	t.tm_mday = rec.month;
	t.tm_min = rec.minute;
	t.tm_mon = rec.month - 1;
	t.tm_sec = rec.second;
	//t.tm_wday = rec.datedweek;
	//t.tm_yday = rec.datedyear;
	t.tm_year = rec.year - 1900;

	ret = mktime( &t );
	if ( ret < 0 )
	{
		throw SqlException("Invalid date time");
	}
	Append(DateTime(ret));

#else
	throw NotImplementedException();
#endif
}
void DateTimeColumn::Append( IColumn *col, const int row ) { Append(col->GetDateTime(row)); }
void DateTimeColumn::AppendNull() { Append(0); }

int8 DateTimeColumn::GetByte(const int row) { throw new InvalidTypeConversionException(); }
int16 DateTimeColumn::GetInt16(const int row) { throw new InvalidTypeConversionException(); }
int32 DateTimeColumn::GetInt32(const int row) { throw new InvalidTypeConversionException(); }
int64 DateTimeColumn::GetInt64(const int row) { throw new InvalidTypeConversionException(); }
Decimal DateTimeColumn::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 DateTimeColumn::GetFloat32(const int row) { throw new InvalidTypeConversionException(); }
float64 DateTimeColumn::GetFloat64(const int row) { throw new InvalidTypeConversionException(); }
bool DateTimeColumn::GetBit(const int row) { throw new InvalidTypeConversionException(); }
DateTime DateTimeColumn::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date DateTimeColumn::GetDate(const int row) { return m_data.ElementAt(row).DatePart(); }
DateTime DateTimeColumn::GetDateTime(const int row) { return m_data.ElementAt(row); }
StringPtr DateTimeColumn::GetChar(const int row) { return m_data.ElementAt(row).ToString(); }
StringPtr DateTimeColumn::GetVarchar(const int row) { return m_data.ElementAt(row).ToString(); }
Variant DateTimeColumn::GetVariant(const int row) { return Variant(m_data.ElementAt(row)); }

#if defined(DEBUG)
void DateTimeColumn::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void DateTimeColumn::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

CharColumn::CharColumn(const String& name, const int maxlen) 
: IColumn(name, maxlen), m_data()
{ 
}

CharColumn::~CharColumn() { }

int CharColumn::Count() const { return m_data.Count(); }
int CharColumn::Type() const { return DbSqlType::SQL_TYPE_CHAR; }

IColumn *CharColumn::Clone() const 
{
	CharColumn *col = new CharColumn(m_name.GetChars(), m_maxlen);
	col->m_data = m_data;
	return col;
}

void CharColumn::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( bool i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( Date i ) { throw new InvalidTypeConversionException(); }
void CharColumn::Append( const String& str ) 
{ 
	struct chars255 data;
	if ( str.Length() >= 255 )
	{
		throw new IndexOutOfBoundsException();
	}
	strcpy(data.chars, str.GetChars());
	m_data.Add(data);
}
void CharColumn::AppendParse( const char *data, const int len) { Append(String(data, len)); }
void CharColumn::Append( void *data, int len ) 
{
	Append( String((char *)data, len) );
}
void CharColumn::Append( IColumn *col, const int row ) { Append(col->GetChar(row)->CharAt(0)); }
void CharColumn::AppendNull() { Append(0); }

int8 CharColumn::GetByte(const int row) { throw new InvalidTypeConversionException(); }
int16 CharColumn::GetInt16(const int row) { throw new InvalidTypeConversionException(); }
int32 CharColumn::GetInt32(const int row) { throw new InvalidTypeConversionException(); }
int64 CharColumn::GetInt64(const int row) { throw new InvalidTypeConversionException(); }
Decimal CharColumn::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 CharColumn::GetFloat32(const int row) { throw new InvalidTypeConversionException(); }
float64 CharColumn::GetFloat64(const int row) { throw new InvalidTypeConversionException(); }
bool CharColumn::GetBit(const int row) { throw new InvalidTypeConversionException(); }
DateTime CharColumn::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date CharColumn::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime CharColumn::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr CharColumn::GetChar(const int row) { return StringPtr(new String(m_data.ElementAt(row).chars)); }
StringPtr CharColumn::GetVarchar(const int row) { return GetChar(row); }
Variant CharColumn::GetVariant(const int row) { return Variant(m_data.ElementAt(row).chars); }

#if defined(DEBUG)
void CharColumn::CheckMem() const { IColumn::CheckMem(); m_data.CheckMem(); }
void CharColumn::ValidateMem() const { IColumn::ValidateMem(); m_data.ValidateMem(); }
#endif

VarCharColumn::VarCharColumn(const String& name, const int maxlen) 
: IColumn(name, maxlen)
{ 
}

VarCharColumn::~VarCharColumn() 
{ 
}

int VarCharColumn::Count() const { return m_data.Count(); }
int VarCharColumn::Type() const { return DbSqlType::SQL_TYPE_CHAR; }

IColumn *VarCharColumn::Clone() const 
{
	VarCharColumn *col = new VarCharColumn(m_name.GetChars(), m_maxlen);
	int count = m_data.Count();
	for ( int x = 0; x < count; x++ )
	{
		col->m_data.Add( StringPtr(new String(*m_data.ElementAt(x))) );
	}
	return col;
}

void VarCharColumn::Append( int8 i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( int16 i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( int32 i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( int64 i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( Decimal i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( float32 i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( float64 i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( bool i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( DateTime i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( Date i ) { throw new InvalidTypeConversionException(); }
void VarCharColumn::Append( const String& str ) { m_data.Add(StringPtr(new String(str))); }
void VarCharColumn::AppendParse( const char *data, const int len) { m_data.Add(StringPtr(new String(data))); }
void VarCharColumn::Append( void *data, int len ) 
{
	Append( String((char *)data, len) );
}
void VarCharColumn::Append( IColumn *col, const int row ) { Append(col->GetVarchar(row)->CharAt(0)); }
void VarCharColumn::AppendNull() { Append(0); }

int8 VarCharColumn::GetByte(const int row) { throw new InvalidTypeConversionException(); }
int16 VarCharColumn::GetInt16(const int row) { throw new InvalidTypeConversionException(); }
int32 VarCharColumn::GetInt32(const int row) { throw new InvalidTypeConversionException(); }
int64 VarCharColumn::GetInt64(const int row) { throw new InvalidTypeConversionException(); }
Decimal VarCharColumn::GetDecimal(const int row) { throw new InvalidTypeConversionException(); }
float32 VarCharColumn::GetFloat32(const int row) { throw new InvalidTypeConversionException(); }
float64 VarCharColumn::GetFloat64(const int row) { throw new InvalidTypeConversionException(); }
bool VarCharColumn::GetBit(const int row) { throw new InvalidTypeConversionException(); }
DateTime VarCharColumn::GetTimeStamp(const int row) { throw new InvalidTypeConversionException(); }
Date VarCharColumn::GetDate(const int row) { throw new InvalidTypeConversionException(); }
DateTime VarCharColumn::GetDateTime(const int row) { throw new InvalidTypeConversionException(); }
StringPtr VarCharColumn::GetChar(const int row) { return m_data.ElementAt(row); }
StringPtr VarCharColumn::GetVarchar(const int row) { return m_data.ElementAt(row); }
Variant VarCharColumn::GetVariant(const int row) { return Variant(*m_data.ElementAt(row)); }

#if defined(DEBUG)
void VarCharColumn::CheckMem() const
{ 
	IColumn::CheckMem(); 
	m_data.CheckMem(); 
	int count = m_data.Count();
	for ( int x = 0; x < m_data.Count(); x++ )
	{
		m_data.ElementAt(x).CheckMem();
	}
}

void VarCharColumn::ValidateMem() const
{ 
	IColumn::ValidateMem(); 
	m_data.ValidateMem(); 
	int count = m_data.Count();
	for ( int x = 0; x < m_data.Count(); x++ )
	{
		m_data.ElementAt(x).ValidateMem();
	}
}
#endif
