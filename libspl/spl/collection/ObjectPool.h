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
#ifndef _objectpool_h
#define _objectpool_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/List.h>
#include <spl/Memory.h>
#include <spl/threading/Mutex.h>
#include <spl/RefCountPtr.h>

namespace spl
{
/**
 * @defgroup collection Collections
 * @{
 */

///@brief Interface to make classes poolable.
class IPoolable : public IMemoryValidate
{
public:
	IPoolable() {}
	virtual ~IPoolable();	// ensures destructors are called

	virtual void OnPreRemoveFromPool() = 0;
	virtual void OnPreReturnToPool() = 0;

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const = 0;
	virtual void ValidateMem() const = 0;
#endif
};

typedef RefCountPtr<IPoolable> IPoolablePtr;

///@brief Factory interface for ObjectPool to create and delete pooled objects.
class IPoolableFactory : public IMemoryValidate
{
public:
	IPoolableFactory() {}
	virtual ~IPoolableFactory() {}	// ensures destructors are called

	virtual IPoolable *CreateInstance() = 0;
	virtual void Destroy( IPoolable *instance ) = 0;

#if defined(DEBUG) || defined(_DEBUG)
	virtual void CheckMem() const = 0;
	virtual void ValidateMem() const = 0;
#endif
};

///@brief Manages a pool of IPooable objects.
class ObjectPool : public IMemoryValidate
{
private:
	// Copy constructor doesn't make sense for this class
	inline ObjectPool(const ObjectPool& op) {}
	inline void operator =(const ObjectPool& op) {}

protected:
	IPoolableFactory *m_factory;
	List<IPoolable *> m_pool;
	Mutex m_poolMutex;
	int m_minInstances;
	int m_preferedPoolMaxInstances;
	int m_maxInstances;
	int m_count;

public:
	///@brief factory is not deleted by ObjectPool.
	ObjectPool(IPoolableFactory *factory, int minInstances, int preferedPoolMaxInstances, int maxInstances);
	virtual ~ObjectPool();

	IPoolablePtr GetObjectSmartPtr();
	IPoolable *GetObject();

	void ReleaseObject( IPoolable *item );
	void ReleaseObject( IPoolablePtr item );

	inline int Count() const { return m_count; }

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

REGISTER_TYPEOF( 404, ObjectPool );

/** @} */
}
#endif
