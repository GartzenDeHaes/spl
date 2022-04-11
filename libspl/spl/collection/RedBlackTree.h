/*
 *  Modified from code found on the net somewhere, need to re-google the attribution.
 */
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
#ifndef _redblacktree_h
#define _redblacktree_h

#include <spl/types.h>
#include <spl/Debug.h>
#ifdef DEBUG
#include <spl/collection/List.h>
#endif
#include <spl/Compare.h>
#include <spl/Exception.h>
#include <spl/collection/IEnumerable.h>
#include <spl/Int32.h>
#include <spl/IIterator.h>
#include <spl/collection/List.h>
#include <spl/Memory.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

template<typename K, typename V>
class RBNode : public IMemoryValidate
{
protected:
	K m_key;
	V m_data;

public:
	RBNode<K, V> *m_parent;
	RBNode<K, V> *m_left;
	RBNode<K, V> *m_right;
	bool m_isRed;

	RBNode(K key, V data)
	: m_key(key), m_data(data), m_parent(NULL), m_left(NULL), m_right(NULL), m_isRed(true)
	{ 
	}

	RBNode(const RBNode<K, V>& node)
	: m_key(node.m_key), m_data(node.m_data), m_parent(node.m_parent), m_left(node.m_left), m_right(node.m_right), m_isRed(node.m_isRed)
	{ 
	}

	virtual ~RBNode() {}

	RBNode<K, V>& operator =(const RBNode<K, V>& node)
	{
		m_key = node.m_key;
		m_data = node.m_data;
		m_parent = node.m_parent;
		m_left = node.m_left;
		m_right = node.m_right;
		m_isRed = node.m_isRed;
		return *this;
	}

	inline V GetData() const { return m_data; }
	inline V& GetDataRef() { return m_data; }
	inline const V& GetDataRef() const { return m_data; }
	inline void SetData(V data) { m_data = data; }
	inline K GetKey() const { return m_key; }
	inline void SetKey(const K key) { m_key = key; }
	
	inline void SetBlack() { m_isRed = false; }
	inline void SetRed() { m_isRed = true; }
	inline bool IsBlack() const { return !m_isRed; }
	inline bool IsRed() const { return m_isRed; }
	inline void SetColor(bool isred) { m_isRed = isred; }

	inline RBNode<K, V> *GetLeft() const { return m_left; }
	inline RBNode<K, V> *GetRight() const { return m_right; }
	inline RBNode<K, V> *GetParent() const { return m_parent; }
	
	inline RBNode<K, V> *GrandParent() const
	{
		ASSERT( NULL != m_parent && NULL != m_parent->m_parent );
		return m_parent->m_parent;
	}

	inline RBNode<K, V> *Uncle() const
	{
		if (m_parent == GrandParent()->m_left)
		{
			return GrandParent()->m_right;
		}
		else
		{
			return GrandParent()->m_left;
		}
	}

	inline RBNode<K, V> *Sibling() const
	{
		if ( NULL == m_parent )
		{
			return NULL;
		}
		if (this == m_parent->m_left)
		{
			return m_parent->m_right;
		}
		else
		{
			return m_parent->m_left;
		}
	}

	bool SiblingIsBlack() const
	{
		if ( NULL == m_parent )
		{
			return true;
		}
		if (this == m_parent->m_left)
		{
			return NULL == m_parent->m_right || m_parent->m_right->IsBlack();
		}
		else
		{
			return NULL == m_parent->m_left || m_parent->m_left->IsBlack();
		}
	}

	bool SiblingIsRed() const
	{
		if ( NULL == m_parent )
		{
			return false;
		}
		if (this == m_parent->m_left)
		{
			return (NULL == m_parent->m_right) ? false : m_parent->m_right->IsRed();
		}
		else
		{
			return (NULL == m_parent->m_left) ? false : m_parent->m_left->IsRed();
		}
	}

	void SiblingSetRed() 
	{
		if ( NULL == m_parent )
		{
			return;
		}
		if (this == m_parent->m_left)
		{
			if (NULL != m_parent->m_right) 
			{
				m_parent->m_right->SetRed();
			}
		}
		else
		{
			if (NULL != m_parent->m_left)
			{
				 m_parent->m_left->SetRed();
			}
		}
	}

