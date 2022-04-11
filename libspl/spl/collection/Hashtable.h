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
#ifndef _thashtable_h
#define _thashtable_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Compare.h>
#include <spl/Exception.h>
#include <spl/collection/IEnumerable.h>
#include <spl/IIterator.h>
#include <spl/collection/List.h>
#include <spl/math/Math.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

/* For resizing the hashtable */
extern const int __hashtableSizes[];

/** @brief A key value map.
 *	Dynamically resizes based on a 75% fill factor.
 *	@todo Allow the user to specify the initial table size.
 */
template<typename K, typename T>
class Hashtable : public IEnumerable<T>
{
private:
	class _Kthashitem : public IMemoryValidate
	{
	public:
		K key;
		T data;

		_Kthashitem() : key(), data() {}
		_Kthashitem(const _Kthashitem& item) : key(item.key), data(item.data) {}
		~_Kthashitem() {}

	#if defined(DEBUG)
		virtual void CheckMem() const
		{
			TypeCheckMem( key );
			TypeCheckMem( data );
		}
		virtual void ValidateMem() const
		{
			TypeValidate( key );
			TypeValidate( data );
		}
	#endif
	};
	
	void Resize()
	{
		if (__hashtableSizes[m_tablesize+1] == -1)
		{
			return;
		}
		m_tablesize++;
		Vector<List<_Kthashitem *> *> vlold(m_valueLists);
		m_valueLists.Clear();
		m_valueLists = Vector<List<_Kthashitem *> *>(TableSize());
		_Clear(m_valueLists);
		m_count = 0;
		
		int x;
		int count = vlold.Count();
		for ( x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = vlold.ElementAt(x);
			if ( list != NULL )
			{
				typename List<_Kthashitem *>::Iterator iter = list->Begin();
				while ( iter.Next() )
				{
					this->Set(iter.Current()->key, iter.Current()->data);
				}
			}
		}
		_Clear(vlold);
	}

	inline void CheckForResize()
	{
		if ((double)m_count / (double)TableSize() > 0.749)
		{
			Resize();
		}
	}
		
protected:
	Vector<List<_Kthashitem *> *> m_valueLists;
	int m_tablesize;
	int m_count;
	
public:
	class Iterator : public IIterator<T>
	{
	private:
		const Vector<List<_Kthashitem *> *> *m_valueLists;
		typename Vector<List<_Kthashitem *> *>::Iterator m_valueListsPos;
		typename List<_Kthashitem *>::Iterator m_listPos;

	public:
		Iterator(const Vector<List<_Kthashitem *> *> *valueLists)
		: m_valueLists(valueLists), m_valueListsPos(valueLists->Begin()), m_listPos()
		{
		}

		Iterator(const Iterator& hki)
		: m_valueLists(hki.m_valueLists), m_valueListsPos(hki.m_valueListsPos), m_listPos(hki.m_listPos)
		{
		}

		virtual bool Next( )
		{
			if (m_listPos.Next())
			{
				return true;
			}
			while (m_valueListsPos.Next())
			{
				if (NULL != m_valueListsPos.Current())
				{
					m_listPos = m_valueListsPos.Current()->Begin();
					if (m_listPos.Next())
					{
						return true;
					}
				}
			}

			return false;
		}

		virtual bool Prev( )
		{
			throw new NotImplementedException();
		}

		virtual T Current()
		{
			return m_listPos.Current()->data;
		}

		virtual T& CurrentRef()
		{
			return m_listPos.Current()->data;
		}

		virtual K CurrentKey()
		{
			return m_listPos.Current()->key;
		}

		virtual K& CurrentKeyRef()
		{
			return m_listPos.Current()->key;
		}
	};

protected:
	inline List<_Kthashitem *> *FindList( const K& key ) const
	{
		int hash = Math::Hash(key) % TableSize();
		ASSERT( hash > -1 );
		ASSERT( m_valueLists.Count() >  hash );
		return m_valueLists.ElementAt( hash );
	}
	
