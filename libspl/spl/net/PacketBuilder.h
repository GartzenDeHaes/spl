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
#ifndef _packetbuilder_h
#define _packetbuilder_h

#include <spl/types.h>
#include <spl/net/Packet.h>

namespace spl
{
/** 
 * @defgroup packet Packets
 * @ingroup network
 * @{
 */

class PacketBuilder;
typedef RefCountPtrCast<PacketBuilder, IStreamReadListener, IStreamReadListenerPtr> PacketBuilderPtr;

REGISTER_TYPEOF(30, PacketBuilderPtr);

/** @brief Allows automatic construction of packets from a stream.
 */
class PacketBuilder : public IStreamReadListener
{
protected:
	enum PacketReadState
	{
		PKT_ENDIAN,
		PKT_SIZE_MCB,
		PKT_SIZE_LCB,
		PKT_DT,
		PKT_DT_LEN_MCB,
		PKT_DT_LEN_LCB,
		PKT_CHARSIZE,
		PKT_DATA,
		PKT_ERROR,
		PKT_CLOSED
	};

	PacketListenerDelegateDispatch  m_listeners;
	enum PacketReadState m_state;

	bool m_isLittleEndian;
	bool m_revbytes;
	int16 m_pktsize;
	char m_datatype;
	int16 m_datalen;
	int8 m_charsize;
	Vector<byte> m_buf;
	int m_readPos;

	void _ParseData();
	StringPtr _ParseString();

	virtual void IStreamRead_OnRead(const Array<byte>& buf, int len);
	virtual void IStreamRead_OnError(const String& msg);
	virtual void IStreamRead_OnClose();

public:
	PacketBuilder();
	virtual ~PacketBuilder();

	inline PacketListenerDelegateDispatch& Delegates() { return m_listeners; }

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

REGISTER_TYPEOF(32, PacketBuilder);

/** @} */
}
#endif
