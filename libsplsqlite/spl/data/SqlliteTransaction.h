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
#ifndef _sqllitetrans_h
#define _sqllitetrans_h

#include <spl/data/Transaction.h>
#include <spl/RefCountPtrCast.h>
#include <spl/WeakReference.h>

namespace spl
{
/** 
 * @defgroup sqlite SQLITE
 * @ingroup database
 * @{
 */

class SqlLiteTransaction;
typedef RefCountPtrCast<SqlLiteTransaction, Transaction, TransactionPtr> SqlLiteTransactionPtr;
typedef WeakReference<SqlLiteTransaction, SqlLiteTransactionPtr> SqlLiteTransactionRef;

class SqlLiteTransaction : public Transaction
{
protected:
	void *m_db;

	inline SqlLiteTransaction(const SqlLiteTransaction&) {}
	inline SqlLiteTransaction& operator =(const SqlLiteTransaction&) {}

public:
	SqlLiteTransaction(void *db);
	~SqlLiteTransaction();

	virtual void Commit();
	virtual void Rollback();
};


inline void TypeValidate( const SqlLiteTransaction& prm )
{
}

inline void TypeValidate( SqlLiteTransaction* prm )
{
}

inline void TypeCheckMem( const SqlLiteTransaction& prm )
{
}

inline void TypeCheckMem( SqlLiteTransaction* prm )
{
}

REGISTER_TYPEOF( 440, SqlLiteTransaction );
REGISTER_TYPEOF( 442, SqlLiteTransactionPtr );
REGISTER_TYPEOF( 444, SqlLiteTransactionRef );

/** @} */
}
#endif
