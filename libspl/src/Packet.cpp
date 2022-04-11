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
#include <stdlib.h>

#include <spl/net/Packet.h>
#include <spl/net/PacketBuilder.h>
#include <spl/text/StringBuffer.h>
#include <spl/io/ThreadedPacketStream.h>

using namespace spl;

PacketListenerDelegateDispatch::~PacketListenerDelegateDispatch()
{
}

void PacketListenerDelegateDispatch::Add(IPacketListener *l)
{
	m_onDataInt64 += DelegateOneParameter<IPacketListener, int64>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataInt32 += DelegateOneParameter<IPacketListener, int32>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataInt16 += DelegateOneParameter<IPacketListener, int16>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataByte += DelegateOneParameter<IPacketListener, byte>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataBool += DelegateOneParameter<IPacketListener, bool>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataDouble += DelegateOneParameter<IPacketListener, double>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataString += DelegateOneParameter<IPacketListener, StringPtr>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataBin += DelegateTwoParameter<IPacketListener, const Vector<byte>&, const int>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataDtm += DelegateOneParameter<IPacketListener, const DateTime&>::Create(l, &IPacketListener::IPacket_OnData);
	m_onDataDate += DelegateOneParameter<IPacketListener, const Date&>::Create(l, &IPacketListener::IPacket_OnData);
	m_onError += DelegateOneParameter<IPacketListener, const String&>::Create(l, &IPacketListener::IStreamRead_OnError);
	m_onClose += Delegate<IPacketListener>::Create(l, &IPacketListener::IStreamRead_OnClose);
}

#ifdef DEBUG
void PacketListenerDelegateDispatch::ValidateMem() const
{
	m_onDataInt64.ValidateMem();
	m_onDataInt32.ValidateMem();
	m_onDataInt16.ValidateMem();
	m_onDataByte.ValidateMem();
	m_onDataBool.ValidateMem();
	m_onDataDouble.ValidateMem();
	m_onDataString.ValidateMem();
	m_onDataBin.ValidateMem();
	m_onDataDtm.ValidateMem();
	m_onDataDate.ValidateMem();
	m_onError.ValidateMem();
	m_onClose.ValidateMem();
}

void PacketListenerDelegateDispatch::CheckMem() const
{
	m_onDataInt64.CheckMem();
	m_onDataInt32.CheckMem();
	m_onDataInt16.CheckMem();
	m_onDataByte.CheckMem();
	m_onDataBool.CheckMem();
	m_onDataDouble.CheckMem();
	m_onDataString.CheckMem();
	m_onDataBin.CheckMem();
	m_onDataDtm.CheckMem();
	m_onDataDate.CheckMem();
	m_onError.CheckMem();
	m_onClose.CheckMem();
}
#endif

Packet::Packet()
:	m_buf(), 
	m_readPacketReady(false),
	m_abuf(1)
{
	Clear();
}

Packet::Packet(const Packet& pkt)
:	m_buf(pkt.m_buf), 
	m_readPacketReady(pkt.m_readPacketReady), 
	m_readpos(pkt.m_readpos),
	m_abuf(1)
{
}

Packet::~Packet()
{
}

Packet& Packet::operator =(const Packet& pkt)
{
	Clear();

	m_buf = pkt.m_buf;
	m_readPacketReady = pkt.m_readPacketReady;
	m_readpos = pkt.m_readpos;

	return *this;
}

