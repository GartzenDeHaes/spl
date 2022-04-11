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
#ifndef _streamdelegatedispatch_h
#define _streamdelegatedispatch_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/DelegateDispatch.h>
#include <spl/io/IStream.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/RefCountPtrCast.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

/** @brief Stream event delegate dispatcher.
 *	@ref DelegateDispatch
 */
class StreamDelegateDispatch : public IMemoryValidate
{
private:
	DelegateDispatch m_onClose;
	DelegateDispatchOneParameter<const String&> m_onError;
	DelegateDispatchTwoParameter<const Array<byte>&, int> m_onRead;

public:
	inline StreamDelegateDispatch()
	: m_onClose(), m_onError(), m_onRead()
	{
	}

	virtual ~StreamDelegateDispatch();

	inline DelegateDispatch& OnClose() { return m_onClose; }
	inline DelegateDispatchOneParameter<const String&>& OnError() { return m_onError; }
	inline DelegateDispatchTwoParameter<const Array<byte>&, int>& OnRead() { return m_onRead; }

	inline void DispatchOnClose() { m_onClose.Call(); }
	inline void DispatchOnError(const String& msg) { m_onError.Call(msg); }
	inline void DispatchOnRead(const Array<byte>&a, int len) { m_onRead.Call(a, len); }

	/** If you would like events through an interface instead, use this method and the delegates will be created for you */
	void Add(IStreamReadListener* l);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
