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
#ifndef _delegate_h
#define _delegate_h

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

class IDelegate : public IMemoryValidate
{
public:
	virtual void Call() = 0;
	virtual void Clear() = 0;

#if defined(DEBUG)
	virtual void CheckMem() const {}
	virtual void ValidateMem() const {}
#endif
};

/** @brief C# style delegate with no arguments for call backs by DelegateDispatch.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref DelegateDispatch
 */
template<class T>
class Delegate : public IDelegate
{
private:
	T* m_object;
	void (T::*m_member)();

public:
	inline Delegate(T *object, void (T::* func)())
	: m_object(object), m_member(func)
	{
	}

	virtual void Call()
	{
		(m_object->*m_member)();
	}

	static RefCountPtr<IDelegate> Create(T *object, void (T::* func)())
	{
		return RefCountPtr<IDelegate>(new Delegate<T>(object, func));
	}

	virtual void Clear()
	{
		m_object = NULL;
	}
};

template<typename ARG>
class IDelegateOneParameter
{
public:
	virtual void Call(ARG arg) = 0;

#if defined(DEBUG)
	virtual void CheckMem() const {}
	virtual void ValidateMem() const {}
#endif
};

/** @brief C# style delegate with one argument for call backs by DelegateDispatch.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref DelegateDispatch
 */
template<class T, typename ARG>
class DelegateOneParameter : public IDelegateOneParameter<ARG>
{
private:
	T* m_object;
	void (T::*m_member)(ARG arg);

public:
	inline DelegateOneParameter(T *object, void (T::* func)(ARG arg))
	: m_object(object), m_member(func)
	{
	}

	virtual void Call(ARG arg)
	{
		(m_object->*m_member)(arg);
	}

	static RefCountPtr<IDelegateOneParameter<ARG> > Create(T *object, void (T::* func)(ARG arg))
	{
		return RefCountPtr<IDelegateOneParameter<ARG> >(new DelegateOneParameter<T, ARG>(object, func));
	}
};

template<typename ARG1, typename ARG2>
class IDelegateTwoParameter
{
public:
	virtual void Call(ARG1 arg1, ARG2 arg2) = 0;

#if defined(DEBUG)
	virtual void CheckMem() const {}
	virtual void ValidateMem() const {}
#endif
};

/** @brief C# style delegate with two arguments for call backs by DelegateDispatch.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref DelegateDispatch
 */
template<class T, typename ARG1, typename ARG2>
class DelegateTwoParameter : public IDelegateTwoParameter<ARG1, ARG2>
{
private:
	T* m_object;
	void (T::*m_member)(ARG1 arg1, ARG2 arg2);

public:
	inline DelegateTwoParameter(T *object, void (T::* func)(ARG1 arg1, ARG2 arg2))
	: m_object(object), m_member(func)
	{
	}

	inline void Call(ARG1 arg1, ARG2 arg2)
	{
		(m_object->*m_member)(arg1, arg2);
	}

	static RefCountPtr<IDelegateTwoParameter<ARG1, ARG2> > Create(T *object, void (T::* func)(ARG1 arg1, ARG2 arg2))
	{
		return RefCountPtr<IDelegateTwoParameter<ARG1, ARG2> >(new DelegateTwoParameter<T, ARG1, ARG2>(object, func));
	}
};

template<typename ARG1, typename ARG2, typename ARG3>
class IDelegateThreeParameter
{
public:
	virtual void Call(ARG1 arg1, ARG2 arg2, ARG3 arg3) = 0;

#if defined(DEBUG)
	virtual void CheckMem() const {}
	virtual void ValidateMem() const {}
#endif
};

/** @brief C# style delegate with three arguments for call backs by DelegateDispatch.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref DelegateDispatch
 */
template<class T, typename ARG1, typename ARG2, typename ARG3>
class DelegateThreeParameter : public IDelegateThreeParameter<ARG1, ARG2, ARG3>
{
private:
	T* m_object;
	void (T::*m_member)(ARG1 arg1, ARG2 arg2, ARG3 arg3);

public:
	inline DelegateThreeParameter(T *object, void (T::* func)(ARG1 arg1, ARG2 arg2, ARG3 arg3))
	: m_object(object), m_member(func)
	{
	}

	inline void Call(ARG1 arg1, ARG2 arg2, ARG3 arg3)
	{
		(m_object->*m_member)(arg1, arg2, arg3);
	}

	static RefCountPtr<IDelegateThreeParameter<ARG1, ARG2, ARG3> > Create(T *object, void (T::* func)(ARG1 arg1, ARG2 arg2, ARG3 arg3))
	{
		return RefCountPtr<IDelegateThreeParameter<ARG1, ARG2, ARG3> >(new DelegateThreeParameter<T, ARG1, ARG2, ARG3>(object, func));
	}
};

/** @} */
}
#endif
