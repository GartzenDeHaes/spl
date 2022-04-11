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
#ifndef _datarow_h
#define _datarow_h

#include <spl/data/DataColumn.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/Variant.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

class DataRow;
typedef RefCountPtr<DataRow> DataRowPtr;

/** @brief A row in a DataTable.
 *	@ref DataTable
 */
class DataRow : public IMemoryValidate
{
private:
	Vector<VariantPtr> m_cols;

public:
	DataRow();
	DataRow(const DataRow& dr);
	DataRow(const Vector<DataColumnPtr>& columns);
	virtual ~DataRow();

	DataRow& operator =(const DataRow& dt);

	inline int Count() const { return m_cols.Count(); }

	inline VariantPtr Cell(int colNum) const { return m_cols.ElementAt(colNum); }

	void AddColumn(const DataColumn& col);
	void AddColumn(const DataColumn& col, const Variant& data);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
}
#endif
