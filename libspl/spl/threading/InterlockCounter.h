/* Ported to SPL, orignal attribution below. */
/* ---------------------------------------------------------------------------
   commonc++ - A C++ Common Class Library
   Copyright (C) 2005-2009  Mark A Lindner

   Ported from commonc++.

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
#ifndef _interlockcount_h
#define _interlockcount_h

#include <spl/types.h>
#include <spl/Debug.h>

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef HAVE_SPTHREAD_H
#include <sys/types.h>
#include <spt_types.h>
#include <sys/time.h>
#include <wchar.h>
#include <signal.h>
#include <spt_extensions.h>
#include <spthread.h>
/* also need LDFLAGS="-spthread" */
#endif

namespace spl
{
/**
 * @defgroup sync Synchronization Primitives
 * @ingroup threading
 * @{
 */

/** @brief An integer counter whose value is modified in an atomic fashion.
*
* @author Mark Lindner
*/
class InterlockCounter
{
private:
	int32 _atomic;

#if !defined(_WINDOWS) && !defined(HAVE_ATOMIC_BUILTINS)
	pthread_mutex_t __atomic_mutex;

	int32 atomic_increment(int32 *ptr);
	int32 atomic_decrement(int32 *ptr);
	int32 atomic_set(int32 *ptr, int32 val);
	int32 atomic_add(int32 *ptr, int32 val);
	int32 atomic_get(const int32 *ptr);
	int32 atomic_cas(int32 *ptr, int32 val, int32 cmp);
	int32 atomic_swap(int32 *ptr, int32 val);

#endif

public:

	/** Construct a new <b>AtomicCounter</b> with the given initial value.
	 *
	 * @param value The initial value.
	 */
	InterlockCounter(int32 value = 0) throw();

	/** Destructor. */
	~InterlockCounter() throw();

	/** Copy constructor. */
	InterlockCounter(InterlockCounter& other) throw();

	/** Assignment operator. */
	InterlockCounter& operator =(InterlockCounter& other) throw();

	/** Increment the counter (prefix). */
	int32 operator++() throw();

	/** Increment the counter (postfix). */
	int32 operator++(int) throw();

	/** Decrement the counter (prefix). */
	int32 operator--() throw();

	/** Decrement the counter (postfix). */
	int32 operator--(int) throw();

	/** Add a value to the counter. */
	int32 operator+=(int32 delta) throw();

	/** Subtract a value from the counter. */
	int32 operator-=(int32 delta) throw();

	/** Compute the sum of the counter and a value. */
	int32 operator+(int32 delta) const throw();

	/** Compute the difference between the counter and a value. */
	int32 operator-(int32 delta) const throw();

	/** Assign a new value to the counter. */
	int32 operator=(int32 value) throw();

	/** Assign a new value to the counter, returning the new value. */
	int32 Set(int32 value) throw();

	/** Assign a new value to the counter, returning the previous value. */
	int32 Swap(int32 value) throw();

	/** Test and set the counter value.
	 *
	 * @param comparand The value to compare the counter to.
	 * @param value The value to set the counter to, if the current value is
	 * equal to the comparand.
	 * @return The original (and possibly unchanged) value of the counter.
	 */
	int32 TestAndSet(int32 value, int32 comparand) throw();

	/** Get the current value of the counter. */
	int32 Get() throw();

	/** Cast operator. */
	//inline operator int32() throw()
	//{
	//	return Get();
	//}
};

inline void TypeValidate(const InterlockCounter& i)
{
}

inline void TypeCheckMem(const InterlockCounter& i)
{
}

/** @} */
}
#endif