	bool SiblingLeftIsBlack() const
	{
		RBNode<K, V> *s = Sibling();
		if ( NULL == s )
		{
			return true;
		}
		if ( NULL == (s = s->m_left) )
		{
			return true;
		}
		return s->IsBlack();
	}

	bool SiblingRightIsBlack() const
	{
		RBNode<K, V> *s = Sibling();
		if ( NULL == s )
		{
			return true;
		}
		if ( NULL == (s = s->m_right) )
		{
			return true;
		}
		return s->IsBlack();
	}

	bool SiblingLeftIsRed() const
	{
		RBNode<K, V> *s = Sibling();
		if ( NULL == s )
		{
			return false;
		}
		if ( NULL == (s = s->m_left) )
		{
			return false;
		}
		return s->IsRed();
	}

	bool SiblingRightIsRed() const
	{
		RBNode<K, V> *s = Sibling();
		if ( NULL == s )
		{
			return false;
		}
		if ( NULL == (s = s->m_right) )
		{
			return false;
		}
		return s->IsRed();
	}

	void SetRightChild(RBNode<K, V> *node) 
	{
		ASSERT( NULL == m_right );
		m_right = node;
		m_right->m_parent = this;
	}

	void SetLeftChild(RBNode<K, V> *node)
	{
		ASSERT( NULL == m_left );
		m_left = node;
		m_left->m_parent = this;
	}

	void Replace(RBNode<K, V> *node)
	{
		if ( NULL != m_parent )
		{
			if ( m_parent->m_left == this )
			{
				m_parent->m_left = node;
			}
			else
			{
				ASSERT( m_parent->m_right == this );
				m_parent->m_right = node;
			}
		}
		if ( NULL != node )
		{
			node->m_parent = m_parent;
		}
	}

	void Prune()
	{
		ASSERT( NULL == m_right && NULL == m_left );
		if ( NULL != m_parent )
		{
			if ( m_parent->m_left == this )
			{
				m_parent->m_left = NULL;
			}
			else
			{
				ASSERT( m_parent->m_right == this );
				m_parent->m_right = NULL;
			}
		}
	}

	void RotateLeft()
	{
		//Let Y be X's right child.
		RBNode<K, V> *y = m_right;
		//Set Y to be the new root.
		y->m_parent = m_parent;
		if ( NULL != m_parent )
		{
			if ( this == m_parent->m_left )
			{
				m_parent->m_left = y;
			}
			else
			{
				ASSERT(m_parent->m_right == this);
				m_parent->m_right = y;
			}
		}
		//Set X's right child to be Y's left child.
		if ( NULL != (m_right = y->m_left) )
		{
			m_right->m_parent = this;
		}
		//Set Y's left child to be X.
		y->m_left = this;
		m_parent = y;
	}

	void RotateRight()
	{
		//Let Y be X's left child.
		RBNode<K, V> *y = m_left;
		//Set Y to be the new root.
		y->m_parent = m_parent;
		if ( NULL != m_parent )
		{
			if ( this == m_parent->m_left )
			{
				m_parent->m_left = y;
			}
			else
			{
				ASSERT(m_parent->m_right == this);
				m_parent->m_right = y;
			}
		}
		//Set X's left child to be Y's right child.
		if ( NULL != (m_left = y->m_right) )
		{
			m_left->m_parent = this;
		}
		//Set Y's right child to be X.
		y->m_right = this;
		m_parent = y;
	}
#ifdef DEBUG
	void ValidateMem() const
	{
		TypeValidate( m_key );
		TypeValidate( m_data );
	}
	inline void CheckMem() const
	{
		TypeCheckMem( m_key );
		TypeCheckMem( m_data );
	}
#endif
};

/** @brief Iterate the tree */
template<typename K, typename V>
class RedBlackTreeIterator : public IIterator<RBNode<K, V> *>
{
private:
	List<RBNode<K, V> * > m_stack;
	RBNode<K, V> *m_cnode;

public:
	RedBlackTreeIterator()
	: m_stack(), m_cnode(NULL)
	{
	}

