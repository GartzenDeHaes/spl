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
#ifndef _datacolumn_h
#define _datacolumn_h

#include <spl/Debug.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/Variant.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

class DbSqlType
{
public:
	enum
	{
		SQL_TYPE_UNASSIGNED = 0,
		SQL_TYPE_INT8 = 1,
		SQL_TYPE_INT16 = 2,
		SQL_TYPE_INT32 = 3,
		SQL_TYPE_INT64 = 4,
		SQL_TYPE_DECIMAL = 5,	/* MYSQL: 65 digit fixed point */
		SQL_TYPE_FLOAT32 = 6,
		SQL_TYPE_FLOAT64 = 7,
		SQL_TYPE_FLAG = 8,
		SQL_TYPE_TIMESTAMP = 9,
		SQL_TYPE_DATE = 10,
		SQL_TYPE_DATETIME = 11,
		SQL_TYPE_CHAR = 12,
		SQL_TYPE_VARCHAR = 13,
		SQL_TYPE_BLOB = 14
	};
};

class DataColumn;
typedef RefCountPtr<DataColumn> DataColumnPtr;

class DataColumn : public IMemoryValidate
{
private:
	String m_name;
	int m_colType;
	int m_dataLen;
	int m_index;

public:
	DataColumn();
	DataColumn(const DataColumn& col);
	DataColumn(const String& name, int index = 0, int colType = DbSqlType::SQL_TYPE_UNASSIGNED, int dataLen = 0);
	virtual ~DataColumn();

	DataColumn& operator =(const DataColumn& col);

	inline String& Name() { return m_name; }
	inline int& Type() { return m_colType; }
	inline int& Length() { return m_dataLen; }
	inline int& Index() { return m_index; }

	DataColumnPtr Clone() const;

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
}
#endif
