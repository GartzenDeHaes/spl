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
#ifndef _datatable_h
#define _datatable_h

#include <spl/Debug.h>
#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/data/DataColumn.h>
#include <spl/data/DataRow.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

/** @brief A table of variant data.
 *
 */
class DataTable : public IMemoryValidate
{
private:
	String m_name;
	Vector<DataRowPtr> m_rows;
	Vector<DataColumnPtr> m_cols;
	Hashtable<String, DataColumnPtr> m_colIdx;

public:
	DataTable();
	DataTable(const String& name);
	DataTable(const DataTable& dt);
	virtual ~DataTable();

	DataTable& operator =(const DataTable& dt);

	inline String& Name() { return m_name; }
	
	inline int RowCount() const { return m_rows.Count(); }

	inline DataRowPtr Row(int rowNum) const { return m_rows.ElementAt(rowNum); }

	inline VariantPtr Cell(int rowNum, int colNum) const { return m_rows.ElementAt(rowNum)->Cell(colNum); }

	inline VariantPtr Cell(int rowNum, const String& colName) const
	{ 
		return m_rows.ElementAt(rowNum)->Cell(m_colIdx.Get(colName)->Index()); 
	}

	inline DataColumnPtr Column(const String& name) const
	{
		return m_colIdx.Get(name);
	}

	inline DataColumnPtr Column(int index) const
	{
		return m_cols.ElementAt(index);
	}

	inline int ColumnCount() const { return m_cols.Count(); }

	inline void Clear() { m_cols.Clear(); m_rows.Clear(); m_colIdx.Clear(); m_name = ""; }

	DataRowPtr AddRow();
	void AddRow(DataRowPtr row);

	DataColumnPtr AddColumn(const String& name);
	void AddColumn(DataColumnPtr col);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
}
#endif
