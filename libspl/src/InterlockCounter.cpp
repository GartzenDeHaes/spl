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
#include <spl/Environment.h>
#include <spl/threading/InterlockCounter.h>
#include <spl/threading/Thread.h>

using namespace spl;

#ifdef _WINDOWS

#include <winsock2.h>

static inline int32 atomic_increment(int32 *ptr)
{
	return InterlockedIncrement((LONG *)ptr);
}

static inline int32 atomic_decrement(int32 *ptr)
{
	return InterlockedDecrement((LONG *)ptr);
}

static inline int32 atomic_set(int32 *ptr, int32 val)
{
	*ptr = val;
	return val;
}

static inline int32 atomic_add(int32 *ptr, int32 val)
{
	return InterlockedExchangeAdd((LONG *)ptr, val);
}

static inline int32 atomic_cas(int32 *ptr, int32 val, int32 cmp)
{
#if _MSC_VER > 1200
	return InterlockedCompareExchange((LONG *)ptr, val, cmp);
#else
	return *(int32 *)InterlockedCompareExchange((void **)&ptr, &val, &cmp);
#endif
}

static inline int32 atomic_swap(int32 *ptr, int32 val)
{
	return InterlockedExchange((LONG *)ptr, val);
}

#elif defined(HAVE_ATOMIC_BUILTINS)

static inline int32 atomic_increment(int32 *ptr)
{
	return __sync_add_and_fetch(ptr, 1);
}

static inline int32 atomic_decrement(int32 *ptr)
{
	return __sync_sub_and_fetch(ptr, 1);
}

static inline int32 atomic_set(int32 *ptr, int32 val)
{
	__sync_lock_test_and_set(ptr, val);
	return val;
}

static inline int32 atomic_add(int32 *ptr, int32 val)
{
	return __sync_add_and_fetch(ptr, val);
}

static inline int32 atomic_cas(int32 *ptr, int32 val, int32 cmp)
{
	return __sync_val_compare_and_swap(ptr, val, cmp);
}

static inline int32 atomic_swap(int32 *ptr, int32 val)
{
	__sync_lock_test_and_set(ptr, val);
	return val;
}

#else

int32 InterlockCounter::atomic_increment(int32 *ptr)
{
	int32 r;

	if (0 != pthread_mutex_lock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	r = ++(*ptr);

	if (0 != pthread_mutex_unlock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	return r;
}

int32 InterlockCounter::atomic_decrement(int32 *ptr)
{
	int32 r;

	if (0 != pthread_mutex_lock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	r = --(*ptr);

	if (0 != pthread_mutex_unlock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	return r;
}

int32 InterlockCounter::atomic_set(int32 *ptr, int32 val)
{
	if ( 0 != pthread_mutex_lock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	(*ptr) = val;

	if (0 != pthread_mutex_unlock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	return val;
}

int32 InterlockCounter::atomic_add(int32 *ptr, int32 val)
{
	int32 r;

	if (0 != pthread_mutex_lock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	r = ((*ptr) += val);

	if (0 != pthread_mutex_unlock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	return r;
}

int32 InterlockCounter::atomic_get(const int32 *ptr)
{
	int32 r;

	if (0 != pthread_mutex_lock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	r = *ptr;

	if (0 != pthread_mutex_unlock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	return r;
}

int32 InterlockCounter::atomic_cas(int32 *ptr, int32 val, int32 cmp)
{
	int32 r;

	if (0 != pthread_mutex_lock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	r = *ptr;

	if((*ptr) == cmp)
	*ptr = val;

	if (0 != pthread_mutex_unlock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	return r;
}

int32 InterlockCounter::atomic_swap(int32 *ptr, int32 val)
{
	int32 r;

	if (0 != pthread_mutex_lock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	r = *ptr;
	*ptr = val;

	if (0 != pthread_mutex_unlock(&__atomic_mutex))
	{
		throw new IOException(Environment::LastErrorMessage());
	}

	return r;
}

#endif

InterlockCounter::InterlockCounter(int32 value /* = 0 */) throw()
{
#if !defined(_WINDOWS) && !defined(HAVE_ATOMIC_BUILTINS)
#ifdef _TANDEM
	__atomic_mutex.field1 = (void *)-1;
	__atomic_mutex.field2 = -1;
	__atomic_mutex.field3 = -1;
#else
	__atomic_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
#endif
	operator=(value);
}

/*
 */

InterlockCounter::~InterlockCounter() throw()
{
#if !defined(_WINDOWS) && !defined(HAVE_ATOMIC_BUILTINS)
	pthread_mutex_destroy(&__atomic_mutex);
#endif
}

/*
 */

int32 InterlockCounter::operator++() throw()
{
	// prefix
	return atomic_increment(&_atomic);
}

/*
 */

int32 InterlockCounter::operator++(int) throw()
{
	// postfix
	int32 r = atomic_increment(&_atomic);
	return --r;
}

/*
 */

int32 InterlockCounter::operator--() throw()
{
	// prefix
	return atomic_decrement(&_atomic);
}

/*
 */

int32 InterlockCounter::operator--(int) throw()
{
	// postfix
	int32 r = atomic_decrement(&_atomic);
	return ++r;
}

/*
 */

int32 InterlockCounter::operator+=(int32 delta) throw()
{
	return atomic_add(&_atomic, delta);
}

/*
 */

int32 InterlockCounter::operator-=(int32 delta) throw()
{
	return atomic_add(&_atomic, -delta);
}

/*
 */

int32 InterlockCounter::operator=(int32 value) throw()
{
	atomic_set(&_atomic, value);

	return value;
}

/*
 */

int32 InterlockCounter::Set(int32 value) throw()
{
	return atomic_set(&_atomic, value);
}

/*
 */

int32 InterlockCounter::Get() throw()
{
#if defined(_WINDOWS) || defined(HAVE_ATOMIC_BUILTINS)
	return(atomic_add(&_atomic, 0));
#else
	return atomic_get(&_atomic);
#endif
}

/*
 */

int32 InterlockCounter::Swap(int32 value) throw()
{
	return atomic_swap(&_atomic, value);
}

/*
 */

int32 InterlockCounter::TestAndSet(int32 value, int32 comparand) throw()
{
	return atomic_cas(&_atomic, value, comparand);
}

/*
 */

InterlockCounter::InterlockCounter(InterlockCounter& other) throw()
{
	Set(other.Get());
}

/*
 */

InterlockCounter& InterlockCounter::operator =(InterlockCounter& other) throw()
{
	Set(other.Get());

	return *this;
}
