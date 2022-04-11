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

/** @file DelimitedFile.cpp
 *  @brief DelimitedFile implementation.
 */

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/io/DelimitedFile.h>
#include <spl/io/File.h>
#include <spl/io/StreamBuffer.h>

using namespace spl;

class DelimitedFileRowParser : public IMemoryValidate
{
private:
	// copy constructor doesn't make sense for this class
	inline DelimitedFileRowParser(const DelimitedFileRowParser& csv) : m_reader(spl::IStreamPtr()) {}

protected:
	char m_delimchar;
	Vector<StringBuffer> m_cols;
	TextReader m_reader;
	StringBuffer m_line;

public:
	DelimitedFileRowParser(char delimchar, spl::IStreamPtr strm);
	virtual ~DelimitedFileRowParser();

	bool Next();

	bool RowHasData() const;
	inline int ColCount() const { return m_cols.Count(); }
	inline const StringBuffer& CellAt(int col) { return m_cols.ElementAtRef(col); }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

DelimitedFile::DelimitedFile(  )
: m_table()
{
}

DelimitedFile::DelimitedFile(const DelimitedFile& csv)
: m_table()
{
	*this = csv;
}


DelimitedFile::~DelimitedFile( )
{
	Clear();
}

DelimitedFile& DelimitedFile::operator =(const DelimitedFile& csv)
{
	Clear();
	m_table = csv.m_table;
	return *this;
}

void DelimitedFile::Clear()
{
	m_table.Clear();
}

DataRowPtr DelimitedFile::RowAt(int idx) const
{ 
	if ( idx >= m_table.RowCount() )
	{
		return DataRowPtr();
	}
	return m_table.Row(idx);
}

DataRowPtr DelimitedFile::operator[] (int idx) const
{
	return m_table.Row(idx);
}

bool DelimitedFile::RowHasData(int rowNum) const
{
	DataRowPtr row = m_table.Row(rowNum);

	int count = row->Count();
	for ( int x = 0; x < count; x++ )
	{
		if ( ! row->Cell(x)->IsUndefined() )
		{
			return true;
		}
	}
	return false;
}

DelimitedFilePtr DelimitedFile::Parse( TextReader& reader, char coldelim )
{
	Array<byte> buf(512);
	DelimitedFilePtr dfile = DelimitedFilePtr(new DelimitedFile());
	int lineLen;

	while ( reader.ReadLine(buf, lineLen) )
	{
		reader.ValidateMem();
		DataRowPtr row = Parse(buf, lineLen - 1, coldelim);
		dfile->AddRow( row );
	}
	reader.Close();

	dfile.ValidateMem();
	return dfile;
}

#ifdef DEBUG
void DelimitedFile::ValidateMem() const
{
	m_table.ValidateMem();
}

void DelimitedFile::CheckMem() const
{
	m_table.CheckMem();
}
#endif

DelimitedFilePtr DelimitedFile::Parse( const String& filename, char coldelim )
{
	if ( ! File::Exists(filename) )
	{
		return DelimitedFilePtr();
	}

	IStreamPtr fs = File::OpenText(filename);
	TextReader reader(StreamBufferPtr(new StreamBuffer(fs, true)));
	DelimitedFilePtr df;

	try
	{
		df = Parse(reader, coldelim);
	}
	catch (Exception *ex)
	{
		reader.Close();
		throw ex;
	}

	// reader is closed by Parse.
	return df;
}

static enum DilimitedRowParseState
{
	DRP_STATE_CHARS,
	DRP_STATE_QUOTE,
	DRP_STATE_QUOTE_COMMA,

} DilimitedRowParseState;

DataRowPtr DelimitedFile::Parse( Array<byte>& cstr, int cstrLen, char coldelim )
{
	DataColumnPtr col(new DataColumn("dummy"));
	DataRowPtr row = DataRowPtr(new DataRow());
	enum DilimitedRowParseState state = DRP_STATE_CHARS;

	bool trailingComma = false;
	int start = 0;
	int len = cstrLen;
	for ( int x = 0; x < len; x++ )
	{
		char ch = cstr[x];
		switch ( state )
		{
		case DRP_STATE_CHARS:
			if ( start == x && ch == '"' )
			{
				state = DRP_STATE_QUOTE;
				start = x + 1;
				break;
			}
			trailingComma = false;
			if ( ch == coldelim )
			{
				int cplen = x - start;
				row->AddColumn(col, VariantPtr(new Variant(String(cstr, start, cplen))));

				start = x + 1;
				trailingComma = true;
			}
			break;
		case DRP_STATE_QUOTE:
			if ( ch == '"' )
			{
				int cplen = x - start;
				row->AddColumn(col, VariantPtr(new Variant(String(cstr, start, cplen))));

				state = DRP_STATE_QUOTE_COMMA;
				trailingComma = false;
			}
			break;
		case DRP_STATE_QUOTE_COMMA:
			if ( ch == ',' )
			{
				start = x + 1;
				state = DRP_STATE_QUOTE;
				trailingComma = true;
			}
			break;
		}
	}
	int cplen = len - start;
	if ( cplen > 0 || trailingComma )
	{
		row->AddColumn(col, VariantPtr(new Variant(String(cstr, start, cplen))));
	}

	row.ValidateMem();
	return row;
}

DelimitedFileRowParser::DelimitedFileRowParser(char delimchar, spl::IStreamPtr strm)
: m_cols(), m_reader(strm), m_delimchar(delimchar), m_line(121)
{
}

DelimitedFileRowParser::~DelimitedFileRowParser()
{
	m_reader.Close();
}

bool DelimitedFileRowParser::RowHasData() const
{
	int colcount = m_cols.Count();
	for ( int x = 0; x < colcount; x++ )
	{
		if ( m_cols.ElementAtRef(x).Length() > 0 )
		{
			return true;
		}
	}
	return false;
}

bool DelimitedFileRowParser::Next()
{
	int x;
	int colcount = m_cols.Count();
	for ( x = 0; x < colcount; x++ )
	{
		m_cols.ElementAtRef(x).Clear();
	}

	m_line.SetLength(0);
	if ( ! m_reader.ReadLine(m_line) )
	{
		return false;
	}

	if ( 0 == m_cols.Count() )
	{
		StringBuffer sb;
		m_cols.Add(sb);
	}

	int curcol = 0;

	enum DilimitedRowParseState state = DRP_STATE_CHARS;
	bool trailingComma = false;
	int len = m_line.Length();
	for ( x = 0; x < len; x++ )
	{
		char ch = m_line.CharAt(x);
		switch ( state )
		{
		case DRP_STATE_CHARS:
			if ( ch == '"' )
			{
				state = DRP_STATE_QUOTE;
				break;
			}
			trailingComma = false;
			if ( ch == m_delimchar )
			{
				curcol++;
				if ( curcol >= m_cols.Count() )
				{
					StringBuffer sb;
					m_cols.Add(sb);
				}
				trailingComma = true;
			}
			else
			{
				m_cols.ElementAtRef(curcol).Append( ch );
			}
			break;
		case DRP_STATE_QUOTE:
			if ( ch == '"' )
			{
				curcol++;
				if ( curcol >= m_cols.Count() )
				{
					StringBuffer sb;
					m_cols.Add(sb);
				}
				state = DRP_STATE_QUOTE_COMMA;
				trailingComma = false;
			}
			break;
		case DRP_STATE_QUOTE_COMMA:
			if ( ch == ',' )
			{
				state = DRP_STATE_QUOTE;
				trailingComma = true;
			}
			break;
		}
	}
	return true;
}

#ifdef DEBUG
void DelimitedFileRowParser::ValidateMem() const
{
	m_reader.ValidateMem();
	m_cols.ValidateMem();
}

void DelimitedFileRowParser::CheckMem() const
{
	m_reader.CheckMem();
	m_cols.CheckMem();
}
#endif
