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
#include <spl/GUID.h>
#include <spl/math/Math.h>
#include <spl/text/StringBuffer.h>
#include <spl/UInt32.h>
#include <spl/UInt64.h>

using namespace spl;

Guid::Guid()
{
	data1 = Math::RandomInt();
	data2 = Math::RandomInt() & 0xFFFF;
	data3 = Math::RandomInt() & 0xFFFF;
	*(int *)&(((byte *)&data4)[4]) = Math::RandomInt();
	*(int *)&(((byte *)&data4)[0]) = Math::RandomInt();
	
	ASSERT(sizeof(data4) == 8);
	ASSERT(0 != data4);
}

Guid::Guid( const Array<byte>& bytes )
{
	ASSERT(bytes.Length() == GUID_SIZE_BYTES);

	data1 = *((uint32 *)&bytes.Data()[0]);
	data2 = *((uint16 *)&bytes.Data()[4]);
	data3 = *((uint16 *)&bytes.Data()[6]);
	data4 = *((uint64 *)&bytes.Data()[8]);
}

Guid::Guid( const Guid& guid )
{
	data1 = guid.data1;
	data2 = guid.data2;
	data3 = guid.data3;
	data4 = guid.data4;
}

Guid::~Guid()
{
}

void Guid::GetBytes( Array<byte>& bytes ) const
{
	ASSERT(bytes.Length() == GUID_SIZE_BYTES);

	memcpy(&bytes[0], &data1, 4);
	memcpy(&bytes[4], &data2, 2);
	memcpy(&bytes[6], &data3, 2);
	memcpy(&bytes[8], &data4, 8);
}

Array<byte> Guid::GetBytes() const
{
	Array<byte> bytes(16);
	GetBytes(bytes);
	return bytes;
}

StringPtr Guid::ToBase64()
{
	byte data[16];
	memcpy(&data[0], &data1, 4);
	memcpy(&data[4], &data2, 2);
	memcpy(&data[6], &data3, 2);
	memcpy(&data[8], &data4, 8);

	return String::Base64Encode( (char *)data, 16 );
}

StringPtr Guid::ToHex()
{
	StringBuffer sbuf(40);
	sbuf.Append( UInt32::ToString(data1, 16) );
	sbuf.Append( '-' );
	sbuf.Append( UInt32::ToString(data2, 16) );
	sbuf.Append( '-' );
	sbuf.Append( UInt32::ToString(data3, 16) );
	sbuf.Append( '-' );
	StringPtr s16(UInt64::ToString(data4, 16));
	sbuf.Append( s16 );

	return sbuf.ToString();
}

Guid Guid::ParseBase64(const String& str)
{
	RefCountPtr<Array<byte> > bytes = String::Base64Decode(str);
	ASSERT( bytes->Length() == Guid::Length() );
	return Guid(bytes);
}

Guid Guid::ParseHex(const String& str)
{
	throw new NotImplementedException();
}
