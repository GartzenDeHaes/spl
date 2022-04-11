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
#ifndef _delimfile_h
#define _delimfile_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <stdio.h>
#include <spl/data/DataTable.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/io/Stream.h>
#include <spl/text/StringBuffer.h>
#include <spl/io/TextReader.h>

namespace spl
{
/** 
 * @defgroup file File System
 * @ingroup io
 * @{
 */

class DelimitedFile;
typedef RefCountPtr<DelimitedFile> DelimitedFilePtr;

/** @brief Loads delimited file into memory.
 *	@ref DataTable
 */
class DelimitedFile : public IMemoryValidate
{
protected:
	DataTable m_table;

	DelimitedFile(  );

	static DataRowPtr Parse( Array<byte>& cstr, int cstrLen, char coldelim );

public:
	DelimitedFile(const DelimitedFile& csv);
	virtual ~DelimitedFile();

	DelimitedFile& operator =(const DelimitedFile& csv);

	inline int RowCount() const { return m_table.RowCount(); }
	inline void AddRow( DataRowPtr row ) { m_table.AddRow(row); }
	DataRowPtr operator[] (int idx) const;
	DataRowPtr RowAt(int idx) const;
	inline VariantPtr CellAt( int row, int col ) const { return RowAt(row)->Cell(col); }

	bool RowHasData(int row) const;

	void Clear();

	static DelimitedFilePtr Parse( TextReader& reader, char coldelim );
	static DelimitedFilePtr Parse( const String& filename, char coldelim );

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
