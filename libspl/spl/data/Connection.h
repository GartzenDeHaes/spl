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
#ifndef _idbconn_h
#define _idbconn_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/data/Command.h>
#include <spl/Memory.h>
#include <spl/data/RecordSet.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/data/Transaction.h>

namespace spl
{
/** 
 * @defgroup database Database
 * @{
 */

/** 
 * @defgroup datamodel Data Access Classes
 * @{
 */

class Connection;
typedef RefCountPtr<Connection> ConnectionPtr;

/** @brief Abstract base class for database connections.
 *	@ref MySqlConnection
 *	@ref SqlLiteConnection
 */
class Connection : public IMemoryValidate
{
private:
	// Copy constructor doesn't make sense for this class
	inline Connection(const Connection& con) {}
	inline void operator =(const Connection& con) {}

protected:
	String m_host;
	String m_database;
	String m_uid;
	String m_pw;

public:
	Connection(const String& serverOrIP, const String& database, const String& uid, const String& pw);
	virtual ~Connection();

	/// @brief Open the connection, ready for use after Open.
	virtual void Open() = 0;

	/// @brief Close the connection, free resources.
	virtual void Close() = 0;

	/// \brief Change to a different database
	virtual void ChangeDatabase(const String& db) = 0;

	virtual TransactionPtr BeginTransaction() = 0;

	virtual CommandPtr CreateCommand() = 0;
	virtual CommandPtr CreateCommand(const String& cmdText) = 0;

	virtual int ExecuteNonQuery(const String& sql) = 0;
	virtual RecordSetPtr ExecuteQuery(const String& sql) = 0;

	/// @brief If multiple threads are accessing the connection, each thread needs to call this.
	virtual void RegisterThread() = 0;

	/// @brief If multiple threads are accessing the connection, each thread needs to call this on exit.
	virtual void RegisterThreadExit() = 0;

	/// @brief Setting to true requires Command's to query the parameter names from the stored procedure -- default: false.
	virtual void AllowOutOfOrderCommandParameters(bool allow) = 0;

	static ConnectionPtr GetConnection(const String& connectString);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
/** @} */
}
#endif
