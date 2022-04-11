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
#ifndef _dualchannelstream_h
#define _dualchannelstream_h

#include <spl/io/Stream.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

class DualChannelStream;
typedef RefCountPtrCast<DualChannelStream, spl::IStream, spl::IStreamPtr> DualChannelStreamPtr;
typedef WeakReference<DualChannelStream, spl::IStreamPtr> DualChannelStreamRef;

REGISTER_TYPEOF(86, DualChannelStreamPtr);
REGISTER_TYPEOF( 451, DualChannelStreamRef );

/** @brief Uses two different streams for input and output.
 *
 */
class DualChannelStream : public spl::IStream
{
protected:
	spl::IStreamPtr m_in;
	spl::IStreamPtr m_out;

public:
	DualChannelStream(spl::IStreamPtr input, spl::IStreamPtr output, bool canClose = false);
	virtual ~DualChannelStream();

	virtual void Close();
	virtual void Flush();
	virtual int Read(Array<byte>& buffer, const int offset, int count);
	virtual int ReadByte();
	virtual long Seek(const long offset, const SeekOrigin origin);
	virtual void Write(const Array<byte>& buffer, const int offset, const int count);
	virtual void WriteByte(byte value);

	virtual bool CanRead() const;
	virtual bool CanSeek() const;
	virtual bool CanWrite() const;

	virtual long Length() const;
	virtual long Position() const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF(88, DualChannelStream);

/** @} */
}
#endif