void Packet::Clear()
{
	m_buf.Clear();
	m_readpos = 0;
#if LITTLE_ENDIAN
	m_buf.Add(1);
#else
	m_buf.Add(0);
#endif
	m_buf.Add(0);	// byte count MCB
	m_buf.Add(0);	// byte count LCB

	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::SendPacket(spl::IStream& sock)
{
	int16 count = m_buf.Count();
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
#if LITTLE_ENDIAN
	m_buf.SetElementAt( count >> 8, 1 );
	m_buf.SetElementAt( count & 0xFF, 2 );
#else
	m_buf.SetElementAt( count & 0xFF, 1 );
	m_buf.SetElementAt( count >> 8, 2 );
#endif
	sock.Write( m_buf.ToArray(), 0, m_buf.Count() );
}

void Packet::ReadPacket(spl::IStream& sock)
{
	m_readPacketReady = false;

	m_buf.Clear();
	if (1 != sock.Read(m_abuf, 0, 1) )
	{
		throw new SocketException("Read byte order failed");
	}
	m_buf.Add(m_abuf[0]);
#if LITTLE_ENDIAN
	m_readRevByteOrder = m_abuf[0] != 1;
#else
	m_readRevByteOrder = m_abuf[0] == 1;
#endif
	if (1 != sock.Read(m_abuf, 0, 1) )
	{
		throw new SocketException("count MCB failed");
	}
	m_buf.Add(m_abuf[0]);
#if LITTLE_ENDIAN
	m_rpacketsize = m_abuf[0] << 8;
#else
	m_rpacketsize = m_abuf[0] & 0xFF;
#endif
	if (1 != sock.Read(m_abuf, 0, 1) )
	{
		throw new SocketException("count LCB failed");
	}
	m_buf.Add(m_abuf[0]);
	ASSERT(m_buf.Count() == 3);
#if LITTLE_ENDIAN
	m_rpacketsize |= m_abuf[0] & 0xFF;
#else
	m_rpacketsize |= m_abuf[0] << 8;
#endif

	int len2 = m_rpacketsize - m_buf.Count();
	if (m_abuf.Length() < len2)
	{
		m_abuf = Array<byte>(len2);
	}
	
	if ( len2 != sock.Read( m_abuf, 0, m_rpacketsize-m_buf.Count()) )
	{
		throw new SocketException("Short read");
	}
	m_buf.AddRange(m_abuf, len2);
	m_buf.SetCount(m_rpacketsize);
	m_readpos = 3;
	m_readPacketReady = true;
}

void Packet::_AppendRaw( byte i )
{
	m_buf.Add( i );
}

void Packet::_AppendRaw( int16 i )
{
#if LITTLE_ENDIAN
	m_buf.Add( (byte)((i >> 8) & 0xFF) );
	m_buf.Add( (byte)(i & 0xFF) );
#else
	m_buf.Add( (byte)(i & 0xFF) );
	m_buf.Add( (byte)((i >> 8) & 0xFF) );
#endif
}

void Packet::_AppendRaw( int32 i )
{
#if LITTLE_ENDIAN
	m_buf.Add( (byte)(i >> 24) );
	m_buf.Add( (byte)((i >> 16) & 0xFF) );
	m_buf.Add( (byte)((i >> 8) & 0xFF) );
	m_buf.Add( (byte)(i & 0xFF) );
#else
	m_buf.Add( (byte)(i & 0xFF) );
	m_buf.Add( (byte)((i >> 8) & 0xFF) );
	m_buf.Add( (byte)((i >> 16) & 0xFF) );
	m_buf.Add( (byte)(i >> 24) );
#endif
}

void Packet::_AppendRaw( int64 i )
{
#if LITTLE_ENDIAN
	m_buf.Add( (byte)(i >> 56) & 0xFF );
	m_buf.Add( (byte)(i >> 48) & 0xFF );
	m_buf.Add( (byte)(i >> 40) & 0xFF );
	m_buf.Add( (byte)(i >> 32) & 0xFF );
	m_buf.Add( (byte)(i >> 24) & 0xFF );
	m_buf.Add( (byte)((i >> 16) & 0xFF) );
	m_buf.Add( (byte)((i >> 8) & 0xFF) );
	m_buf.Add( (byte)(i & 0xFF) );
#else
	m_buf.Add( (byte)(i & 0xFF) );
	m_buf.Add( (byte)((i >> 8) & 0xFF) );
	m_buf.Add( (byte)((i >> 16) & 0xFF) );
	m_buf.Add( (byte)(i >> 24) & 0xFF );
	m_buf.Add( (byte)(i >> 32) & 0xFF );
	m_buf.Add( (byte)(i >> 40) & 0xFF );
	m_buf.Add( (byte)(i >> 48) & 0xFF );
	m_buf.Add( (byte)(i >> 56) & 0xFF );
#endif
}

void Packet::Append(int64 i)
{
	m_buf.Add( (byte)'L' );
	_AppendRaw( i );
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::Append(int32 i)
{
	m_buf.Add( (byte)'I' );
	_AppendRaw( i );
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::Append(int16 i)
{
	m_buf.Add( (byte)'X' );
	_AppendRaw( i );
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::Append(byte i)
{
	m_buf.Add( (byte)'B' );
	m_buf.Add( i );
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::Append(bool b)
{
	m_buf.Add( (byte)'F' );
	m_buf.Add( (b)?1:0 );
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::Append(const char *str, int count)
{
	// append datatype
	m_buf.Add( (byte)'S' );

	// append the number of characters
	int numchar = count;
	_AppendRaw( (int16)count );

	// append the size of each char
	byte charsize = (byte)sizeof(char);
	_AppendRaw( charsize );
	
	if ( 1 == charsize )
	{
		for ( int x = 0; x < numchar; x++ )
		{
			m_buf.Add( (byte)str[x] );
		}
	}
	else if ( 2 == charsize )
	{
		for ( int x = 0; x < numchar; x++ )
		{
			_AppendRaw( (int16)str[x] );
		}
	}
	else
	{
		for ( int x = 0; x < numchar; x++ )
		{
			_AppendRaw( (int32)str[x] );
		}
	}
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::Append(const Array<byte>& buf, int start, int len)
{
	_AppendRaw( (byte)'R' );
	_AppendRaw( (int16)(len - start) );

	for ( int x = start; x < start+len; x++ )
	{
		_AppendRaw( buf[x] );
	}
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

//void Packet::Append(float f)
//{
//	char buf[128];
//	int numchars = sprintf(buf, "%f.12", f);
//	ASSERT(numchars < 128);
//	
//	_AppendRaw( (byte)'f' );
//	_AppendRaw( (byte)numchars );
//
//	for ( int x = 0; x < numchars; x++ )
//	{
//		m_buf.Add( (byte)buf[x] );
//	}
//	ASSERT(0 == m_buf.ElementAt(1));
//	ASSERT(0 == m_buf.ElementAt(2));
//}

void Packet::Append(double d)
{
	char buf[128];
	int numchars = sprintf(buf, "%lf.20", d);
	ASSERT(numchars < 128);
	
	_AppendRaw( (byte)'d' );
	_AppendRaw( (byte)numchars );

	for ( int x = 0; x < numchars; x++ )
	{
		m_buf.Add( (byte)buf[x] );
	}
	ASSERT(0 == m_buf.ElementAt(1));
	ASSERT(0 == m_buf.ElementAt(2));
}

void Packet::Append(const DateTime& dtm)
{
	_AppendRaw( (byte)'T' );
	_AppendRaw( (int16)dtm.Year() );
	_AppendRaw( (byte)dtm.Month() );
	_AppendRaw( (byte)dtm.Day() );
	_AppendRaw( (byte)dtm.Hour() );
	_AppendRaw( (byte)dtm.Minutes() );
	_AppendRaw( (byte)dtm.Seconds() );
}

void Packet::Append(const Date& dt)
{
	_AppendRaw( (byte)'D' );
	_AppendRaw( (int16)dt.Year() );
	_AppendRaw( (byte)dt.Month() );
	_AppendRaw( (byte)dt.Day() );
}

int Packet::PacketSize()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	return m_buf.Count();
}

int64 Packet::ReadInt64()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'L' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected int64");
	}
#if LITTLE_ENDIAN
	int64 i = (_NextByte() << 24) | (_NextByte() << 16) | (_NextByte() << 8) | (_NextByte());
	i <<= 32;
	return i | ((_NextByte() << 24) | (_NextByte() << 16) | (_NextByte() << 8) | (_NextByte()));
#else
	int64 i = _NextByte() | (_NextByte() << 8) | (_NextByte() << 16) | (_NextByte() << 24);
	int64 i2 = _NextByte() | (_NextByte() << 8) | (_NextByte() << 16) | (_NextByte() << 24);
	return (i2 << 32) | i;
#endif
}

int32 Packet::ReadInt32()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'I' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected int32");
	}
#if LITTLE_ENDIAN
	return (_NextByte() << 24) | (_NextByte() << 16) | (_NextByte() << 8) | (_NextByte());
#else
	return _NextByte() | (_NextByte() << 8) | (_NextByte() << 16) | (_NextByte() << 24);
#endif
}

int16 Packet::ReadInt16()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'X' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected int16");
	}
#if LITTLE_ENDIAN
	return (int16)((_NextByte() << 8) | (_NextByte()));
#else
	return (int16)(_NextByte() | (_NextByte() << 8));
#endif
}

byte Packet::ReadInt8()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'B' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected byte");
	}
	return (byte)_NextByte();
}

