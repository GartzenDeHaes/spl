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
#ifndef _rwlock_h
#define _rwlock_h

#include <spl/threading/Event.h>

namespace spl
{
/**
 * @defgroup sync Synchronization Primitives
 * @ingroup threading
 * @{
 */

class RWLock;
typedef RefCountPtr<RWLock> RWLockPtr;
typedef WeakReference<RWLock, RWLockPtr> RWLockRef;

REGISTER_TYPEOF( 466, RWLockPtr );
REGISTER_TYPEOF( 468, RWLockRef );

///@brief A multiple reader single writer lock.  Readers cannot upgrade their lock to
/// writer.  LockRead/UnlockRead and LockWrite/UnlockWrite must alway be called in pairs
/// and cannot be interleved.
class RWLock
{
private:
	// forbid copy constructor
	inline RWLock(const RWLock& rw) {}
	inline void operator =(const RWLock& rw) {}

protected:

#if defined(_WINDOWS) || defined(__TANDEM)

	Mutex m_stateMtx;
	int m_readLockCount;
	int m_writeLockCount;

	Event m_waitingWriters;
	int m_waitingWritersCount;

	Event m_waitingReaders;
	int m_waitingReadersCount;

#else
	pthread_rwlock_t m_rwlock;
#endif

public:
	RWLock();
	virtual ~RWLock();

	void LockRead();
	void LockWrite();
	void UnlockRead();
	void UnlockWrite();
};

inline void TypeValidate(const RWLock& rw)
{
}

inline void TypeCheckMem(const RWLock& rw)
{
}

REGISTER_TYPEOF( 470, RWLock );

/** @} */
}
#endif
