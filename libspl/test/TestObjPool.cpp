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
#include <spl/Debug.h>
#include <spl/io/log/Log.h>
#include <spl/collection/ObjectPool.h>

#ifdef DEBUG

using namespace spl;

class TestPoolObj : public IPoolable
{
protected:
	int m_count;
public:
	TestPoolObj(int count) : m_count(count) {}
	~TestPoolObj() {}
	virtual void OnPreRemoveFromPool()
	{
	}
	virtual void OnPreReturnToPool()
	{
	}
	int Count() { return m_count; }

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const {}
	void ValidateMem() const {}
#endif
};

class TestPoolFactory : public IPoolableFactory
{
protected:
	int m_count;
public:
	TestPoolFactory() : m_count(0) {}
	virtual ~TestPoolFactory() {}	// ensures destructors are called

	virtual IPoolable *CreateInstance()
	{
		return new TestPoolObj(m_count++);
	}
	virtual void Destroy( IPoolable *instance ) 
	{
		delete (TestPoolObj *)instance;
	}

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const {}
	void ValidateMem() const {}
#endif
};

static void _TestObjPool1()
{
	TestPoolFactory factory;
	ObjectPool pool(&factory, 0, 1, 2);
	Log::SWriteOkFail( "ObjectPool test 1" );
}

static void _TestObjPool2()
{
	TestPoolFactory factory;
	ObjectPool pool(&factory, 0, 1, 2);

	TestPoolObj *obj1 = (TestPoolObj *)pool.GetObject();
	UNIT_ASSERT( "object 1", NULL != obj1 && obj1->Count() == 0 );
	UNIT_ASSERT( "pool count 1", 1 == pool.Count() );

	TestPoolObj *obj2 = (TestPoolObj *)pool.GetObject();
	UNIT_ASSERT( "object 2", NULL != obj2 && obj2->Count() == 1 );
	UNIT_ASSERT( "pool count 2", 2 == pool.Count() );

	TestPoolObj *obj3 = (TestPoolObj *)pool.GetObject();
	UNIT_ASSERT( "should be null", NULL == obj3 );
	UNIT_ASSERT( "no object created", 2 == pool.Count() );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( obj1 );
	DEBUG_NOTE_MEM( obj2 );
	pool.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("ObjectPool test 2.1");

	pool.ReleaseObject( obj1 );
	UNIT_ASSERT( "perfered count is one, so should delete this", 1 == pool.Count() );

	pool.ReleaseObject( obj2 );
	UNIT_ASSERT( "should keep this one", 1 == pool.Count() );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	pool.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("ObjectPool test 2.1");

	Log::SWriteOkFail( "ObjectPool test 2" );
}

void TestObjectPool()
{
	_TestObjPool1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestObjPool2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