	RedBlackTreeIterator(RBNode<K, V> *cnode)
	: m_stack(), m_cnode(cnode)
	{
	}

	RedBlackTreeIterator(const RedBlackTreeIterator<K, V>& ti)
	: m_stack(ti.m_stack), m_cnode(ti.m_cnode)
	{
	}

	virtual bool Next( )
	{
		if (NULL == m_cnode)
		{
			return false;
		}
		if ( NULL != m_cnode->m_left )
		{
			if ( NULL != m_cnode->m_right )
			{
				m_stack.Add( m_cnode->m_right );
			}
			m_cnode = m_cnode->m_left;
			return true;
		}
		else if ( NULL != m_cnode->m_right )
		{
			m_cnode = m_cnode->m_right;
			return true;
		}
		else if ( m_stack.Count() > 0 )
		{
			m_cnode = m_stack.Pop();
			return true;
		}
		else
		{
			m_cnode = NULL;
			return false;
		}
	}

	virtual bool Prev( )
	{
		throw new NotImplementedException();
	}

	virtual RBNode<K, V> * Current()
	{
		return m_cnode;
	}

	virtual RBNode<K, V> *& CurrentRef()
	{
		return m_cnode;
	}
};

/** @brief A balanced binary tree where the longest path from the root to any leaf is no more than twice as long as the shortest path from the root to any other leaf in that tree.  */
template<typename K, typename V>
class RedBlackTree : public IEnumerable<V>
{
private:
	RBNode<K, V> *m_root;
	int m_count;

public:
	virtual RefCountPtr<IIterator<V> > _Begin()
	{
		return RefCountPtr<IIterator<V> >((IIterator<V> *)new RedBlackTreeIterator<K, V>(m_root));
	}

	/** @brief Iterate the tree values. */
	class Iterator : public IIterator<V>
	{
	private:
		RedBlackTreeIterator<K, V> m_iter;

	public:
		Iterator()
		: m_iter()
		{
		}

		Iterator(RBNode<K, V> *cnode)
		: m_iter(cnode)
		{
		}

		Iterator(const Iterator& ti)
		: m_iter(ti)
		{
		}

		virtual bool Next( )
		{
			return m_iter.Next();
		}

		virtual bool Prev( )
		{
			return m_iter.Prev();
		}

		virtual V Current()
		{
			return m_iter.Current()->GetData();
		}

		virtual V& CurrentRef()
		{
			return m_iter.Current()->GetDataRef();
		}
	};

	RedBlackTree()
	: m_root(NULL), m_count(0)
	{
	}

	RedBlackTree(const RedBlackTree<K, V>& rbt)
	: m_root(NULL), m_count(0)
	{
		*this = rbt;
	}

	~RedBlackTree()
	{
		Clear();
	}

	RedBlackTree<K, V>& operator =(const RedBlackTree<K, V>& rbt)
	{
		Clear();

		if ( NULL == m_root )
		{
			throw OutOfMemoryException();
		}
		List<RBNode<K, V> * > stack;
		RBNode<K, V> *cnode = m_root;
		while ( NULL != cnode )
		{
			Insert( cnode.GetKey(), cnode.GetData() );

			if ( NULL != cnode->m_left )
			{
				if ( NULL != cnode->m_right )
				{
					stack.Add( cnode->m_right );
				}
				cnode = cnode->m_left;
			}
			else if ( NULL != cnode->m_right )
			{
				cnode = cnode->m_right;
			}
			else if ( stack.Count() > 0 )
			{
				cnode = stack.Pop();
			}
			else
			{
				cnode = NULL;
			}
		}

		return this;
	}

	inline RedBlackTreeIterator<K, V> NodeIterator()
	{
		return RedBlackTreeIterator<K, V>(m_root);
	}

	inline Iterator Begin() const
	{
		return Iterator(m_root);
	}

	int Count() const
	{
		return m_count;
	}

