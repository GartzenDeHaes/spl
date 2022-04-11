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
#ifndef _transaction_h
#define _transaction_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/RefCountPtr.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

class Transaction;
typedef RefCountPtr<Transaction> TransactionPtr;

/** @brief Abstract base class for transactions.
 *
 */
class Transaction
{
public:
	Transaction();
	~Transaction();

	virtual void Commit() = 0;
	virtual void Rollback() = 0;
};

inline void TypeValidate( const Transaction& prm )
{
}

inline void TypeValidate( Transaction* prm )
{
}

inline void TypeCheckMem( const Transaction& prm )
{
}

inline void TypeCheckMem( Transaction* prm )
{
}

/** @} */
}
#endif