bool Packet::ReadBool()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'F' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected bool");
	}
	return _NextByte() != 0;
}

StringPtr Packet::ReadString()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'S' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected string type char");
	}

	int16 numchars = (_NextByte() << 8) | _NextByte();
	int charsize = (byte)_NextByte();

	StringBuffer buf(80);

	if ( charsize == 1 )
	{
		for ( int x = 0; x < numchars; x++ )
		{
			buf.Append( (char)_NextByte() );
		}
	}
	else if ( charsize == 2 )
	{
		for ( int x = 0; x < numchars; x++ )
		{
			buf.Append( (char)_NextByte() );
			_NextByte();
		}
	}
	else
	{
		for ( int x = 0; x < numchars; x++ )
		{
			buf.Append( (char)_NextByte() );
			_NextByte();
			_NextByte();
			_NextByte();
		}
	}
	buf.Append('\0');
	buf.ValidateMem();
	return buf.ToString();
}

byte *Packet::ReadBuf(int expectedlen)
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'R' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected raw data");
	}
	int16 len = ReadInt16();
	if ( len != expectedlen )
	{
		throw new PacketUnderflowException();
	}
	ASSERT( 0 != len );
	if ( len > m_buf.Count() - m_readpos )
	{
		throw new PacketUnderflowException();
	}
	byte *buf = (byte *)malloc( len );
	if ( NULL == buf )
	{
		return NULL;
	}
	for ( int x = 0; x < len; x++ )
	{
		buf[x] = _NextByte();
	}
	return buf;
}