	void Insert( const K key, V data )
	{
		RBNode<K, V> *cnode = m_root;
		m_count++;
		if ( NULL == cnode )
		{
			if ( NULL == (m_root = CreateNode(key, data)) )
			{
				throw OutOfMemoryException();
			}
			m_root->SetBlack();
			return;
		}
		while ( true )
		{
			int cmp = (int)Compare::Cmp(cnode->GetKey(), key );
			if ( 0 == cmp )
			{
				throw new InvalidArgumentException("Duplicate key");
			}
			if ( 0 > cmp )
			{
				if ( NULL == cnode->GetRight() )
				{
					RBNode<K, V> *rt = CreateNode(key, data);
					if ( NULL == rt )
					{
						throw OutOfMemoryException();
					}
					cnode->SetRightChild(rt);
					InsertCase1(rt);
					break;
				}
				else
				{
					cnode = cnode->GetRight();
				}
			}
			else
			{
				if ( NULL == cnode->GetLeft() )
				{
					RBNode<K, V> *rt = CreateNode(key, data);
					if ( NULL == rt )
					{
						throw OutOfMemoryException();
					}
					cnode->SetLeftChild(rt);
					InsertCase1(rt);
					break;
				}
				else
				{
					cnode = cnode->GetLeft();
				}
			}
		}
		VerifyTree();
	}

	void Remove( const K& key )
	{
		RBNode<K, V> *n = FindNode( key );
		if ( NULL == n )
		{
			return;
		}
		if ( n->GetRight() != NULL && n->GetLeft() != NULL )
		{
			// Node has two children - get max of left subtree
			RBNode<K, V> *temp = n->m_left;
			ASSERT_PTR( temp );
			while (NULL != temp->m_right) 
			{
				temp = temp->m_right;
			}
			K tkey = temp->GetKey();
			V tdata = temp->GetData();
			Remove( tdata );
			n->SetData(tdata);
			n->SetKey(tkey);
			VerifyTree();
			return;
		}
		m_count--;
		/* Precondition: n has at most one non-null child */
		RBNode<K, V> *child = (NULL == n->m_right) ? n->m_left : n->m_right;
		if ( n == m_root )
		{
			if ( NULL != child )
			{
				ASSERT( n == child->m_parent );
				child->m_parent = NULL;
			}
			m_root = child;
		}
		else
		{
			n->Replace(child);
		}
		if (n->IsBlack() && NULL != child)
		{
			if (child->IsRed())
			{
				child->SetBlack();
			}
			else
			{
				DeleteCase1(child);
			}
		}
		delete n;
		VerifyTree();
	}

	void Clear()
	{
		while ( m_count > 0 )
		{
			Remove( m_root->GetKey() );
		}
	}

	V Find( const K& key ) const
	{
		RBNode<K, V> *cnode = FindNode( key );
		if ( NULL != cnode )
		{
			return cnode->GetData();
		}
		return V();
	}

	V& FindRef( const K& key ) const
	{
		RBNode<K, V> *cnode = FindNode( key );
		if ( NULL != cnode )
		{
			return cnode->GetDataRef();
		}
		throw InvalidArgumentException("key not found");
	}

	bool ContainsKey( const K& key ) const
	{
		return NULL != FindNode( key );
	}

#ifdef DEBUG
	void VerifyTree() const
	{
		int count = 0;
		if ( NULL == m_root )
		{
			ASSERT( m_count == 0 );
			return;
		}
		ASSERT( NULL == m_root->m_parent );

		List<RBNode<K, V> * > stack;
		RBNode<K, V> *cnode = m_root;
		while ( NULL != cnode )
		{
			ASSERT_PTR( cnode );
			count++;
			if ( NULL != cnode->m_parent )
			{
				ASSERT( cnode == cnode->m_parent->m_left || cnode == cnode->m_parent->m_right );
			}
			else
			{
				ASSERT( cnode == m_root );
			}
			if ( cnode->IsRed() )
			{
				ASSERT( cnode->m_left == NULL || cnode->m_left->IsBlack() );
				ASSERT( cnode->m_right == NULL || cnode->m_right->IsBlack() );
			}
			if ( NULL != cnode->m_left )
			{
				if ( NULL != cnode->m_right )
				{
					stack.Add( cnode->m_right );
				}
				cnode = cnode->m_left;
			}
			else if ( NULL != cnode->m_right )
			{
				cnode = cnode->m_right;
			}
			else if ( stack.Count() > 0 )
			{
				cnode = stack.Pop();
			}
			else
			{
				cnode = NULL;
			}
		}
		ASSERT( m_count == count );
	}

