/* Original source attribution */
/* ---------------------------------------------------------------------------
   commonc++ - A C++ Common Class Library
   Copyright (C) 2005-2009  Mark A Lindner

   Ported to SPL from commonc++.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   ---------------------------------------------------------------------------
*/

#ifndef _semaphore_h
#define _semaphore_h

#ifndef _TANDEM

#include <spl/types.h>
#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/threading/Mutex.h>
#include <spl/io/Permissions.h>
#include <spl/String.h>
#include <spl/RefCountPtr.h>

#ifndef _WINDOWS
#include <fcntl.h>
#include <semaphore.h>
#endif

namespace spl
{
/** 
 * @defgroup sync Synchronization Primitives
 * @ingroup threading
 * @{
 */

class Semaphore;
typedef RefCountPtr<Semaphore> SemaphorePtr;
typedef WeakReference<Semaphore, SemaphorePtr> SemaphoreRef;

REGISTER_TYPEOF( 472, SemaphorePtr );
REGISTER_TYPEOF( 475, SemaphoreRef );

/**@brief  A counting semaphore -- a synchronization primitive that allows
* multiple processes to coordinate access to a shared resource. A
* Semaphore has an initial value, which represents the quantity of
* some shared resource.  When a process acquires the semaphore,
* this value is decremented, and when it releases the semaphore,
* the value is incremented.
*
* @author Mark Lindner
*/
class Semaphore : public IMemoryValidate
{
private:

#ifdef _WINDOWS
	HANDLE _sem;
#else
	sem_t _sem;
#endif

public:

	/** Construct a new Semaphore with the given name, initial value,
	 * and permissions. If the underlying semaphore object did not yet
	 * exist, it is created.
	 *
	 * @param name The name of the semaphore. On POSIX systems, the name
	 * must consist of at most 14 alphanumeric characters and may not
	 * contain slashes.
	 * @param value The initial value of the semaphore. Must be at least 1.
	 * @param perm The permissions with which to create the semaphore, if it
	 * does not yet exist.
	 */
	Semaphore
	(
		uint32 value = 1,
		const Permissions& perm = Permissions::USER_READ_WRITE
	);

	/** Destructor. Destroys the underlying semaphore object, if no other
	 * processes have a reference to it. */
	~Semaphore();

	/** Wait on the semaphore. If the semaphore's value is greater than 0,
	 * it's value is decremented and the method returns. Otherwise, the method
	 * blocks until the value becomes greater than 0.
	 *
	 * @return <b>true</b> if the semaphore was acquired, <b>false</b> if an
	 * error occurred.
	 */
	void Lock();

	/** Try to wait on the semaphore, returning immediately if it couldn't be
	 * acquired.
	 *
	 * @return <b>true</b> if the semaphore was acquired, <b>false</b>
	 * otherwise.
	 */
	bool TryWait();

	/** Signal the semaphore. Increments the semaphore's value by 1. The
	 * semaphore must have previously been acquired.
	 *
	 * @return <b>true</b> if the semaphore was released, <b>false</b>
	 * otherwise.
	 */
	void Unlock();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 476, Semaphore );

/** @} */
}
#endif

#endif
