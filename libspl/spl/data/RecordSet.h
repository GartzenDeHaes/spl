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
#ifndef _cpprecordset_h
#define _cpprecordset_h

#include <spl/data/DataColumn.h>
#include <spl/DateTime.h>
#include <spl/Decimal.h>
#include <spl/collection/Hashtable.h>
#include <spl/IIterator.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/Variant.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

class IColumn : public IMemoryValidate
{
private:
	int m_currow;

protected:
	String m_name;
	int m_maxlen;

public:
	IColumn(const String& name, const int maxlen);
	virtual ~IColumn();

	virtual IColumn *Clone() const = 0;

	inline void BeginInter() { m_currow = -1; };
	inline bool Next() { if ( ++m_currow >= Count()) return false; return true; }
	inline void SeekRow(const int row) { if (row >= Count()) throw new InvalidArgumentException("EOF"); m_currow = row; }
	virtual int Count() const = 0;
	
	virtual int Type() const = 0;
	inline const String &Name() const { return m_name; }
	inline int MaxLength() const { return m_maxlen; }

	virtual void Append( int8 i ) = 0;
	virtual void Append( int16 i ) = 0;
	virtual void Append( int32 i ) = 0;
	virtual void Append( int64 i ) = 0;
	virtual void Append( Decimal i ) = 0;
	virtual void Append( float32 i ) = 0;
	virtual void Append( float64 i ) = 0;
	virtual void Append( bool i ) = 0;
	virtual void Append( DateTime i ) = 0;
	virtual void Append( Date i ) = 0;
	virtual void Append( const String& str ) = 0;
	virtual void AppendParse( const char *data, const int len) = 0;
	virtual void Append( void *data, int len ) = 0;
	virtual void Append( IColumn *col, const int row ) = 0;
	virtual void AppendNull() = 0;

	inline int8 GetByte() { return GetByte(m_currow); }
	inline int16 GetInt16() { return GetInt16(m_currow); }
	inline int32 GetInt32() { return GetInt32(m_currow); }
	inline int64 GetInt64() { return GetInt64(m_currow); }
	inline Decimal GetDecimal() { return GetDecimal(m_currow); }
	inline float32 GetFloat32() { return GetFloat32(m_currow); }
	inline float64 GetFloat64() { return GetFloat64(m_currow); }
	inline bool GetBit() { return GetBit(m_currow); }
	inline DateTime GetTimeStamp() { return GetTimeStamp(m_currow); }
	inline Date GetDate() { return GetDate(m_currow); }
	inline DateTime GetDateTime() { return GetDateTime(m_currow); }
	inline StringPtr GetChar() { return GetChar(m_currow); }
	inline StringPtr GetVarchar() { return GetVarchar(m_currow); }
	inline Variant GetVariant() { return GetVariant(m_currow); }

	virtual int8 GetByte(const int row) = 0;
	virtual int16 GetInt16(const int row) = 0;
	virtual int32 GetInt32(const int row) = 0;
	virtual int64 GetInt64(const int row) = 0;
	virtual Decimal GetDecimal(const int row) = 0;
	virtual float32 GetFloat32(const int row) = 0;
	virtual float64 GetFloat64(const int row) = 0;
	virtual bool GetBit(const int row) = 0;
	virtual DateTime GetTimeStamp(const int row) = 0;
	virtual Date GetDate(const int row) = 0;
	virtual DateTime GetDateTime(const int row) = 0;
	virtual StringPtr GetChar(const int row) = 0;
	virtual StringPtr GetVarchar(const int row) = 0;
	virtual Variant GetVariant(const int row) = 0;

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

inline void TypeValidate( IColumn *col )
{
	if ( NULL != col )
	{
		ASSERT_PTR( col );
		col->ValidateMem();
	}
}

inline void TypeCheckMem( IColumn *col )
{
	if ( NULL != col )
	{
		DEBUG_NOTE_MEM( col );
		col->CheckMem();
	}
}

class RecordSet;
typedef RefCountPtr<RecordSet> RecordSetPtr;

/** @brief In memory reocrd set with typed columns (no variants).
 *	@ref DataTable
 *	@ref Command
 */
class RecordSet : public IMemoryValidate
{
protected:
	Hashtable<String, IColumn *> m_columnIdx;
	Vector<IColumn *> m_columns;

public:
	RecordSet();
	RecordSet(const RecordSet& rs);
	~RecordSet();

	RecordSet& operator =(const RecordSet& rs);

	void SetCurrentRow(const int row);
	inline int RowCount() const { return (0 == m_columns.Count()) ? 0 : m_columns.ElementAt(0)->Count(); }
	void BeginIter();
	bool Next();

	void Clear();

	void DefineColumn(const String& name, int type, int fieldmaxlen);
	void SetColumns( RecordSet& rs );
	inline IColumn *GetColumn(const String& name) { return m_columnIdx.Get(String(name)); }
	inline IColumn *GetColumn(int idx) { return m_columns.ElementAt(idx); }
	
	inline int ColumnCount() const { return m_columns.Count(); }

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
}
#endif