	inline List<_Kthashitem *> *AddList( const K& key )
	{
		List<_Kthashitem *> *list = new List<_Kthashitem *>();
		m_valueLists.SetElementAt( list, Math::Hash(key) % TableSize() );
		return list;
	}
	
	inline bool FindItem(typename List<_Kthashitem *>::Iterator& list, const K& key) const
	{
		while ( list.Next() )
		{
			if ( Compare::Equals(list.Current()->key, key) )
			{
				return true;
			}
		}
		return false;
	}
	
	virtual RefCountPtr<IIterator<T> > _Begin()
	{
		return RefCountPtr<IIterator<T> >(new Iterator(&m_valueLists));
	}

	inline int TableSize() const
	{
		return __hashtableSizes[m_tablesize];
	}
	
	void _Clear(Vector<List<_Kthashitem *> *>& valueLists)
	{
		int x;
		int count = valueLists.Count();
		for ( x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = valueLists.ElementAt(x);
			if ( list != NULL )
			{
				typename List<_Kthashitem *>::Iterator iter = list->Begin();
				while ( iter.Next() )
				{
					delete iter.Current();
				}
				delete list;
			}
		}
		valueLists.Clear();

		for ( x = 0; x < TableSize(); x++ )
		{
			valueLists.Add( NULL );
		}
	}

public:
	Hashtable(  )
	: m_count(0), m_tablesize(0), m_valueLists(__hashtableSizes[0])
	{
		ASSERT(Math::IsPrime(TableSize()));
		Clear();
	}

	Hashtable( const Hashtable &ht )
	: m_count(ht.m_count), m_tablesize(ht.m_tablesize), m_valueLists(__hashtableSizes[ht.m_tablesize])
	{
		Clear();
		
		int count = ht.m_valueLists.Count();
		for ( int x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = ht.m_valueLists.ElementAt(x);
			if ( list != NULL )
			{
				int lcount = list->Count();
				for ( int y = 0; y < lcount; y++ )
				{
					_Kthashitem *item = list->ElementAt(y);
					ASSERT_MEM( item, sizeof(_Kthashitem) );
					Set( item->key, item->data );
				}
			}
		}
	}

	virtual ~Hashtable()
	{
		int x;
		int count = m_valueLists.Count();
		for ( x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = m_valueLists.ElementAt(x);
			if ( list != NULL )
			{
				typename List<_Kthashitem *>::Iterator iter = list->Begin();
				while ( iter.Next() )
				{
					delete iter.Current();
				}
				delete list;
			}
		}
	}

	Hashtable<K, T>& operator =(const Hashtable<K, T>& ht)
	{
		int x;

		Clear();

		m_count = ht.m_count;

		if ( m_tablesize != ht.m_tablesize )
		{
			m_tablesize = ht.m_tablesize;
			m_valueLists.Clear();

			for ( x = 0; x < TableSize(); x++ )
			{
				m_valueLists.Add( NULL );
			}
		}

		for ( x = 0; x < TableSize(); x++ )
		{
			List<_Kthashitem *> *list = ht.m_valueLists.ElementAt(x);
			if ( list != NULL )
			{
				int lcount = list->Count();
				for ( int y = 0; y < lcount; y++ )
				{
					_Kthashitem *item = list->ElementAt(y);
					ASSERT_MEM( item, sizeof(_Kthashitem) );
					Set( item->key, item->data );
				}
			}
		}
		return *this;
	}

	bool ContainsKey( const K& key ) const
	{
		List<_Kthashitem *> *list = FindList( key );
		if ( NULL == list )
		{
			return false;
		}
		typename List<_Kthashitem *>::Iterator iter = list->Begin();
		return FindItem( iter, key );
	}
	
	T Get( const K& key ) const
	{
		List<_Kthashitem *> *list = FindList( key );
		if ( NULL == list )
		{
			return T();
		}
		typename List<_Kthashitem *>::Iterator iter = list->Begin();
		if ( ! FindItem( iter, key ) )
		{
			return T();
		}
		return iter.Current()->data;
	}
	