//float Packet::ReadFloat()
//{
//	if ( ! m_readPacketReady )
//	{
//		throw new PacketNotReadyException("Packet must be in read state to get size");
//	}
//	byte datatype = _NextByte();
//	if ( (byte)'f' != datatype )
//	{
//		throw new PacketReadTypeMismatchException("Expected float");
//	}
//	int len = _NextByte();
//	StringBuffer buf(len+1);
//
//	for ( int x = 0; x < len; x++ )
//	{
//		buf.Append( (char)_NextByte() );
//	}
//	buf.Append('\0');
//	float val;
//	sscanf(buf.GetChars(), "%f", &val);
//	return val;
//}

double Packet::ReadDouble()
{
	if ( ! m_readPacketReady )
	{
		throw new PacketNotReadyException("Packet must be in read state to get size");
	}
	byte datatype = _NextByte();
	if ( (byte)'d' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected double");
	}
	int len = _NextByte();
	StringBuffer buf(len+1);

	for ( int x = 0; x < len; x++ )
	{
		buf.Append( (char)_NextByte() );
	}
	buf.Append('\0');
	double val;
	sscanf(buf.GetChars(), "%lf", &val);
	return val;
}

DateTime Packet::ReadDateTime()
{
	byte datatype = _NextByte();
	if ( (byte)'T' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected DateTime type char");
	}

	int year = (int)_NextByte() << 8 | (int)_NextByte();
	int month = (int)_NextByte();
	int day = (int)_NextByte();
	int hour = (int)_NextByte();
	int min = (int)_NextByte();
	int sec = (int)_NextByte();

	return DateTime(year, month, day, hour, min, sec);
}

