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
#ifndef _packet_h
#define _packet_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/collection/Array.h>
#include <spl/Date.h>
#include <spl/DateTime.h>
#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/net/Socket.h>
#include <spl/io/Stream.h>
#include <spl/io/StreamReadPump.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup packet Packets
 * @ingroup network
 * @{
 */

class Packet;
typedef RefCountPtr<Packet> PacketPtr;

REGISTER_TYPEOF(20, PacketPtr);

/** @brief Endian aware network packet interface.
  *
  */
class Packet : public IMemoryValidate
{
protected:
	Array<byte> m_abuf;
	Vector<byte> m_buf;
	bool m_readPacketReady;
	int m_readpos;
	bool m_readRevByteOrder;
	int16 m_rpacketsize;

	inline byte _NextByte() { if (m_readpos >= m_buf.Count()) throw new PacketUnderflowException(); return m_buf.ElementAt(m_readpos++); }

	void _AppendRaw( byte i );
	void _AppendRaw( int16 i );
	void _AppendRaw( int32 i );
	void _AppendRaw( int64 i );

public:
	Packet();
	Packet(const Packet& pkt);
	virtual ~Packet();

	Packet& operator =(const Packet& pkt);

	void Clear();
	void SendPacket(spl::IStream& sock);
	void ReadPacket(spl::IStream& sock);

	void Append(int64 i);
	void Append(int32 i);
	void Append(int16 i);
	void Append(byte i);
	void Append(bool b);
	//void Append(float f);
	void Append(double d);
	void Append(const DateTime& dtm);
	void Append(const Date& dt);
	inline void Append(const String& str) { Append(str.GetChars(), str.Length()); }
	inline void Append(const char *str) { Append(str, (int)strlen(str)); }
	void Append(const char *str, int len);
	void Append(const Array<byte>& buf, int start, int len);
	
	inline void Append(const Array<byte>& buf, int len)
	{
		Append(buf, 0, len);
	}

	inline void Append(const Array<byte>& buf)
	{
		Append(buf, 0, buf.Count());
	}

	int PacketSize();
	int64 ReadInt64();
	int32 ReadInt32();
	int16 ReadInt16();
	byte ReadInt8();
	bool ReadBool();
	//float ReadFloat();
	double ReadDouble();
	StringPtr ReadString();
	byte *ReadBuf(int len);
	DateTime ReadDateTime();
	Date ReadDate();

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

REGISTER_TYPEOF(22, Packet);

class IPacketListener;
typedef RefCountPtr<IPacketListener> IPacketListenerPtr;

REGISTER_TYPEOF(24, IPacketListenerPtr);

class IPacketListener
{
public:
	IPacketListener() {}
	virtual ~IPacketListener();

	virtual void IPacket_OnData(int64 i) = 0;
	virtual void IPacket_OnData(int32 i) = 0;
	virtual void IPacket_OnData(int16 i) = 0;
	virtual void IPacket_OnData(byte i) = 0;
	virtual void IPacket_OnData(bool b) = 0;
	virtual void IPacket_OnData(double d) = 0;
	virtual void IPacket_OnData(StringPtr str) = 0;
	virtual void IPacket_OnData(const Vector<byte>& data, const int len) = 0;
	virtual void IPacket_OnData(const DateTime& dtm) = 0;
	virtual void IPacket_OnData(const Date& i) = 0;

	virtual void IStreamRead_OnError(const String& msg) = 0;
	virtual void IStreamRead_OnClose() = 0;
};

REGISTER_TYPEOF(26, IPacketListener);

class PacketListenerDelegateDispatch : public IMemoryValidate
{
private:
	DelegateDispatchOneParameter<int64> m_onDataInt64;
	DelegateDispatchOneParameter<int32> m_onDataInt32;
	DelegateDispatchOneParameter<int16> m_onDataInt16;
	DelegateDispatchOneParameter<byte> m_onDataByte;
	DelegateDispatchOneParameter<bool> m_onDataBool;
	DelegateDispatchOneParameter<double> m_onDataDouble;
	DelegateDispatchOneParameter<StringPtr> m_onDataString;
	DelegateDispatchTwoParameter<const Vector<byte>&, const int> m_onDataBin;
	DelegateDispatchOneParameter<const DateTime&> m_onDataDtm;
	DelegateDispatchOneParameter<const Date&> m_onDataDate;
	DelegateDispatchOneParameter<const String&> m_onError;
	DelegateDispatch m_onClose;

public:
	inline PacketListenerDelegateDispatch()
	: m_onDataInt32()
	{
	}

	virtual ~PacketListenerDelegateDispatch();

	inline DelegateDispatchOneParameter<int64>& OnDataInt64() { return m_onDataInt64; }
	inline DelegateDispatchOneParameter<int32>& OnDataInt32() { return m_onDataInt32; }
	inline DelegateDispatchOneParameter<int16>& OnDataInt16() { return m_onDataInt16; }
	inline DelegateDispatchOneParameter<byte>& OnDataByte() { return m_onDataByte; }
	inline DelegateDispatchOneParameter<bool>& OnDataBool() { return m_onDataBool; }
	inline DelegateDispatchOneParameter<double>& OnDataDouble() { return m_onDataDouble; }
	inline DelegateDispatchOneParameter<StringPtr>& OnDataString() { return m_onDataString; }
	inline DelegateDispatchTwoParameter<const Vector<byte>&, const int>& OnDataBinary() { return m_onDataBin; }
	inline DelegateDispatchOneParameter<const DateTime&>& OnDataDateTime() { return m_onDataDtm; }
	inline DelegateDispatchOneParameter<const Date&>& OnDataDate() { return m_onDataDate; }
	inline DelegateDispatchOneParameter<const String&>& OnError() { return m_onError; }
	inline DelegateDispatch& OnClose() { return m_onClose; }

	inline void DispatchOnData(int64 i) { m_onDataInt64.Call(i); }
	inline void DispatchOnData(int32 i) { m_onDataInt32.Call(i); }
	inline void DispatchOnData(int16 i) { m_onDataInt16.Call(i); }
	inline void DispatchOnData(byte i) { m_onDataByte.Call(i); }
	inline void DispatchOnData(bool i) { m_onDataBool.Call(i); }
	inline void DispatchOnData(double i) { m_onDataDouble.Call(i); }
	inline void DispatchOnData(StringPtr i) { m_onDataString.Call(i); }
	inline void DispatchOnData(const Vector<byte>& v, const int len) { m_onDataBin.Call(v, len); }
	inline void DispatchOnData(const DateTime& dtm) { m_onDataDtm.Call(dtm); }
	inline void DispatchOnData(const Date& dt) { m_onDataDate.Call(dt); }
	inline void DispatchOnError(const String& msg) { m_onError.Call(msg); }
	inline void DispatchOnClose() { m_onClose.Call(); }

	void Add(IPacketListener *l);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF(28, PacketListenerDelegateDispatch);

/** @} */
}
#endif
