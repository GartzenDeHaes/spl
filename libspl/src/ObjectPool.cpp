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
#include <spl/collection/ObjectPool.h>

using namespace spl;

IPoolable::~IPoolable()
{
}

ObjectPool::ObjectPool
(
	IPoolableFactory *factory, 
	int minInstances, 
	int preferedPoolMaxInstances, 
	int maxInstances
)
:	m_factory(factory),
	m_minInstances(minInstances), 
	m_preferedPoolMaxInstances(preferedPoolMaxInstances),
	m_maxInstances(maxInstances),
	m_pool(),
	m_poolMutex(),
	m_count(0)
{
	for ( int x = 0; x < minInstances; x++ )
	{
		m_pool.Add( factory->CreateInstance() );	
	}
	m_count = minInstances;
}

ObjectPool::~ObjectPool()
{
	while ( m_pool.Count() > 0 )
	{
		IPoolable *item = m_pool.Pop();
		m_factory->Destroy( item );
	}
}

IPoolablePtr ObjectPool::GetObjectSmartPtr()
{
	return IPoolablePtr(GetObject());
}

IPoolable *ObjectPool::GetObject()
{
	IPoolable *item;
	m_poolMutex.Lock();
	if ( m_pool.Count() == 0 )
	{
		if ( m_count >= m_maxInstances )
		{
			m_poolMutex.Unlock();
			return NULL;
		}
		item = m_factory->CreateInstance();
		ASSERT( NULL != item );
		m_count++;
	}
	else
	{
		item = m_pool.Pop();
	}
	m_poolMutex.Unlock();
	item->OnPreRemoveFromPool();
	return item;
}

void ObjectPool::ReleaseObject( IPoolablePtr item )
{
	IPoolable *ptr = item.Detach();
	ReleaseObject(ptr);
}

void ObjectPool::ReleaseObject( IPoolable *item )
{
	item->OnPreReturnToPool();
	m_poolMutex.Lock();
	if ( m_count > m_preferedPoolMaxInstances )
	{
		m_factory->Destroy( item );
		m_count--;
	}
	else
	{
		m_pool.Add( item );
	}
	m_poolMutex.Unlock();
}

#if defined(DEBUG) || defined(_DEBUG)
void ObjectPool::CheckMem() const
{
	m_factory->CheckMem();
	//m_poolMutex.Lock();
	m_pool.CheckMem();
	int count = m_pool.Count();
	for ( int x = 0; x < count; x++ )
	{
		IPoolable *item = m_pool.ElementAt(x);
		DEBUG_NOTE_MEM( item );
		item->CheckMem();
	}
	//m_poolMutex.Unlock();
}

void ObjectPool::ValidateMem() const
{
	m_factory->ValidateMem();
	//m_poolMutex.Lock();
	m_pool.ValidateMem();
	int count = m_pool.Count();
	for ( int x = 0; x < count; x++ )
	{
		IPoolable *item = m_pool.ElementAt(x);
		ASSERT_PTR( item );
		item->ValidateMem();
	}
	//m_poolMutex.Unlock();
}
#endif