Date Packet::ReadDate()
{
	byte datatype = _NextByte();
	if ( (byte)'D' != datatype )
	{
		throw new PacketReadTypeMismatchException("Expected Date type char");
	}

	int year = (int)_NextByte() << 8 | (int)_NextByte();
	int month = (int)_NextByte();
	int day = (int)_NextByte();

	return Date(year, month, day);
}

#if defined(DEBUG) || defined(_DEBUG)
void Packet::CheckMem() const
{
	m_buf.CheckMem();
	m_abuf.CheckMem();
}

void Packet::ValidateMem() const
{
	m_buf.ValidateMem();
	m_abuf.ValidateMem();
}
#endif

/*#################################################*/

IPacketListener::~IPacketListener()
{
}

PacketBuilder::PacketBuilder(/*IPacketListener *listener*/)
: m_listeners(), m_state(PKT_ENDIAN), m_buf(), m_readPos(0), m_pktsize(0)
{
}

PacketBuilder::~PacketBuilder()
{
}

void PacketBuilder::IStreamRead_OnError(const String& msg)
{
	m_state = PKT_ERROR;
	//m_listener->IStreamRead_OnError(msg);
	m_listeners.DispatchOnError(msg);
}

void PacketBuilder::IStreamRead_OnClose()
{
	m_state = PKT_CLOSED;
	//m_listener->IStreamRead_OnClose();
	m_listeners.DispatchOnClose();
}

void PacketBuilder::IStreamRead_OnRead(const Array<byte>& buf, int len)
{
	for (int bufpos = 0; bufpos < len; bufpos++ )
	{
		if (m_pktsize > 0 && m_readPos == m_pktsize)
		{
			m_state = PKT_ENDIAN;
		}
		else
		{
			ASSERT(0 == m_pktsize || m_readPos < m_pktsize);
			m_readPos++;
		}
		byte b = buf[bufpos];
		switch ( m_state )
		{
		case PKT_ENDIAN:
			m_readPos = 1;
			if ( b == 1 )
			{
				m_isLittleEndian = true;
			}
			else
			{
				m_isLittleEndian = false;
			}
#if LITTLE_ENDIAN
			m_revbytes = !m_isLittleEndian;
#else
			m_revbytes = m_isLittleEndian;
#endif
			m_state = PKT_SIZE_MCB;
			break;
		case PKT_SIZE_MCB:
#if LITTLE_ENDIAN
			m_pktsize = b << 8;
#else
			m_pktsize = b & 0xFF;
#endif
			m_state = PKT_SIZE_LCB;
			break;
		case PKT_SIZE_LCB:
#if LITTLE_ENDIAN
			m_pktsize |= b & 0xFF;
#else
			m_pktsize |= b << 8;
#endif
			ASSERT(m_pktsize > 0);
			m_state = PKT_DT;
			break;
		case PKT_DT:
			m_datatype = (char)b;
			switch ( m_datatype )
			{
			case 'I':
				m_datalen = 4;
				m_state = PKT_DATA;
				break;
			case 'X':
				m_datalen = 2;
				m_state = PKT_DATA;
				break;
			case 'B':
				m_datalen = 1;
				m_state = PKT_DATA;
				break;
			case 'S':
				m_state = PKT_DT_LEN_MCB;
				break;
			case 'R':
				m_state = PKT_DT_LEN_MCB;
				break;
			case 'F':
				m_datalen = 1;
				m_state = PKT_DATA;
				break;
			//case 'f':
			//	m_charsize = 1;
			//	m_state = PKT_DT_LEN_MCB;
			//	break;
			case 'd':
				m_charsize = 1;
				m_state = PKT_DT_LEN_MCB;
				break;
			case 'D':
				m_datalen = 4;
				m_state = PKT_DATA;
				break;
			case 't':
				m_datalen = 7;
				m_state = PKT_DATA;
				break;
			default:
				m_state = PKT_ERROR;
				throw new IOException("Invalid datatype recevied");
			}
			break;
		case PKT_DT_LEN_MCB:
#if LITTLE_ENDIAN
			m_datalen = b << 8;
#else
			m_datalen = b & 0xFF;
#endif
			m_state = PKT_DT_LEN_LCB;
			break;
		case PKT_DT_LEN_LCB:
#if LITTLE_ENDIAN
			m_datalen |= b & 0xFF;
#else
			m_datalen |= b << 8;
#endif
			if ( m_datatype == 'S' )
			{
				m_state = PKT_CHARSIZE;
			}
			else
			{
				m_state = PKT_DATA;
			}
			break;
		case PKT_CHARSIZE:
			m_charsize = b;
			m_datalen *= m_charsize;
			m_state = PKT_DATA;
			break;
		case PKT_DATA:
			m_buf.Add( b );
			if ( m_buf.Count() == m_datalen )
			{
				_ParseData();
				m_buf.Clear();
			}
			ASSERT( m_buf.Count() < m_datalen );
			break;
		case PKT_ERROR:
			break;
		}
	}

	if (m_readPos == m_pktsize)
	{
		m_state = PKT_ENDIAN;
	}
	ASSERT(m_readPos <= m_pktsize);
}

