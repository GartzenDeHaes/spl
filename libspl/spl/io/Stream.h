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
#ifndef _stream2_h
#define _stream2_h

#include <stdio.h>

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/threading/Event.h>
#include <spl/threading/Mutex.h>
#include <spl/io/IStream.h>
#include <spl/Memory.h>
#include <spl/RefCountPtrCast.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>
#include <spl/WeakReference.h>

/** 
 * @defgroup io IO
 * @{
 */

 /** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

namespace spl
{
/** @brief State pattern for streams (internal).
  *
  */
class IStreamState : public IStream
{
protected:
	IStream *m_parent;
	IStreamState **m_holder;

	void ChangeState(IStreamState *newState);
public:
	IStreamState( IStream *parent, IStreamState **parentStateVar );
	virtual ~IStreamState();

	virtual void OnLeave(IStreamState *newState) = 0;
	virtual void OnEnter(IStreamState *oldState) = 0;
};

/** @} */
/** @} */
}
#endif
