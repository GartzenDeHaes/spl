/* Ported to SPL from commonc++, original attribution below. */
/* ---------------------------------------------------------------------------
   commonc++ - A C++ Common Class Library
   Copyright (C) 2005-2009  Mark A Lindner

   This file is part of commonc++.

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
#ifndef _threadstartdelegate_h
#define _threadstartdelegate_h

#include <spl/Memory.h>
#include <spl/threading/Thread.h>

namespace spl
{
/** @brief An adapter object that implements the Runnable interface and
* delegates to an arbitrary method (one that must have no arguments or
* return value) in some other object. This allows a class to
* have multiple methods that will execute asynchronously in their own
* threads. For example, consider the class:
* <pre>
* class MyClass
* {
*   public:
*
*   MyClass();
*   ~MyClass();
*
*   void Thread1();
*   void Thread2();
* };
* </pre>
* Given an instance of MyClass, the following construct causes a new
* thread to begin executing in the <b>Thread1()</b> method of that
* instance.
*
* <pre>
* MyClass mc;
*
* ThreadStartDelegate&lt;MyClass&gt; thread1(&mc, &MyClass::Thread1);
* thread1.Start();
* </pre>
*/
template<class T> class ThreadStartDelegate : public Thread
{
private:
	T* m_object;
	void (T::*m_func)();

public:

	/** Construct a new ThreadStartDelegate which will call a method on another
	 * object.
	 *
	 * @param object The object.
	 * @param func A member function (method) in the object which will be
	 * called by <b>run()</b>.
	 */
	inline ThreadStartDelegate(T *object, void (T::* func)())
	: m_object(object), m_func(func)
	{
	}

	inline ThreadStartDelegate()
	: m_object(NULL), m_func(NULL)
	{
	}

	virtual ~ThreadStartDelegate()
	{
	}

	inline void Set(T *object, void (T::* func)())
	{
		m_object = object;
		m_func = func;
	}

private:
	virtual void Run()
	{
		(m_object->*m_func)();
	}
};
}
#endif