void PacketBuilder::_ParseData()
{
	int year;
	StringPtr str;
	switch ( m_datatype )
	{
		case 'I':
			if (m_revbytes)
			{
				//m_listener->IPacket_OnData((int32)(m_buf.ElementAt(0) | (m_buf.ElementAt(1) << 8) | (m_buf.ElementAt(2) << 16) | (m_buf.ElementAt(3) << 24)));
				m_listeners.DispatchOnData((int32)(m_buf.ElementAt(0) | (m_buf.ElementAt(1) << 8) | (m_buf.ElementAt(2) << 16) | (m_buf.ElementAt(3) << 24)));
			}
			else
			{
				//m_listener->IPacket_OnData((int32)(m_buf.ElementAt(3) | (m_buf.ElementAt(2) << 8) | (m_buf.ElementAt(1) << 16) | (m_buf.ElementAt(0) << 24)));
				m_listeners.DispatchOnData((int32)(m_buf.ElementAt(3) | (m_buf.ElementAt(2) << 8) | (m_buf.ElementAt(1) << 16) | (m_buf.ElementAt(0) << 24)));
			}
			break;
		case 'X':
			if (m_revbytes)
			{
				//m_listener->IPacket_OnData((int16)(m_buf.ElementAt(0) | (m_buf.ElementAt(1) << 8)));
				m_listeners.DispatchOnData((int16)(m_buf.ElementAt(0) | (m_buf.ElementAt(1) << 8)));
			}
			else
			{
				//m_listener->IPacket_OnData((int16)(m_buf.ElementAt(1) | (m_buf.ElementAt(0) << 8)));
				m_listeners.DispatchOnData((int16)(m_buf.ElementAt(1) | (m_buf.ElementAt(0) << 8)));
			}
			break;
		case 'B':
			//m_listener->IPacket_OnData( (byte)m_buf.ElementAt(0) );
			m_listeners.DispatchOnData( (byte)m_buf.ElementAt(0) );
			break;
		case 'S':
			str = _ParseString();
			//m_listener->IPacket_OnData(str);
			m_listeners.DispatchOnData(str);
			break;
		case 'R':
			//m_listener->IPacket_OnData(m_buf, m_datalen);
			m_listeners.DispatchOnData(m_buf, m_datalen);
			break;
		case 'F':
			//m_listener->IPacket_OnData( (bool)(0 != m_buf.ElementAt(0)) );
			m_listeners.DispatchOnData( (bool)(0 != m_buf.ElementAt(0)) );
			break;
		//case 'f':
		//	str = _ParseString();
		//	sscanf(str->GetChars(), "%lf", &d);
		//	m_listener->IPacket_OnData((float)d);
		//	break;
		case 'd':
			double d;
			str = _ParseString();
			sscanf(str->GetChars(), "%lf", &d);
			//m_listener->IPacket_OnData(d);
			m_listeners.DispatchOnData(d);
			break;
		case 't':
			if (m_revbytes)
			{
				year = (int)(m_buf.ElementAt(0) | (int)(m_buf.ElementAt(1) << 8));
			}
			else
			{
				year = (int)(m_buf.ElementAt(1) | (int)(m_buf.ElementAt(0) << 8));
			}
			//m_listener->IPacket_OnData
			//(
			//	DateTime
			//	(
			//		year, 
			//		(int)m_buf.ElementAt(2),
			//		(int)m_buf.ElementAt(3),
			//		(int)m_buf.ElementAt(4),
			//		(int)m_buf.ElementAt(5),
			//		(int)m_buf.ElementAt(6)
			//	)
			//);
			m_listeners.DispatchOnData
			(
				DateTime
				(
					year, 
					(int)m_buf.ElementAt(2),
					(int)m_buf.ElementAt(3),
					(int)m_buf.ElementAt(4),
					(int)m_buf.ElementAt(5),
					(int)m_buf.ElementAt(6)
				)
			);
			break;
		case 'D':
			if (m_revbytes)
			{
				year = (int)(m_buf.ElementAt(0) | (int)(m_buf.ElementAt(1) << 8));
			}
			else
			{
				year = (int)(m_buf.ElementAt(1) | (int)(m_buf.ElementAt(0) << 8));
			}
			//m_listener->IPacket_OnData
			//(
			//	Date
			//	(
			//		year, 
			//		(int)m_buf.ElementAt(2),
			//		(int)m_buf.ElementAt(3)
			//	)
			//);
			m_listeners.DispatchOnData
			(
				Date
				(
					year, 
					(int)m_buf.ElementAt(2),
					(int)m_buf.ElementAt(3)
				)
			);
			break;
		default:
			throw new Exception("Internal error");
	}
	m_state = PKT_DT;
}

