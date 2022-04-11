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
#include <spl/io/StringStream.h>

using namespace spl;

StringStream::StringStream(const String& str)
{
	//Write(str.ToByteArray());
	Write(str);
}

StringStream::~StringStream()
{
}

void StringStream::Write(const String& data)
{
	int len = data.Length();

	for ( int x = 0; x < len; x++)
	{
		m_buf.Add( (byte)data[x] );
	}
}
