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
#include <spl/io/Stream.h>

#ifdef _stream2_h
/* _stream2_h is in Stream.h, only compile this file if using the new Stream.h header */

#ifndef _streamreadpump_h
#define _streamreadpump_h

#include <spl/collection/Array.h>
#include <spl/DelegateDispatch.h>
#include <spl/io/StreamDelegateDispatch.h>
#include <spl/threading/Thread.h>

namespace spl
{
/**
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

/** @brief Creates a thread and pushes stream data reads through a StreamDelegateDispatch.
 *	@ref StreamDelegateDispatch
 *	@ref
 */
class StreamReadPump : public Thread
{
private:
	// forbid copy constructor
	inline StreamReadPump(const StreamReadPump& pmp) : Thread() {}
	inline void operator =(const StreamReadPump& pmp) {}

protected:
	spl::IStreamPtr m_strm;
	bool m_running;
	Array<byte> m_buf;
	int m_bufsize;

	StreamDelegateDispatch m_eventDispatch;

public:
	StreamReadPump(spl::IStreamPtr strm, int bufsize = 512);
	~StreamReadPump();

	inline spl::IStreamPtr GetStream() { return m_strm; }

	void Run();
	void Stop();

	inline StreamDelegateDispatch& Delegates() { return m_eventDispatch; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
#endif