	virtual void ValidateMem() const
	{
		if ( NULL == m_root )
		{
			return;
		}
		List<RBNode<K, V> * > stack;
		RBNode<K, V> *cnode = m_root;
		while ( NULL != cnode )
		{
			ASSERT_PTR( cnode );
			cnode->ValidateMem();
			if ( NULL != cnode->m_left )
			{
				if ( NULL != cnode->m_right )
				{
					stack.Add( cnode->m_right );
				}
				cnode = cnode->m_left;
			}
			else if ( NULL != cnode->m_right )
			{
				cnode = cnode->m_right;
			}
			else if ( stack.Count() > 0 )
			{
				cnode = stack.Pop();
			}
			else
			{
				cnode = NULL;
			}
		}
	}

	virtual void CheckMem() const
	{
		if ( NULL == m_root )
		{
			return;
		}
		List<RBNode<K, V> * > stack;
		RBNode<K, V> *cnode = m_root;
		while ( NULL != cnode )
		{
			DEBUG_NOTE_MEM_ALLOCATION( cnode );
			cnode->CheckMem();
			if ( NULL != cnode->m_left )
			{
				if ( NULL != cnode->m_right )
				{
					stack.Add( cnode->m_right );
				}
				cnode = cnode->m_left;
			}
			else if ( NULL != cnode->m_right )
			{
				cnode = cnode->m_right;
			}
			else if ( stack.Count() > 0 )
			{
				cnode = stack.Pop();
			}
			else
			{
				cnode = NULL;
			}
		}
	}
#else
	inline void VerifyTree() const {}
#endif

protected:
	inline RBNode<K, V> *CreateNode(const K key, V data)
	{
		return new RBNode<K, V>(key, data);
	}

	void RotateLeft(RBNode<K, V> *node)
	{
		node->RotateLeft();
		if ( node == m_root )
		{
			m_root = node->m_parent;
		}
	}

	void RotateRight(RBNode<K, V> *node)
	{
		node->RotateRight();
		if ( node == m_root )
		{
			m_root = node->m_parent;
		}
	}

	RBNode<K, V> *FindNode( const K& key ) const
	{
		VerifyTree();
		RBNode<K, V> *cnode = m_root;
		while ( NULL != cnode )
		{
			int cmp = (int)Compare::Cmp(cnode->GetKey(), key );
			if ( 0 == cmp )
			{
				return cnode;
			}
			if ( 0 > cmp )
			{
				cnode = cnode->GetRight();
			}
			else
			{
				cnode = cnode->GetLeft();
			}
		}
		return NULL;
	}

	void DeleteCase1(RBNode<K, V> *n)
	{
		if (n->m_parent == NULL)
		{
			return;
		}
		else
		{
			DeleteCase2(n);
		}
	}

	void DeleteCase2(RBNode<K, V> *n)
	{
		if (n->SiblingIsRed()) 
		{
			n->m_parent->SetRed();
			n->Sibling()->SetBlack();
			if (n == n->m_parent->m_left)
			{
				RotateLeft(n->m_parent);
			}
			else
			{
				RotateRight(n->m_parent);
			}
		}
		DeleteCase3(n);
	}

	void DeleteCase3(RBNode<K, V> *n)
	{
		if (n->m_parent->IsBlack() &&
			n->SiblingIsBlack() &&
			n->SiblingLeftIsBlack() &&
			n->SiblingRightIsBlack())
		{
			n->SiblingSetRed();
			DeleteCase1(n->m_parent);
		}
		else
		{
			DeleteCase4(n);
		}
	}