StringPtr PacketBuilder::_ParseString()
{
	StringBuffer sbuf((m_datalen / m_charsize) + 1);
	if ( 1 == m_charsize )
	{
		for (int x = 0; x < m_datalen; x++ )
		{
			sbuf.Append( (char)m_buf.ElementAt(x) );
		}
	}
	else if ( 2 == m_charsize )
	{
		for (int x = 0; x < m_datalen; x += 2 )
		{
			if ( m_revbytes )
			{
				//sbuf.Append( (char)((m_buf.ElementAt(x)) | (m_buf.ElementAt(x+1) << 8)) );
				//Just take the ascii
				sbuf.Append( (char)m_buf.ElementAt(x) );
			}
			else
			{
				//sbuf.Append( (char)((m_buf.ElementAt(x+1)) | (m_buf.ElementAt(x) << 8)) );
				sbuf.Append( (char)(m_buf.ElementAt(x+1)) );
			}
		}
	}
	else
	{
		if (m_revbytes)
		{
			//sbuf.Append((char)(m_buf.ElementAt(0) | (m_buf.ElementAt(1) << 8) | (m_buf.ElementAt(2) << 16) | (m_buf.ElementAt(3) << 24)));
			sbuf.Append((char)m_buf.ElementAt(0));
		}
		else
		{
			sbuf.Append((char)m_buf.ElementAt(3));
		}
	}
	sbuf.Append('\0');
	return sbuf.ToString();
}

#if defined(DEBUG) || defined(_DEBUG)
void PacketBuilder::CheckMem() const
{
	m_buf.CheckMem();
	m_listeners.CheckMem();
}

void PacketBuilder::ValidateMem() const
{
	m_buf.ValidateMem();
	m_listeners.ValidateMem();
}
#endif

ThreadedPacketStream::ThreadedPacketStream(IPacketListener *listener, spl::IStreamPtr conn)
: m_builder(), m_conn(conn)
{
	m_builder.Delegates().Add(listener);
	m_conn.Delegates().Add(&m_builder);
}

ThreadedPacketStream::~ThreadedPacketStream()
{
}

#if defined(DEBUG) || defined(_DEBUG)
void ThreadedPacketStream::CheckMem() const
{
	m_conn.CheckMem();
	m_builder.CheckMem();
}

void ThreadedPacketStream::ValidateMem() const
{
	m_conn.ValidateMem();
	m_builder.ValidateMem();
}
#endif
