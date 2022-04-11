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
#ifndef _columntypes_h
#define _columntypes_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Vector.h>
#include <spl/data/RecordSet.h>
#include <spl/Variant.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

class Int8Column : public IColumn
{
protected:
	Vector<int8> m_data;

public:
	Int8Column(const String& name);
	virtual ~Int8Column();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char *data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class Int16Column : public IColumn
{
protected:
	Vector<int16> m_data;

public:
	Int16Column(const String& name);
	virtual ~Int16Column();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class Int32Column : public IColumn
{
protected:
	Vector<int32> m_data;

public:
	Int32Column(const String& name);
	virtual ~Int32Column();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class Int64Column : public IColumn
{
protected:
	Vector<int64> m_data;

public:
	Int64Column(const String& name);
	virtual ~Int64Column();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class Float32Column : public IColumn
{
protected:
	Vector<float32> m_data;

public:
	Float32Column(const String& name);
	virtual ~Float32Column();
	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class Float64Column : public IColumn
{
protected:
	Vector<float64> m_data;

public:
	Float64Column(const String& name);
	virtual ~Float64Column();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class DecimalColumn : public IColumn
{
protected:
	Vector<Decimal> m_data;

public:
	DecimalColumn(const String& name, const int maxlen);
	virtual ~DecimalColumn();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class BitColumn : public IColumn
{
protected:
	Vector<bool> m_data;

public:
	BitColumn(const String& name);
	virtual ~BitColumn();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class TimeStampColumn : public IColumn
{
protected:
	Vector<DateTime> m_data;

public:
	TimeStampColumn(const String& name);
	virtual ~TimeStampColumn();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class DateColumn : public IColumn
{
protected:
	Vector<Date> m_data;

public:
	DateColumn(const String& name);
	virtual ~DateColumn();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class DateTimeColumn : public IColumn
{
protected:
	Vector<DateTime> m_data;

public:
	DateTimeColumn(const String& name);
	virtual ~DateTimeColumn();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

struct chars255
{
	char chars[255];
};

inline void TypeCheckMem(chars255& uri)
{
}

inline void TypeValidate(chars255& uri)
{
}

class CharColumn : public IColumn
{
protected:
	Vector<struct chars255> m_data;

public:
	CharColumn(const String& name, const int maxlen);
	virtual ~CharColumn();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

class VarCharColumn : public IColumn
{
protected:
	Vector<StringPtr> m_data;

public:
	VarCharColumn(const String& name, const int maxlen);
	virtual ~VarCharColumn();

	virtual int Count() const;
	virtual int Type() const;
	virtual IColumn *Clone() const;

	virtual void Append( int8 i );
	virtual void Append( int16 i );
	virtual void Append( int32 i );
	virtual void Append( int64 i );
	virtual void Append( Decimal i );
	virtual void Append( float32 i );
	virtual void Append( float64 i );
	virtual void Append( bool i );
	virtual void Append( DateTime i );
	virtual void Append( Date i );
	virtual void Append( const String& str );
	virtual void AppendParse( const char * data, const int len);
	virtual void Append( void *data, int len );
	virtual void Append( IColumn *col, const int row );
	virtual void AppendNull(  );

	virtual int8 GetByte(const int row);
	virtual int16 GetInt16(const int row);
	virtual int32 GetInt32(const int row);
	virtual int64 GetInt64(const int row);
	virtual Decimal GetDecimal(const int row);
	virtual float32 GetFloat32(const int row);
	virtual float64 GetFloat64(const int row);
	virtual bool GetBit(const int row);
	virtual DateTime GetTimeStamp(const int row);
	virtual Date GetDate(const int row);
	virtual DateTime GetDateTime(const int row);
	virtual StringPtr GetChar(const int row);
	virtual StringPtr GetVarchar(const int row);
	virtual Variant GetVariant(const int row);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
}
#endif