	void DeleteCase4(RBNode<K, V> *n)
	{
		if (n->m_parent->IsRed() &&
			n->SiblingIsBlack() &&
			n->SiblingLeftIsBlack() &&
			n->SiblingRightIsBlack())
		{
			n->SiblingSetRed();
			n->m_parent->SetBlack();
		}
		else
		{
			DeleteCase5(n);
		}
	}

	void DeleteCase5(RBNode<K, V> *n)
	{
		if (n == n->m_parent->m_left &&
			n->SiblingIsBlack() &&
			n->SiblingLeftIsRed() &&
			n->SiblingRightIsBlack())
		{
			ASSERT(NULL != n->Sibling() && NULL != n->Sibling()->m_left);
			n->Sibling()->SetRed();
			n->Sibling()->m_left->SetBlack();
			RotateRight(n->Sibling());
		}
		else if (n == n->m_parent->m_right &&
				 n->SiblingIsBlack() &&
				 n->SiblingRightIsRed() &&
				 n->SiblingLeftIsBlack())
		{
			ASSERT(NULL != n->Sibling() && NULL != n->Sibling()->m_right);
			n->Sibling()->SetRed();
			n->Sibling()->m_right->SetBlack();
			RotateLeft(n->Sibling());
		}
		DeleteCase6(n);
	}

	void DeleteCase6(RBNode<K, V> *n)
	{
		ASSERT(NULL != n->Sibling());
		n->Sibling()->SetColor(n->m_parent->IsRed());
		n->m_parent->SetBlack();
		if (n == n->m_parent->m_left) 
		{
			/* Here, sibling(n)->right->color == RED */
			ASSERT(NULL != n->Sibling()->m_right);
			n->Sibling()->m_right->SetBlack();
			RotateLeft(n->m_parent);
		}
		else
		{
			/* Here, sibling(n)->left->color == RED */
			ASSERT(NULL != n->Sibling()->m_left);
			n->Sibling()->m_left->SetBlack();
			RotateRight(n->m_parent);
		}
	}

	void InsertCase1(RBNode<K, V> *n)
	{
		if (n->m_parent == NULL)
		{
			n->m_isRed = false;
		}
		else
		{
			InsertCase2(n);
		}
	}

	void InsertCase2(RBNode<K, V> *n)
	{
		if (!n->m_parent->m_isRed)
		{
			return; /* Tree is still valid */
		}
		else
		{
			InsertCase3(n);
		}
	}
	
	void InsertCase3(RBNode<K, V> *n)
	{
		RBNode<K, V> *uncle = n->Uncle();
		if (uncle != NULL && uncle->m_isRed) 
		{
			n->m_parent->m_isRed = false;
			uncle->m_isRed = false;
			n->GrandParent()->m_isRed = true;
			InsertCase1(n->GrandParent());
		}
		else
		{
			InsertCase4(n);
		}
	}
	
	void InsertCase4(RBNode<K, V> *n)
	{
		if (n == n->m_parent->m_right && n->m_parent == n->GrandParent()->m_left) 
		{
			RotateLeft(n->m_parent);
			n = n->m_left;
		} 
		else if (n == n->m_parent->m_left && n->m_parent == n->GrandParent()->m_right) 
		{
			RotateRight(n->m_parent);
			n = n->m_right;
		}
		InsertCase5(n);
	}

	void InsertCase5(RBNode<K, V> *n)
	{
		n->m_parent->m_isRed = false;
		n->GrandParent()->m_isRed = true;
		if (n == n->m_parent->m_left && n->m_parent == n->GrandParent()->m_left) 
		{
			RotateRight(n->GrandParent());
		} 
		else 
		{
			/* Here, n == n->parent->right && n->parent == grandparent(n)->right */
			RotateLeft(n->GrandParent());
		}
	}
};

REGISTER_TYPEOF2( 350, RedBlackTree<int,int> );
REGISTER_TYPEOF2( 351, RedBlackTree<String,String> );
REGISTER_TYPEOF2( 590, RedBlackTree<int,Int32> );

/** @} */
}
#endif
