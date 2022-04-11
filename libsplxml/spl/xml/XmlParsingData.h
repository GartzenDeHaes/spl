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
#ifndef _xmlparsingdata_h
#define _xmlparsingdata_h

#include <spl/Debug.h>

namespace spl
{
	// Used by the parsing routines.
	enum XmlEncoding
	{
		TIXML_ENCODING_UNKNOWN,
		TIXML_ENCODING_UTF8,
		TIXML_ENCODING_LEGACY
	};

	const XmlEncoding TIXML_DEFAULT_ENCODING = TIXML_ENCODING_UNKNOWN;

	/*	Internal structure for tracking location of items 
		in the XML file.
	*/
	struct TiXmlCursor
	{
		TiXmlCursor()		{ Clear(); }
		void Clear()		{ row = col = -1; }

		int row;	// 0 based.
		int col;	// 0 based.
	};

	extern void _XmlEncodeString( const String& str, StringBuffer& outString );

	class XmlDocument;

	class XmlParsingData
	{
		friend class XmlDocument;
		
	public:
		void Stamp( const char* now, XmlEncoding encoding );

		const TiXmlCursor& Cursor()	{ return cursor; }

		XmlParsingData( const char* start, int _tabsize, int row, int col, bool _condenseWs )
		{
			assert( start );
			stamp = start;
			tabsize = _tabsize;
			cursor.row = row;
			cursor.col = col;
			condenseWs = _condenseWs;
		}

	private:
		TiXmlCursor		cursor;
		const char*		stamp;
		int				tabsize;
		bool			condenseWs;
	};
}

#endif