	T& GetRef( const K& key ) const
	{
		List<_Kthashitem *> *list = FindList( key );
		if ( NULL == list )
		{
			throw new IndexOutOfBoundsException("key not found");
		}
		typename List<_Kthashitem *>::Iterator iter = list->Begin();
		if ( ! FindItem( iter, key ) )
		{
			throw new IndexOutOfBoundsException("key not found");
		}
		return iter.Current()->data;
	}
	
	inline T& operator[] (const K& key) const
	{
		return GetRef(key);
	}

	void Set( K key, T val )
	{
		List<_Kthashitem *> *list = FindList( key );
		if ( NULL == list )
		{
			list = AddList( key );
			m_count++;
		}
		else
		{
			typename List<_Kthashitem *>::Iterator iter = list->Begin();
			if ( FindItem( iter, key ) )
			{
				_Kthashitem *item = iter.Current();
				list->RemoveCurrent(iter);
				delete item;
			}
			else
			{
				m_count++;
			}
		}
		_Kthashitem *item = new _Kthashitem();
		if ( NULL == item )
		{
			throw OutOfMemoryException();
		}
		item->key = key;
		item->data = val;
		list->Add( item );
				
		CheckForResize();
	}

	void Remove( const K& key )
	{
		List<_Kthashitem *> *list = FindList( key );
		if ( NULL == list )
		{
			return;
		}
		typename List<_Kthashitem *>::Iterator iter = list->Begin();
		if ( ! FindItem( iter, key ) )
		{
			return;
		}
		_Kthashitem *item = iter.Current();
		list->RemoveCurrent(iter);
		delete item;
		m_count--;
	}

	RefCountPtr<Vector<K> > Keys() const
	{
		RefCountPtr<Vector<K> > vect(new Vector<K>());
		int count = m_valueLists.Count();
		for ( int x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = m_valueLists.ElementAt(x);
			if ( NULL == list )
			{
				continue;
			}
			ASSERT_MEM(list, sizeof(List<_Kthashitem *>));
			list->ValidateMem();

			typename List<_Kthashitem *>::Iterator iter = list->Iterator();
			while(iter.Next())
			{
				vect->Add(iter.Current()->key);
			}
		}
		ASSERT(m_count == vect->Count());
		return vect;
	}
	
	inline Iterator Begin() const
	{
		return Iterator(&m_valueLists);
	}

	inline void ForEachKey(IDelegateOneParameter<K&>& func)
	{
		Iterator iter = Begin();
		while(iter.Next())
		{
			func.Call(iter->Current(iter.CurrentRef()));
		}
	}

	RefCountPtr<Vector<T> > Values() const
	{
		RefCountPtr<Vector<T> > vect(new Vector<T>());
		int count = m_valueLists.Count();
		for ( int x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = m_valueLists.ElementAt(x);
			if ( NULL == list )
			{
				continue;
			}
			ASSERT_MEM(list, sizeof(List<_Kthashitem *>));
			list->ValidateMem();

			typename List<_Kthashitem *>::Iterator iter = list->Begin();
			while(iter.Next())
			{
				vect->Add(iter.Current()->data);
			}
		}
		ASSERT(m_count == vect->Count());
		return vect;
	}

	inline void Clear()
	{
		_Clear(m_valueLists);
		m_count = 0;
	}

	inline int Count() const
	{
		return m_count;
	}

#ifdef DEBUG
	virtual void ValidateMem() const
	{
		m_valueLists.ValidateMem();
		int count = m_valueLists.Count();
		for ( int x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = m_valueLists.ElementAt(x);
			if ( list != NULL )
			{
				ASSERT_MEM( list, sizeof(List<_Kthashitem *>) );
				list->ValidateMem();
			}
		}
	}

	virtual void CheckMem() const
	{
		m_valueLists.CheckMem();
		int count = m_valueLists.Count();
		for ( int x = 0; x < count; x++ )
		{
			List<_Kthashitem *> *list = m_valueLists.ElementAt(x);
			if ( list != NULL )
			{
				DEBUG_NOTE_MEM( list );
				list->CheckMem();
			}
		}
	}
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

/** @} */
}
#endif
