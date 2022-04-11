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
#ifndef _delegatedispatch_h
#define _delegatedispatch_h

#include <spl/Delegate.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

/** @brief Dispatcher for C# style delegates with no arguments.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref Delegate
 */
class DelegateDispatch : public IMemoryValidate
{
private:
	Vector<RefCountPtr<IDelegate> > m_delegates;

public:
	DelegateDispatch();
	virtual ~DelegateDispatch();

	inline void Add(RefCountPtr<IDelegate> d)
	{
		m_delegates.Add(d);
	}

	inline void operator +=(RefCountPtr<IDelegate> d)
	{
		Add(d);
	}

	void Call();

	void Clear();

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @brief Dispatcher for C# style delegates with one argument.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref DelegateOneParameter
 */
template<typename ARG1>
class DelegateDispatchOneParameter : public IMemoryValidate
{
private:
	Vector<RefCountPtr<IDelegateOneParameter<ARG1> > > m_delegates;

public:
	DelegateDispatchOneParameter()
	: m_delegates()
	{
	}

	virtual ~DelegateDispatchOneParameter()
	{
	}

	inline void Add(RefCountPtr<IDelegateOneParameter<ARG1> > d)
	{
		m_delegates.Add(d);
	}

	inline void operator +=(RefCountPtr<IDelegateOneParameter<ARG1> > d)
	{
		Add(d);
	}

	void Call(ARG1 arg1)
	{
		for (int x = 0; x < m_delegates.Count(); x++ )
		{
			m_delegates.ElementAtRef(x)->Call(arg1);
		}
	}

	void Clear()
	{
		for (int x = 0; x < m_delegates.Count(); x++ )
		{
			m_delegates.ElementAtRef(x)->Clear();
		}
		m_delegates.Clear();
	}

#if defined(DEBUG)
	void CheckMem() const
	{
		m_delegates.CheckMem();
	}

	void ValidateMem() const
	{
		m_delegates.ValidateMem();
	}
#endif
};

/** @brief Dispatcher for C# style delegates with two arguments.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref DelegateTwoParameter
 */
template<typename ARG1, typename ARG2>
class DelegateDispatchTwoParameter : public IMemoryValidate
{
private:
	Vector<RefCountPtr<IDelegateTwoParameter<ARG1, ARG2> > > m_delegates;

public:
	DelegateDispatchTwoParameter()
	: m_delegates()
	{
	}

	virtual ~DelegateDispatchTwoParameter()
	{
	}

	inline void Add(RefCountPtr<IDelegateTwoParameter<ARG1, ARG2> > d)
	{
		m_delegates.Add(d);
	}

	inline void operator +=(RefCountPtr<IDelegateTwoParameter<ARG1, ARG2> > d)
	{
		Add(d);
	}

	void Call(ARG1 arg1, ARG2 arg2)
	{
		for (int x = 0; x < m_delegates.Count(); x++ )
		{
			m_delegates.ElementAtRef(x)->Call(arg1, arg2);
		}
	}

	void Clear()
	{
		for (int x = 0; x < m_delegates.Count(); x++ )
		{
			m_delegates.ElementAtRef(x)->Clear();
		}
		m_delegates.Clear();
	}

#if defined(DEBUG)
	void CheckMem() const
	{
		m_delegates.CheckMem();
	}

	void ValidateMem() const
	{
		m_delegates.ValidateMem();
	}
#endif
};

/** @brief Dispatcher for C# style delegates with three arguments.
 *	Use in place of implemting call back interfaces to symplify code.
 *	@ref DelegateThreeParameter
 */
template<typename ARG1, typename ARG2, typename ARG3>
class DelegateDispatchThreeParameter : public IMemoryValidate
{
private:
	Vector<RefCountPtr<IDelegateThreeParameter<ARG1, ARG2, ARG3> > > m_delegates;

public:
	DelegateDispatchThreeParameter()
	: m_delegates()
	{
	}

	virtual ~DelegateDispatchThreeParameter()
	{
	}

	inline void Add(RefCountPtr<IDelegateThreeParameter<ARG1, ARG2, ARG3> > d)
	{
		m_delegates.Add(d);
	}

	inline void operator +=(RefCountPtr<IDelegateThreeParameter<ARG1, ARG2, ARG3> > d)
	{
		Add(d);
	}

	void Call(ARG1 arg1, ARG2 arg2, ARG3 arg3)
	{
		for (int x = 0; x < m_delegates.Count(); x++ )
		{
			m_delegates.ElementAtRef(x)->Call(arg1, arg2, arg3);
		}
	}

	void Clear()
	{
		for (int x = 0; x < m_delegates.Count(); x++ )
		{
			m_delegates.ElementAtRef(x)->Clear();
		}
		m_delegates.Clear();
	}

#if defined(DEBUG)
	void CheckMem() const
	{
		m_delegates.CheckMem();
	}

	void ValidateMem() const
	{
		m_delegates.ValidateMem();
	}
#endif
};

/** @} */
}
#endif
