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
#ifndef _association_h
#define _association_h

#include <spl/Debug.h>
#include <spl/Memory.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

/** @brief A key value pair. */
template<typename KT, typename VT>
class Association : public IMemoryValidate
{
protected:
	KT m_key;
	VT m_val;

public:
	inline Association()
	: m_key(), m_val()
	{
	}

	inline Association(KT key, VT val)
	: m_key(key), m_val(val)
	{
	}

	inline Association(const Association& assoc)
	: m_key(assoc.m_key), m_val(assoc.m_val)
	{
	}

	virtual ~Association()
	{
	}

	inline KT Key()
	{
		return m_key;
	}

	inline VT Value()
	{
		return m_val;
	}

	inline KT& KeyRef()
	{
		return m_key;
	}

	inline VT& ValueRef()
	{
		return m_val;
	}

	inline Association<KT, VT>& operator =(const Association<KT, VT>& a)
	{
		m_key = a.m_key;
		m_val = a.m_val;

		return *this;
	}

#ifdef DEBUG
	virtual void ValidateMem() const
	{
		TypeValidate( m_key );
		TypeValidate( m_val );
	}

	virtual void CheckMem() const
	{
		TypeCheckMem( m_key );
		TypeCheckMem( m_val );
	}
#endif
};

/** @} */
}
#endif
