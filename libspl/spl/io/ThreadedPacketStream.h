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
#ifndef _threadedpacketstream_h
#define _threadedpacketstream_h

#include <spl/io/Stream.h>
#include <spl/net/Packet.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class ThreadedPacketStream;
typedef RefCountPtr<ThreadedPacketStream> ThreadedPacketStreamPtr;

/** @brief ThreadedPacketStream creates a thread to read from a stream and pumps the bytes to a PacketBuilder.
 *	@ref PacketBuilder
 *	@ref StreamReadPump
 */
class ThreadedPacketStream : public IMemoryValidate
{
protected:
	PacketBuilder m_builder;
	StreamReadPump m_conn;

public:
	ThreadedPacketStream(IPacketListener *listener, spl::IStreamPtr conn);
	virtual ~ThreadedPacketStream();

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
}
#endif
